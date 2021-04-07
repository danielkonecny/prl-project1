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

#include <mpi.h>

#include "pms.h"

using namespace std;


#define TAG 0
#define INPUT_FILE_NAME "numbers"

enum State {Wait, PopOne, PopTwo};


vector<int> read_input(string file_name) {
    int number;
    vector<int> unsorted_numbers;

    fstream fin;
    fin.open(file_name, ios::in);

    // cout << "LOADED NUMBERS:" << endl;

    number = fin.get();
    for (unsigned index = 0; fin.good(); index++) {
        unsorted_numbers.push_back(number);
        // cout << index << ": " << number << endl;
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


int main (int argc, char *argv[]) {
	int numprocs;
	int myid;
	MPI_Status stat;

	MPI_Init(&argc, &argv); 					// MPI initialization.
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 	// Number of processes running.
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); 		// ID of this particular process.

    if (myid == 0) {
        vector<int> unsorted_numbers;
        unsorted_numbers = read_input(INPUT_FILE_NAME);
        print_unsorted_numbers(unsorted_numbers);
        
        for(auto const& number: unsorted_numbers) {
            MPI_Send(&number, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
            // cout << "Process " << myid << " sent number " << number << "." << endl;
        }
    }
    else if (myid == 1) {
        queue<int> queue1;
        queue<int> queue2;

        bool ready = false;
        int loaded = 0;
        int sent = 0;
        int upper_sent = 0;
        int lower_sent = 0;

        while (sent < 16) {
            int new_number;

            if (loaded < 16) {
                MPI_Recv(&new_number, 1, MPI_INT, myid-1, TAG, MPI_COMM_WORLD, &stat);
                // cout << "Process " << myid << " recieved number " << new_number << "." << endl;

                if (loaded % 2 < 1) {
                    queue1.push(new_number);
                    // cout << "Number " << new_number << " in queue 1." << endl;
                }
                else {
                    queue2.push(new_number);
                    // cout << "Number " << new_number << " in queue 2." << endl;
                }
                loaded++;
            }

            if (!ready) {
                // cout << "Queue 1 - " << queue1.size() << "." << endl;
                // cout << "Queue 2 - " << queue2.size() << "." << endl;
                if (queue1.size() >= 1 && queue2.size() >= 1) {
                    // cout << "Process " << myid << " queue ready." << endl;
                    ready = true;
                }
            }
            else {
                if (upper_sent < 1 && lower_sent < 1) {
                    if (queue1.front() <= queue2.front()) {
                        new_number = queue1.front();
                        queue1.pop();
                        upper_sent++;
                    }
                    else {
                        new_number = queue2.front();
                        queue2.pop();
                        lower_sent++;
                    }
                }
                else if (upper_sent == 1 && lower_sent == 0) {
                    new_number = queue2.front();
                    queue2.pop();
                    upper_sent = 0;
                }
                else if (upper_sent == 0 && lower_sent == 1) {
                    new_number = queue1.front();
                    queue1.pop();
                    lower_sent = 0;
                }
                else {
                    cout << "ERROR: Upper and lower both on 1 or sth." << endl;
                }
                MPI_Send(&new_number, 1, MPI_INT, myid+1, TAG, MPI_COMM_WORLD);
                cout << "Process " << myid << " sent number " << new_number << "." << endl;
                sent++;
            }
        }
    }
    else if (myid == 2) {
        queue<int> queue1;
        queue<int> queue2;

        bool ready = false;
        int loaded = 0;
        int sent = 0;
        int upper_sent = 0;
        int lower_sent = 0;

        while (sent < 16) {
            int new_number;

            if (loaded < 16) {
                MPI_Recv(&new_number, 1, MPI_INT, myid-1, TAG, MPI_COMM_WORLD, &stat);

                if (loaded % 4 < 2) {
                    queue1.push(new_number);
                }
                else {
                    queue2.push(new_number);
                }
                loaded++;
            }

            if (!ready) {
                if ((queue1.size() >= 2 && queue2.size() >= 1) ||
                    (queue1.size() >= 1 && queue2.size() >= 2)) {
                    ready = true;
                }
            }
            else {
                if (upper_sent < 2 && lower_sent < 2) {
                    if (queue1.front() <= queue2.front()) {
                        new_number = queue1.front();
                        queue1.pop();
                        upper_sent++;
                    }
                    else {
                        new_number = queue2.front();
                        queue2.pop();
                        lower_sent++;
                    }
                }
                else if (upper_sent == 2 && lower_sent < 1) {
                    new_number = queue2.front();
                    queue2.pop();
                    lower_sent++;
                }
                else if (upper_sent == 2 && lower_sent == 1) {
                    new_number = queue2.front();
                    queue2.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else if (upper_sent < 1 && lower_sent == 2) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent++;
                }
                else if (upper_sent == 1 && lower_sent == 2) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else {
                    cout << "ERROR: Upper and lower both on 2 or sth." << endl;
                }
                MPI_Send(&new_number, 1, MPI_INT, myid+1, TAG, MPI_COMM_WORLD);
                cout << "Process " << myid << " sent number " << new_number << "." << endl;
                sent++;
            }
        }
    }
    else if (myid == 3) {
        queue<int> queue1;
        queue<int> queue2;

        bool ready = false;
        int loaded = 0;
        int sent = 0;
        int upper_sent = 0;
        int lower_sent = 0;

        while (sent < 16) {
            int new_number;

            if (loaded < 16) {
                MPI_Recv(&new_number, 1, MPI_INT, myid-1, TAG, MPI_COMM_WORLD, &stat);

                if (loaded % 8 < 4) {
                    queue1.push(new_number);
                }
                else {
                    queue2.push(new_number);
                }
                loaded++;
            }

            if (!ready) {
                if ((queue1.size() >= 4 && queue2.size() >= 1) ||
                    (queue1.size() >= 1 && queue2.size() >= 4)) {
                    ready = true;
                }
            }
            else {
                if (upper_sent < 4 && lower_sent < 4) {
                    if (queue1.front() <= queue2.front()) {
                        new_number = queue1.front();
                        queue1.pop();
                        upper_sent++;
                    }
                    else {
                        new_number = queue2.front();
                        queue2.pop();
                        lower_sent++;
                    }
                }
                else if (upper_sent == 4 && lower_sent < 3) {
                    new_number = queue2.front();
                    queue2.pop();
                    lower_sent++;
                }
                else if (upper_sent == 4 && lower_sent == 3) {
                    new_number = queue2.front();
                    queue2.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else if (upper_sent < 3 && lower_sent == 4) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent++;
                }
                else if (upper_sent == 3 && lower_sent == 4) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else {
                    cout << "ERROR: Upper and lower both on 4 or sth." << endl;
                }
                MPI_Send(&new_number, 1, MPI_INT, myid+1, TAG, MPI_COMM_WORLD);
                cout << "Process " << myid << " sent number " << new_number << "." << endl;
                sent++;
            }
        }
    }
    else if (myid == 4) {
        vector<int> sorted_numbers;
        queue<int> queue1;
        queue<int> queue2;

        bool ready = false;
        int loaded = 0;
        int sent = 0;
        int upper_sent = 0;
        int lower_sent = 0;

        while (sent < 16) {
            int new_number;

            if (loaded < 16) {
                MPI_Recv(&new_number, 1, MPI_INT, myid-1, TAG, MPI_COMM_WORLD, &stat);

                if (loaded % 16 < 8) {
                    queue1.push(new_number);
                }
                else {
                    queue2.push(new_number);
                }
                loaded++;
            }

            if (!ready) {
                if ((queue1.size() >= 8 && queue2.size() >= 1) ||
                    (queue1.size() >= 1 && queue2.size() >= 8)) {
                    ready = true;
                }
            }
            else {
                if (upper_sent < 8 && lower_sent < 8) {
                    if (queue1.front() <= queue2.front()) {
                        new_number = queue1.front();
                        queue1.pop();
                        upper_sent++;
                    }
                    else {
                        new_number = queue2.front();
                        queue2.pop();
                        lower_sent++;
                    }
                }
                else if (upper_sent == 8 && lower_sent < 7) {
                    new_number = queue2.front();
                    queue2.pop();
                    lower_sent++;
                }
                else if (upper_sent == 8 && lower_sent == 7) {
                    new_number = queue2.front();
                    queue2.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else if (upper_sent < 7 && lower_sent == 8) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent++;
                }
                else if (upper_sent == 7 && lower_sent == 8) {
                    new_number = queue1.front();
                    queue1.pop();
                    upper_sent = 0;
                    lower_sent = 0;
                }
                else {
                    cout << "ERROR: Upper and lower both on 4 or sth." << endl;
                }
                sorted_numbers.push_back(new_number);
                sent++;
            }
        }
        print_sorted_numbers(sorted_numbers);
    }
    else {
        cerr << "ERROR: Unknown process ID: " << myid << "." << endl;
    }

	MPI_Finalize();
	return 0;
 }
