/**
 * PRL - Project - Pipeline Merge Sort
 * Implementation of parallel sorting algorithm called Pipeline Merge Sort.
 * @file            pms.h
 * @version			1.0
 * @author			Daniel Konecny (xkonec75)
 * @organisation	Brno University of Technology - Faculty of Information Technologies
 * @date			09. 04. 2021
 */

#include <string>
#include <vector>

#include <mpi.h>

using namespace std;

vector<int> read_input(string file_name);

void print_unsorted_numbers(vector<int> numbers);

void print_sorted_numbers(vector<int> numbers);

void check_and_reset_counters(int *upper_sent, int *lower_sent, int queue_length);

vector<int> process_new_number(int process_id, int new_number, vector<int> sorted_numbers);

vector<int> middle_processor(MPI_Status stat, int process_id, vector<int> sorted_numbers);

int main (int argc, char *argv[]);
