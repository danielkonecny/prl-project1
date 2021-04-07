/**
 * PRL - Project - Pipeline Merge Sort
 * Implementation of parallel sorting algorithm called Pipeline Merge Sort.
 * @file            pms.cpp
 * @version			1.0
 * @author			Daniel Konecny (xkonec75)
 * @organisation	Brno University of Technology - Faculty of Information Technologies
 * @date			09. 04. 2021
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

#include <mpi.h>

#include "pms.h"

using namespace std;

#define TAG 0
#define INPUT_FILE_NAME "numbers"
#define NUMBER_COUNT 16

vector<int> read_input(string file_name) {
    int number;
    vector<int> unsorted_numbers;

    fstream fin;
    fin.open(file_name, ios::in);

    number = fin.get();
    for (unsigned index = 0; fin.good(); index++) {
        unsorted_numbers.push_back(number);
        number = fin.get();
    }

    fin.close();
    return unsorted_numbers;
}

void print_unsorted_numbers(vector<int> numbers) {
    for(auto const& number: numbers) {
        cout << number << " ";
    }
    cout << endl;
}

void print_sorted_numbers(vector<int> numbers) {
    for(auto const& number: numbers) {
        cout << number << endl;
    }
}

void check_and_reset_counters(int *upper_sent, int *lower_sent, int queue_length) {
    if (*upper_sent == queue_length && *lower_sent == queue_length) {
        *upper_sent = 0;
        *lower_sent = 0;
    }
}

vector<int> process_new_number(int process_id, int new_number, vector<int> sorted_numbers) {
    if (process_id < 4) {
        MPI_Send(&new_number, 1, MPI_INT, process_id+1, TAG, MPI_COMM_WORLD);
    }
    else {
        sorted_numbers.push_back(new_number);
    }
    return sorted_numbers;
}

vector<int> middle_processor(MPI_Status stat, int process_id, vector<int> sorted_numbers) {
    queue<int> queue_upper;
    queue<int> queue_lower;
    bool ready = false;
    int loaded = 0;
    int sent = 0;
    int upper_sent = 0;
    int lower_sent = 0;
    int power = pow(2, process_id);
    int queue_length = pow(2, process_id-1);

    while (sent < NUMBER_COUNT) {
        int new_number;

        if (loaded < NUMBER_COUNT) {
            MPI_Recv(&new_number, 1, MPI_INT, process_id-1, TAG, MPI_COMM_WORLD, &stat);

            if (loaded % power < queue_length) {
                queue_upper.push(new_number);
            }
            else {
                queue_lower.push(new_number);
            }
            loaded++;
        }

        if (!ready) {
            if ((queue_upper.size() >= queue_length && queue_lower.size() >= 1) ||
                (queue_upper.size() >= 1 && queue_lower.size() >= queue_length)) {
                ready = true;
            }
            else {
                continue;
            }
        }

        // Take from lower queue because upper queue is empty or number in lower queue is lower.
        if (upper_sent == queue_length ||
            (lower_sent != queue_length && queue_upper.front() > queue_lower.front())) {
            new_number = queue_lower.front();
            queue_lower.pop();
            lower_sent++;
        }
        // Take from upper queue because lower queue is empty or number in upper queue is lower.
        else if (lower_sent == queue_length || queue_upper.front() <= queue_lower.front()) {
            new_number = queue_upper.front();
            queue_upper.pop();
            upper_sent++;
        }

        check_and_reset_counters(&upper_sent, &lower_sent, queue_length);

        sorted_numbers = process_new_number(process_id, new_number, sorted_numbers);
        sent++;
    }
    return sorted_numbers;
}

int main (int argc, char *argv[]) {
	int process_id;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    vector<int> sorted_numbers;

    if (process_id == 0) {
        vector<int> unsorted_numbers;
        unsorted_numbers = read_input(INPUT_FILE_NAME);
        print_unsorted_numbers(unsorted_numbers);
        
        for(auto const& number: unsorted_numbers) {
            MPI_Send(&number, 1, MPI_INT, process_id+1, TAG, MPI_COMM_WORLD);
        }
    }
    else if (process_id >= 1 && process_id <= 4) {
        sorted_numbers = middle_processor(stat, process_id, sorted_numbers);
    }

    if (process_id == 4) {
        print_sorted_numbers(sorted_numbers);
    }

	MPI_Finalize();
	return 0;
 }
