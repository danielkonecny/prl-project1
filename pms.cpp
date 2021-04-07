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

#include <mpi.h>

#include "pms.h"

using namespace std;


#define BUFSIZE 128
#define INPUT_FILE_NAME "numbers"


vector<int> read_input(string file_name) {
    int number;
    vector<int> unsorted_numbers;

    fstream fin;
    fin.open(file_name, ios::in);

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
	char buff[BUFSIZE];
	int numprocs;
	int myid;
	MPI_Status stat;
    vector<int> unsorted_numbers;
    vector<int> sorted_numbers;

	MPI_Init(&argc, &argv); 					// MPI initialization.
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 	// Number of processes running.
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); 		// ID of this particular process.

    if (myid == 0) {
        unsorted_numbers = read_input(INPUT_FILE_NAME);
        print_unsorted_numbers(unsorted_numbers);
        print_sorted_numbers(unsorted_numbers);
    }



	MPI_Finalize();
	return 0;
 }
