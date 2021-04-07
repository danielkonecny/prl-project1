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

using namespace std;

vector<int> read_input(string file_name);

void print_unsorted_numbers(vector<int> numbers);

void print_sorted_numbers(vector<int> numbers);
