#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;

namespace P3 {
	const int size = 10000;
	const int tests = 16;

	int* unsorted = new int[size];
	int* to_sort = new int[size];

	int* correct = new int[size];
}

namespace P3vis {
	const int size = 64;

	int* unsorted = new int[size];
	int* to_sort = new int[size];

	int* correct = new int[size];
}

// Functions
void random_lists(int* array1, int* array2, int size); // 15-bit
void random_lists30(int* array1, int* array2, int size); // 30-bit
void random_lists32(int* array1, int* array2, int size); // 32-bit
void copyarray(int* array1, int* array2, int size); // 2 = 1
void write_csv(ofstream* Out, int* array, string list, double time, bool values); // csv out
void repetitive_stuff(chrono::steady_clock::time_point _start,
	chrono::steady_clock::time_point _end,
	string _name, int _i, vector<double>* _durations); // general reset
void test_valid(int* array1, int* array2, int size, bool print_if_true);
// Helpers
void combine2array(int* array1, int* array2, int size1, int size2);
void combinearrays(int* array, int size, int thread_count);
void heapify(int* array, int size, int index);
void merge(int* array, int left, int mid, int right);
int partitionArray(int* array, int begin, int end);
// Main Sorting
void BubbleSort(int* array, int size);
void stdsort(int* array, int size);
void HeapSort(int* array, int size);
void MergeSort(int* array, int begin, int end);
void QuickSort(int* array, int begin, int end);
// Multi-Main Sorting // Accepts int values 2->size
void MultiBubbleSort(int* array, int size, int thread_count);
void Multistdsort(int* array, int size, int thread_count);
void MultiHeapSort(int* array, int size, int thread_count);
void MultiMergeSort(int* array, int size, int thread_count);
void MultiQuickSort(int* array, int size, int thread_count);
// Visual Sorting
void TransferRandomNumbers(int* arrayTo, int arrayToSize, int* arrayFrom, int arrayFromSize); // just for fun ig
void TransmitArray(int* array, int side, ofstream* Out, string inFile, string outFile);
void BubbleSortVis(int* array, int size, ofstream* Out, string inFile, string outFile);
void HeapSortVis(int* array, int size, ofstream* Out, string inFile, string outFile);
void MergeSortVis(int* array, int size, ofstream* Out, string inFile, string outFile);
void QuickSortVis(int* array, int size, ofstream* Out, string inFile, string outFile);
void MultiBubbleSortVis(int* array, int size, int thread_count, ofstream* Out, string inFile, string outFile);

int main() {
	cout << "Test or Visuals?" << endl;
	string input;
	cin >> input;

	//------// Variables and General Setup of the Program //------//
	// Time Variables
	chrono::steady_clock::time_point start;
	chrono::steady_clock::time_point end;
	vector<double> durations;
	// Naming of Algorithms
	string name;
	vector<string> names;
	bool first = true;
	// Output
	ofstream Out;

	if (input == "Test" || input == "test" || input == "T" || input == "t") {
		Out.open("output.csv", ios_base::binary);
		if (!Out.is_open()) { std::cout << "FAILED OPENING" << '\n'; }
		// Prep Output
		Out << "Algorithm" << ',' << ',' << "Average" << ',' << "Speedup" << ',' << ',' << "Values: " << P3::size << ',' << ',' << "Tests: " << P3::tests << '\n' << endl;

		bool Bubble = false;
		bool std = true; // do not turn off due to "correct" value
		bool Heap = true;
		bool Merge = true;
		bool Quick = true;

		bool BubbleMulti = false;
		bool stdMulti = true;
		bool HeapMulti = true;
		bool MergeMulti = true;
		bool QuickMulti = true;

		// Main Algorithm-Running
		int number_of_threads = 8;
		for (int i = 0; i < P3::tests; i++) {
			random_lists(P3::unsorted, P3::to_sort, P3::size);
			//write_csv(&Out, P3::unsorted, "Unsorted " + to_string(i), 0, true);
			cout << "Sorting List " << i << '\n';

			if (Bubble) {
				name = "Bubble Sort"; // name
				if (first) { names.push_back(name); } // push name into list of algs
				start = chrono::high_resolution_clock::now(); // start time
				BubbleSort(P3::to_sort, P3::size); // run algorithm
				end = chrono::high_resolution_clock::now(); // end time
				//write_csv(&Out, P3::to_sort, name, 0, true); // write out the info, array if true
				repetitive_stuff(start, end, name, i, &durations); // reset for next function
			}

			if (std) {
				name = "std::sort";
				if (first) { names.push_back(name); }
				start = chrono::high_resolution_clock::now();
				stdsort(P3::to_sort, P3::size);
				end = chrono::high_resolution_clock::now();
				copyarray(P3::to_sort, P3::correct, P3::size); // to have reference to test validity of new algs
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (Heap) {
				name = "Heap Sort";
				if (first) { names.push_back(name); }
				start = chrono::high_resolution_clock::now();
				HeapSort(P3::to_sort, P3::size);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (Merge) {
				name = "Merge Sort";
				if (first) { names.push_back(name); }
				start = chrono::high_resolution_clock::now();
				MergeSort(P3::to_sort, 0, P3::size - 1);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (Quick) {
				name = "Quick Sort";
				if (first) { names.push_back(name); }
				start = chrono::high_resolution_clock::now();
				QuickSort(P3::to_sort, 0, P3::size - 1);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			// Multi-Threaded because it is fun

			if (BubbleMulti) {
				name = "-Thread Bubble Sort";
				number_of_threads = 32;
				if (first) { names.push_back(to_string(number_of_threads) + name); }
				name = to_string(number_of_threads) + name;
				start = chrono::high_resolution_clock::now();
				MultiBubbleSort(P3::to_sort, P3::size, number_of_threads);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (stdMulti) {
				name = "-Thread std::sort";
				number_of_threads = 8;
				if (first) { names.push_back(to_string(number_of_threads) + name); }
				name = to_string(number_of_threads) + name;
				start = chrono::high_resolution_clock::now();
				Multistdsort(P3::to_sort, P3::size, number_of_threads);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (HeapMulti) {
				name = "-Thread Heap Sort";
				number_of_threads = 8;
				if (first) { names.push_back(to_string(number_of_threads) + name); }
				name = to_string(number_of_threads) + name;
				start = chrono::high_resolution_clock::now();
				MultiHeapSort(P3::to_sort, P3::size, number_of_threads);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (MergeMulti) {
				name = "-Thread Merge Sort";
				number_of_threads = 2;
				if (first) { names.push_back(to_string(number_of_threads) + name); }
				name = to_string(number_of_threads) + name;
				start = chrono::high_resolution_clock::now();
				MultiMergeSort(P3::to_sort, P3::size, number_of_threads);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			if (QuickMulti) {
				name = "-Thread Quick Sort";
				number_of_threads = 16;
				if (first) { names.push_back(to_string(number_of_threads) + name); }
				name = to_string(number_of_threads) + name;
				start = chrono::high_resolution_clock::now();
				MultiQuickSort(P3::to_sort, P3::size, number_of_threads);
				end = chrono::high_resolution_clock::now();
				test_valid(P3::to_sort, P3::correct, P3::size, false);
				repetitive_stuff(start, end, name, i, &durations);
			}

			// Enable if wanting an extra space
			cout << '\n';
			//Out << '\n';
			first = false;
		}

		// Writing Out Final Data
		int algorithms = (int)names.size();
		double total = 0;
		double average_time;
		string base_name = "std::sort"; // reference algorithm for speedup calc
		double base_time = 1; // reference time for speedup
		vector<double> times_out;
		for (int i = 0; i < algorithms; i++) {
			Out << names[i] << ',' << ',';
			for (int j = i; j < (durations.size()); j += algorithms) {
				total += durations[j];
			}
			average_time = total / P3::tests;
			if (names[i] == base_name) {
				base_time = average_time;
			}
			Out << average_time << ',' << base_time / average_time << ',' << ',';
			if (true) {
				for (int j = i; j < (durations.size()); j += algorithms) {
					Out << durations[j] << ',';
				}
			}
			Out << '\n';
			total = 0;
		}
		Out.close();
	}

	// ====== Visual Half ======

	else if (input == "Visuals" || input == "visuals" || input == "V" || input == "v") {
		// Visuals
		string outFile = "cppout.csv";
		string inFile = "cppin.csv";
		random_lists(P3vis::unsorted, P3vis::to_sort, P3vis::size);
		TransmitArray(P3vis::unsorted, P3vis::size, &Out, inFile, outFile);
		copyarray(P3vis::unsorted, P3vis::correct, P3vis::size);
		sort(P3vis::correct, P3vis::correct + P3vis::size);

		BubbleSortVis(P3vis::to_sort, P3vis::size, &Out, inFile, outFile);
		test_valid(P3vis::to_sort, P3vis::correct, P3vis::size, false);
		copyarray(P3vis::unsorted, P3vis::to_sort, P3vis::size);

		HeapSortVis(P3vis::to_sort, P3vis::size, &Out, inFile, outFile);
		test_valid(P3vis::to_sort, P3vis::correct, P3vis::size, false);
		copyarray(P3vis::unsorted, P3vis::to_sort, P3vis::size);

		MergeSortVis(P3vis::to_sort, P3vis::size, &Out, inFile, outFile);
		test_valid(P3vis::to_sort, P3vis::correct, P3vis::size, false);
		copyarray(P3vis::unsorted, P3vis::to_sort, P3vis::size);

		QuickSortVis(P3vis::to_sort, P3vis::size, &Out, inFile, outFile);
		test_valid(P3vis::to_sort, P3vis::correct, P3vis::size, false);
		copyarray(P3vis::unsorted, P3vis::to_sort, P3vis::size);

		MultiBubbleSortVis(P3vis::to_sort, P3vis::size, 2, &Out, inFile, outFile);
		test_valid(P3vis::to_sort, P3vis::correct, P3vis::size, false);
		copyarray(P3vis::unsorted, P3vis::to_sort, P3vis::size);
	}
	else {

	}

	// Wrap it up

	delete[] P3::unsorted;
	delete[] P3::to_sort;
	delete[] P3::correct;
	return 0;
}

// Functions

void random_lists(int* array1, int* array2, int size) {
	chrono::steady_clock::time_point now;
	now = chrono::high_resolution_clock::now();
	long long seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	int random;
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] = random;
		array2[i] = random;
	}
}

void random_lists30(int* array1, int* array2, int size) {
	chrono::steady_clock::time_point now;
	now = chrono::high_resolution_clock::now();
	long long seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	int random;
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] = random << 15;
		array2[i] = random << 15;
	}
	now = chrono::high_resolution_clock::now();
	seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] += random;
		array2[i] += random;
	}
}

void random_lists32(int* array1, int* array2, int size) { // generates negative and positive 32-bit
	chrono::steady_clock::time_point now;
	now = chrono::high_resolution_clock::now();
	long long seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	int random;
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] = random << 17;
		array2[i] = random << 17;
	}
	now = chrono::high_resolution_clock::now();
	seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] += random << 2;
		array2[i] += random << 2;
	}
	now = chrono::high_resolution_clock::now();
	seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	for (int i = 0; i < size; i++) {
		random = rand();
		array1[i] += random % 4;
		array2[i] += random % 4;
	}
}

void copyarray(int* array1, int* array2, int size) {
	for (int i = 0; i < size; i++) {
		array2[i] = array1[i];
	}
}

void write_csv(ofstream* Out, int* array, string list, double time, bool values) {
	*Out << list << ',' << ',';
	if (time) {
		*Out << time;
	}
	*Out << ',' << ',';
	if (values) {
		for (int i = 0; i < P3::size; i++) {
			*Out << array[i] << ',';
		}
	}
	*Out << '\n';
}

void repetitive_stuff(chrono::steady_clock::time_point _start, chrono::steady_clock::time_point _end, string _name, int _i, vector<double>* _durations) {
	chrono::duration<long long, nano> time = _end - _start;
	double duration = (double)time.count() / 1000000;
	cout << _name << " " << _i << " took " << duration << " milliseconds." << '\n';
	_durations->push_back(duration);
	copyarray(P3::unsorted, P3::to_sort, P3::size);
}

void test_valid(int* array1, int* array2, int size, bool print_if_true) {
	for (int i = 0; i < size; i++) {
		if (array1[i] != array2[i]) {
			cout << "Array1 != Array2 @ " << i << '\n';
			return;
		}
	}
	if (print_if_true) {
		cout << "Array1 == Array2" << '\n';
	}
}

// Helpers

// yes i found a slightly cleaner way to do it in MultiBubbleSortVis, but i dont wanna break existing code for 0.1% gains in performance
void combine2array(int* array1, int* array2, int size1, int size2) { // two adjacent arrays being sorted into one bigger array
	bool array1done = false;
	bool array2done = false;
	int array1index = 0;
	int array2index = 0;
	int left_off;
	int* output_temp = new int[size1 + size2];
	for (int i = 0; i < size1 + size2 - 1; i++) {
		if (array1[array1index] <= array2[array2index]) {
			output_temp[i] = array1[array1index];
			array1index++;
			if (array1index == size1) {
				array1done = true;
				left_off = i + 1;
				break;
			}
		}
		else {
			output_temp[i] = array2[array2index];
			array2index++;
			if (array2index == size2) {
				array2done = true;
				left_off = i + 1;
				break;
			}
		}
	}
	if (array1done) {
		for (int i = left_off; i < size1 + size2; i++) {
			output_temp[i] = array2[array2index];
			array2index++;
		}
	}
	else {
		for (int i = left_off; i < size1 + size2; i++) {
			output_temp[i] = array1[array1index];
			array1index++;
		}
	}
	int output_index = 0;
	for (int i = 0; i < size1; i++) {
		array1[i] = output_temp[output_index];
		output_index++;
	}
	for (int i = 0; i < size2; i++) {
		array2[i] = output_temp[output_index];
		output_index++;
	}
	delete[] output_temp;
}

void combinearrays(int* array, int size, int thread_count) {
	vector<thread> threads;
	int normal_size = size / thread_count;
	int remainder = size % thread_count;
	int last = normal_size + remainder;
	int array_count = thread_count;
	int current_threads = thread_count / 2;
	bool odd = array_count % 2;
	bool one_more_merge = false;
	while (current_threads) {
		for (int i = 0; i < (current_threads - 1); i++) { // make ends merge better
			threads.push_back(thread(combine2array, array + (2 * i * normal_size), array + ((2 * i + 1) * normal_size), normal_size, normal_size));
		}
		odd = array_count % 2;
		if (odd) {
			threads.push_back(thread(combine2array, array + ((2 * (current_threads - 1)) * normal_size), array + ((2 * (current_threads - 1) + 1) * normal_size), normal_size, normal_size));
			array_count = array_count / 2 + 1;
			if (array_count == 3) {
				one_more_merge = true;
			}
		}
		else { // if even
			threads.push_back(thread(combine2array, array + ((2 * (current_threads - 1)) * normal_size), array + ((2 * (current_threads - 1) + 1) * normal_size), normal_size, last));
			array_count /= 2;
			last += normal_size;
		}
		normal_size *= 2;
		for (int i = 0; i < current_threads; i++) {
			threads[i].join();
		}
		if (odd && current_threads % 2) {
			current_threads = current_threads / 2 + 1;
		}
		else {
			current_threads /= 2;
		}
		threads.clear();
	}
	if (one_more_merge && 0) {
		threads.push_back(thread(combine2array, array, array + normal_size, normal_size, last));
		threads[0].join();
		threads.clear();
	}
}

void heapify(int* array, int size, int index) {
	// Heapify a subtree

	// Initialize indices

	int largest = index;
	int left = (2 * index) + 1;
	int right = (2 * index) + 2;

	if (left < size && array[left] > array[largest])  // The left child greater than root
		largest = left;

	if (right < size && array[right] > array[largest])  // The right child is is the new largest
		largest = right;

	if (largest != index) {  // Lergest is not the root

		swap(array[index], array[largest]);
		heapify(array, size, largest); // Heapify the sub-tree
	}

	// Reference: @ https://www.geeksforgeeks.org/heap-sort
}

void merge(int* array, int left, int mid, int right) {

}

int partitionArray(int* array, int begin, int end) {
	// Partition an array for quick sorting

	int pivotPoint = array[begin];
	int count = 0;

	for (int i = begin + 1; i <= end; i++) {
		if (array[i] <= pivotPoint)
			count++;
	}

	// Giving pivot element its correct position

	int pivotIndex = begin + count;
	swap(array[pivotIndex], array[begin]);

	// Sort to the left and right of the pivot index

	while (begin < pivotIndex && end > pivotIndex) {

		while (array[begin] <= pivotPoint)
			begin++;

		while (array[end] > pivotPoint)
			end--;

		if (begin < pivotIndex && end > pivotIndex)
			swap(array[begin++], array[end--]);
	}

	return pivotIndex;

	// Reference: @ https://www.geeksforgeeks.org/quick-sort/?ref=lbp
}

// Algorithms

void BubbleSort(int* array, int size) { // Bubble Sort
	bool swapped;
	for (int i = 0; i < size - 1; i++) {
		swapped = false;
		for (int j = 0; j < size - i - 1; j++) {
			if (array[j] > array[j + 1]) {
				swap(array[j], array[j + 1]);
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
	// Credit for this bubble sort goes to GeeksForGeeks @ https://www.geeksforgeeks.org/bubble-sort/
}

void stdsort(int* array, int size) {
	sort(array, array + size);
}

void HeapSort(int* array, int size) {
	// Sort an array with heapsort

	for (int i = (size / 2) - 1; i >= 0; i--)  // Build heap in place
		heapify(array, size, i);

	for (int i = size - 1; i >= 0; i--) {
		swap(array[0], array[i]);

		heapify(array, i, 0);
	}
}

void MergeSort(int* array, int begin, int end) {

}

void QuickSort(int* array, int begin, int end) {
	// Sort an array with quicksort

	if (begin >= end)  // Base case of recursion
		return;

	int pivotIndex = partitionArray(array, begin, end);  // Partition the array

	QuickSort(array, begin, pivotIndex - 1);  // Sort left of pivot
	QuickSort(array, pivotIndex + 1, end);  // Sort right of pivot
}

// Multi-Main Sorting

void MultiBubbleSort(int* array, int size, int thread_count) { // Multithreaded Bubble Sort
	vector<thread> threads;
	int max_threads = thread_count;
	int normal_size = size / max_threads;
	for (int i = 0; i < (max_threads - 1); i++) {
		threads.push_back(thread(BubbleSort, array + (i * normal_size), size / max_threads));
	}
	threads.push_back(thread(BubbleSort, array + ((max_threads - 1) * normal_size), normal_size + (size % max_threads)));
	for (int i = 0; i < max_threads; i++) {
		threads[i].join();
	}
	threads.clear();
	combinearrays(array, size, thread_count);
}

void Multistdsort(int* array, int size, int thread_count) { // Multithreaded std::sort
	vector<thread> threads;
	int max_threads = thread_count;
	int normal_size = size / max_threads;
	for (int i = 0; i < (max_threads - 1); i++) {
		threads.push_back(thread(stdsort, array + (i * normal_size), size / max_threads));
	}
	threads.push_back(thread(stdsort, array + ((max_threads - 1) * normal_size), normal_size + (size % max_threads)));
	for (int i = 0; i < max_threads; i++) {
		threads[i].join();
	}
	threads.clear();
	combinearrays(array, size, thread_count);
}

void MultiHeapSort(int* array, int size, int thread_count) { // Multithreaded Heap Sort
	vector<thread> threads;
	int max_threads = thread_count;
	int normal_size = size / max_threads;
	for (int i = 0; i < (max_threads - 1); i++) {
		threads.push_back(thread(HeapSort, array + (i * normal_size), size / max_threads));
	}
	threads.push_back(thread(HeapSort, array + ((max_threads - 1) * normal_size), normal_size + (size % max_threads)));
	for (int i = 0; i < max_threads; i++) {
		threads[i].join();
	}
	threads.clear();
	combinearrays(array, size, thread_count);
}

void MultiMergeSort(int* array, int size, int thread_count) { // Multithreaded Merge Sort
	vector<thread> threads;
	int max_threads = thread_count;
	int normal_size = size / max_threads;
	for (int i = 0; i < (max_threads - 1); i++) {
		threads.push_back(thread(MergeSort, array, (i * normal_size), (i * normal_size) + normal_size - 1));
	}
	if (size % max_threads == 0) {
		threads.push_back(thread(MergeSort, array, ((max_threads - 1) * normal_size), ((max_threads - 1) * normal_size) + normal_size - 1));
	}
	else {
		threads.push_back(thread(MergeSort, array, ((max_threads - 1) * normal_size), ((max_threads - 1) * normal_size) + (size % max_threads) - 1));
	}
	for (int i = 0; i < max_threads; i++) {
		threads[i].join();
	}
	threads.clear();
	combinearrays(array, size, thread_count);
}

void MultiQuickSort(int* array, int size, int thread_count) { // Multithreaded Quick Sort
	vector<thread> threads;
	int max_threads = thread_count;
	int normal_size = size / max_threads;
	for (int i = 0; i < (max_threads - 1); i++) {
		threads.push_back(thread(QuickSort, array, (i * normal_size), (i * normal_size) + normal_size - 1));
	}
	if (size % max_threads == 0) {
		threads.push_back(thread(QuickSort, array, ((max_threads - 1) * normal_size), ((max_threads - 1) * normal_size) + normal_size - 1));
	}
	else {
		threads.push_back(thread(QuickSort, array, ((max_threads - 1) * normal_size), ((max_threads - 1) * normal_size) + (size % max_threads) - 1));
	}
	for (int i = 0; i < max_threads; i++) {
		threads[i].join();
	}
	threads.clear();
	combinearrays(array, size, thread_count);
}

// Visual Functions

void TransferRandomNumbers(int* arrayTo, int arrayToSize, int* arrayFrom, int arrayFromSize) { // for VJ
	chrono::steady_clock::time_point now;
	now = chrono::high_resolution_clock::now();
	long long seed = now.time_since_epoch().count();
	srand(seed % 0x100000000);
	for (int i = 0; i < arrayToSize; i++) {
		arrayTo[i] = arrayFrom[rand() * i % arrayFromSize];
	}
}

void TransmitArray(int* array, int size, ofstream* Out, string inFile, string outFile) {
	Out->open(outFile);
	*Out << "Size+Arr," << size;
	for (int i = 0; i < size; i++) {
		*Out << ',' << array[i];
	}
	*Out << '\n';
}

void BubbleSortVis(int* array, int size, ofstream* Out, string inFile, string outFile) {
	*Out << "Bubble Sort" << ",-,-,-,-,-,-,-,-,-,-,-" << '\n';
	bool swapped;
	for (int i = 0; i < size - 1; i++) {
		swapped = false;
		*Out << "iteration," << i << '\n';
		for (int j = 0; j < size - i - 1; j++) {
			*Out << "greater," << j << ',' << j + 1 << '\n';
			if (array[j] > array[j + 1]) {
				swap(array[j], array[j + 1]);
				*Out << "swapped," << j << ',' << j + 1 << '\n';
				swapped = true;
			}
		}
		if (swapped == false)
			//*Out << "no swaps" << '\n';
			break;
	}
	*Out << "complete" << '\n';
	// Credit for this bubble sort goes to GeeksForGeeks @ https://www.geeksforgeeks.org/bubble-sort/
}

void HeapSortVis(int* array, int size, ofstream* Out, string inFile, string outFile) {
	*Out << "Heap Sort" << ",-,-,-,-,-,-,-,-,-,-,-" << '\n';
}

void MergeSortVis(int* array, int size, ofstream* Out, string inFile, string outFile) {
	*Out << "Merge Sort" << ",-,-,-,-,-,-,-,-,-,-,-" << '\n';
}

void QuickSortVis(int* array, int size, ofstream* Out, string inFile, string outFile) {
	*Out << "Quick Sort" << ",-,-,-,-,-,-,-,-,-,-,-" << '\n';
}

void MultiBubbleSortVis(int* array, int size, int thread_count, ofstream* Out, string inFile, string outFile) {
	*Out << "2-Thread Bubble Sort" << ",-,-,-,-,-,-,-,-,-,-,-" << '\n';
	bool swapped;
	for (int i = 0; i < (size / 2) - 1; i++) {
		swapped = false;
		*Out << "iteration," << i << '\n';
		for (int j = 0; j < (size / 2) - i - 1; j++) {
			*Out << "greater," << j << ',' << j + 1 << '\n';
			*Out << "greater," << 32 + j << ',' << 32 + j + 1 << '\n';
			if (array[j] > array[j + 1]) {
				swap(array[j], array[j + 1]);
				*Out << "swapped," << j << ',' << j + 1 << '\n';
				swapped = true;
			}
			if (array[32 + j] > array[32 + j + 1]) {
				swap(array[32 + j], array[32 + j + 1]);
				*Out << "swapped," << 32 + j << ',' << 32 + j + 1 << '\n';
				swapped = true;
			}
		}
		if (swapped == false)
			//*Out << "no swaps" << '\n';
			break;
	}
	int* tempArray = new int[size];
	int index1 = 0;
	int index2 = 32;
	for (int i = 0; i < size; i++) {
		if (index1 == 32) {
			*Out << "move," << index2 << '\n';
			tempArray[i] = array[index2];
			index2++;
		}
		else if (index2 == 64) {
			*Out << "move," << index1 << '\n';
			tempArray[i] = array[index1];
			index1++;
		}
		else if (array[index1] < array[index2]) {
			*Out << "lesser between," << index1 << ',' << index2 << '\n';
			*Out << "pick," << index1 << '\n';
			tempArray[i] = array[index1];
			index1++;
		}
		else {
			*Out << "lesser between," << index1 << ',' << index2 << '\n';
			*Out << "pick," << index2 << '\n';
			tempArray[i] = array[index2];
			index2++;
		}
	}
	copyarray(tempArray, array, size);
	delete[] tempArray;
	*Out << "complete" << '\n';
	// Credit for this bubble sort goes to GeeksForGeeks @ https://www.geeksforgeeks.org/bubble-sort/
}