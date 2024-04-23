#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

namespace P3 {
	const int size = 10000;
	const int tests = 16;

	int* unsorted = new int[size];
	int* toSort = new int[size];

	int* correct = new int[size];
}

namespace P3vis {
	const int size = 64;

	int* unsorted = new int[size];
	int* toSort = new int[size];

	int* correct = new int[size];
}

// FUNCTIONS

void randomLists(int* array1, int* array2, int size); // 15-bit
void randomLists30(int* array1, int* array2, int size); // 30-bit
void randomLists32(int* array1, int* array2, int size); // 32-bit
void copyArray(int* array1, int* array2, int size); // 2 = 1
void writeCSV(ofstream* out, int* array, string list, double time, bool values); // csv out
void repetitiveStuff(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end, string name, int i, vector<double>* durations); // general reset
void testIfValid(int* array1, int* array2, int size, bool printIfTrue);

// HELPERS

void combineTwoArrays(int* array1, int* array2, int size1, int size2);
void combineArrays(int* array, int size, int threadCount);
void heapify(int* array, int size, int index);
void merge(int* array, int left, int mid, int right);
int partitionArray(int* array, int begin, int end);

// MAIN SORTING

void bubbleSort(int* array, int size);
void STDSort(int* array, int size);
void heapSort(int* array, int size);
void mergeSort(int* array, int begin, int end);
void quickSort(int* array, int begin, int end);

// MULTI-MAIN SORTING 
// Accepts int values 2->size

void multiBubbleSort(int* array, int size, int threadCount);
void multiSTDSort(int* array, int size, int threadCount);
void multiHeapSort(int* array, int size, int threadCount);
void mulitMergeSort(int* array, int size, int threadCount);
void multiQuickSort(int* array, int size, int threadCount);

int main() {

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

	ofstream out;

	out.open("output.csv", ios_base::binary);
	if (!out.is_open()) { std::cout << "FAILED OPENING" << '\n'; }

	// Prep Output
	out << "Algorithm" << ',' << ',' << "Average" << ',' << "Speedup" << ',' << ',' << "Values: " << P3::size << ',' << ',' << "Tests: " << P3::tests << '\n' << endl;

	// Select which algorithms to test

	bool bubble = false;
	bool STD = true; // do not turn off due to "correct" value
	bool heap = true;
	bool merge = true;
	bool quick = true;

	bool bubbleMulti = false;
	bool STDMulti = true;
	bool heapMulti = true;
	bool mergeMulti = true;
	bool quickMulti = true;

	// Main Algorithm-Running

	int threadCount = 8;

	for (int i = 0; i < P3::tests; i++) {

		randomLists(P3::unsorted, P3::toSort, P3::size);

		//writeCSV(&out, P3::unsorted, "Unsorted " + to_string(i), 0, true);

		cout << "Sorting List " << i << '\n';

		// Test each sort if its boolean is true

		if (bubble) {
			name = "Bubble Sort"; // name

			if (first)  // push name into list of algs
				names.push_back(name);

			start = chrono::high_resolution_clock::now(); // start time
			bubbleSort(P3::toSort, P3::size); // run algorithm
			end = chrono::high_resolution_clock::now(); // end time

			//writeCSV(&out, P3::toSort, name, 0, true); // write out the info, array if true

			repetitiveStuff(start, end, name, i, &durations); // reset for next function
		}

		if (STD) {
			name = "std::sort";

			if (first)
				names.push_back(name);

			start = chrono::high_resolution_clock::now();
			STDSort(P3::toSort, P3::size);
			end = chrono::high_resolution_clock::now();

			copyArray(P3::toSort, P3::correct, P3::size); // to have reference to test validity of new algs

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (heap) {

			name = "Heap Sort";

			if (first)
				names.push_back(name);

			start = chrono::high_resolution_clock::now();
			heapSort(P3::toSort, P3::size);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (merge) {

			name = "Merge Sort";

			if (first) 
				names.push_back(name);

			start = chrono::high_resolution_clock::now();
			mergeSort(P3::toSort, 0, P3::size - 1);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (quick) {

			name = "Quick Sort";

			if (first)
				names.push_back(name);

			start = chrono::high_resolution_clock::now();
			quickSort(P3::toSort, 0, P3::size - 1);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		// Multi-Threaded because it is fun

		if (bubbleMulti) {

			name = "-Thread Bubble Sort";
			threadCount = 32;

			if (first)
				names.push_back(to_string(threadCount) + name);

			name = to_string(threadCount) + name;

			start = chrono::high_resolution_clock::now();
			multiBubbleSort(P3::toSort, P3::size, threadCount);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (STDMulti) {

			name = "-Thread std::sort";
			threadCount = 8;

			if (first) 
				names.push_back(to_string(threadCount) + name);

			name = to_string(threadCount) + name;

			start = chrono::high_resolution_clock::now();

			multiSTDSort(P3::toSort, P3::size, threadCount);
			end = chrono::high_resolution_clock::now();
			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (heapMulti) {

			name = "-Thread Heap Sort";
			threadCount = 8;

			if (first)
				names.push_back(to_string(threadCount) + name);
			name = to_string(threadCount) + name;

			start = chrono::high_resolution_clock::now();
			multiHeapSort(P3::toSort, P3::size, threadCount);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (mergeMulti) {

			name = "-Thread Merge Sort";
			threadCount = 2;

			if (first)
				names.push_back(to_string(threadCount) + name);

			name = to_string(threadCount) + name;

			start = chrono::high_resolution_clock::now();
			mulitMergeSort(P3::toSort, P3::size, threadCount);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		if (quickMulti) {

			name = "-Thread Quick Sort";
			threadCount = 16;

			if (first) 
				names.push_back(to_string(threadCount) + name);

			name = to_string(threadCount) + name;

			start = chrono::high_resolution_clock::now();
			multiQuickSort(P3::toSort, P3::size, threadCount);
			end = chrono::high_resolution_clock::now();

			testIfValid(P3::toSort, P3::correct, P3::size, false);

			repetitiveStuff(start, end, name, i, &durations);
		}

		// Enable if wanting an extra space
		cout << '\n';
		//out << '\n';

		first = false;


		// Writing Out Final Data

		int algorithms = (int)names.size();
		double total = 0;
		double averageTime;
		string baseName = "std::sort"; // reference algorithm for speedup calc
		double baseTime = 1; // reference time for speedup

		for (int i = 0; i < algorithms; i++) {

			out << names[i] << ',' << ',';

			for (int j = i; j < (durations.size()); j += algorithms) 
				total += durations[j];
		
			averageTime = total / P3::tests;

			if (names[i] == baseName) 
				baseTime = averageTime;
			
			out << averageTime << ',' << baseTime / averageTime << ',' << ',';

			for (int j = i; j < (durations.size()); j += algorithms) 
				out << durations[j] << ',';
		

			out << '\n';
			total = 0;
		}

		out.close();
	}

	// Wrap it up

	delete[] P3::unsorted;
	delete[] P3::toSort;
	delete[] P3::correct;

	return 0;
}

// FUNCTIONS

void randomLists(int* array1, int* array2, int size) {
	// Generate a random array of 15 bit numbers

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

void randomLists30(int* array1, int* array2, int size) {
	// Generate a random array of 30 bit numbers

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

void randomLists32(int* array1, int* array2, int size) {
	// Generates negative and positive 32-bit

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

void copyArray(int* array1, int* array2, int size) {
	// Copy array1 into array2

	for (int i = 0; i < size; i++)
		array2[i] = array1[i];
}

void writeCSV(ofstream* out, int* array, string list, double time, bool values) {
	// Write out a CSV of the sorting results
	
	*out << list << ',' << ',';

	if (time) 
		*out << time;
	
	*out << ',' << ',';

	if (values) {
		for (int i = 0; i < P3::size; i++) 
			*out << array[i] << ',';
	}

	*out << '\n';
}

void repetitiveStuff(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end, string name, int i, vector<double>* durations) {
	// Reset for the next iteration

	chrono::duration<long long, nano> time = end - start;
	double duration = (double)time.count() / 1000000;

	cout << name << " " << i << " took " << duration << " milliseconds." << '\n';

	durations->push_back(duration);
	copyArray(P3::unsorted, P3::toSort, P3::size);
}

void testIfValid(int* array1, int* array2, int size, bool printIfTrue) {
	// Confirm that the algorithms are sorting correctly

	for (int i = 0; i < size; i++) {
		if (array1[i] != array2[i]) {  // The arrays are not equal
			cout << "Array1 != Array2 @ " << i << '\n';
			return;
		}
	}

	if (printIfTrue) {
		cout << "Array1 == Array2" << '\n';
	}
}

// HELPERS
// yes i found a slightly cleaner way to do it in MultiBubbleSortVis, but i dont wanna break existing code for 0.1% gains in performance

void combineTwoArrays(int* array1, int* array2, int size1, int size2) {
	// Two adjacent arrays being sorted into one bigger array

	bool array1Done = false;
	bool array2Done = false;

	int array1Index = 0;
	int array2Index = 0;

	int leftOff;

	int* outputTemp = new int[size1 + size2];

	for (int i = 0; i < size1 + size2 - 1; i++) {

		if (array1[array1Index] <= array2[array2Index]) {

			outputTemp[i] = array1[array1Index];
			array1Index++;

			if (array1Index == size1) {
				array1Done = true;
				leftOff = i + 1;
				break;
			}
		}

		else {
			outputTemp[i] = array2[array2Index];
			array2Index++;

			if (array2Index == size2) {
				array2Done = true;
				leftOff = i + 1;
				break;
			}
		}
	}

	if (array1Done) {

		for (int i = leftOff; i < size1 + size2; i++) {
			outputTemp[i] = array2[array2Index];
			array2Index++;
		}
	}

	else {

		for (int i = leftOff; i < size1 + size2; i++) {
			outputTemp[i] = array1[array1Index];
			array1Index++;
		}
	}

	int output_index = 0;

	for (int i = 0; i < size1; i++) {
		array1[i] = outputTemp[output_index];
		output_index++;
	}

	for (int i = 0; i < size2; i++) {
		array2[i] = outputTemp[output_index];
		output_index++;
	}

	delete[] outputTemp;
}

void combineArrays(int* array, int size, int threadCount) {
	// Combine arrays in during the multithreaded sorts

	vector<thread> threads;

	int normalSize = size / threadCount;
	int remainder = size % threadCount;
	int last = normalSize + remainder;
	int arrayCount = threadCount;
	int currentThreads = threadCount / 2;

	bool odd = arrayCount % 2;
	bool oneMoreMerge = false;

	while (currentThreads) {

		for (int i = 0; i < (currentThreads - 1); i++) // make ends merge better
			threads.push_back(thread(combineTwoArrays, array + (2 * i * normalSize), array + ((2 * i + 1) * normalSize), normalSize, normalSize));
		
		odd = arrayCount % 2;

		if (odd) {
			threads.push_back(thread(combineTwoArrays, array + ((2 * (currentThreads - 1)) * normalSize), array + ((2 * (currentThreads - 1) + 1) * normalSize), normalSize, normalSize));
			arrayCount = arrayCount / 2 + 1;

			if (arrayCount == 3) 
				oneMoreMerge = true;
		}

		else { // if even
			threads.push_back(thread(combineTwoArrays, array + ((2 * (currentThreads - 1)) * normalSize), array + ((2 * (currentThreads - 1) + 1) * normalSize), normalSize, last));
			arrayCount /= 2;
			last += normalSize;
		}

		normalSize *= 2;

		for (int i = 0; i < currentThreads; i++) 
			threads[i].join();
		
		if (odd && currentThreads % 2) 
			currentThreads = currentThreads / 2 + 1;
		
		else 
			currentThreads /= 2;
		
		threads.clear();
	}

	if (oneMoreMerge && 0) {
		threads.push_back(thread(combineTwoArrays, array, array + normalSize, normalSize, last));
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
	// Merge two subarrays

	int subArrayOne = mid - left + 1;
	int subArrayTwo = right - mid;

	// Create temp arrays
	int* leftArray = new int[subArrayOne];
	int* rightArray = new int[subArrayTwo];

	// Copy data to leftArray and rightArray

	for (int i = 0; i < subArrayOne; i++)
		leftArray[i] = array[left + i];

	for (int j = 0; j < subArrayTwo; j++)
		rightArray[j] = array[mid + 1 + j];

	// Inital indices

	int indexOfSubArrayOne = 0;
	int indexOfSubArrayTwo = 0;
	int indexOfMergedArray = left;

	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {  // Merge the temp arrays

		if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
			array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}

		else {
			array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}

		indexOfMergedArray++;
	}

	// Copy the remaining elements of the sub arrays

	while (indexOfSubArrayOne < subArrayOne) {
		array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}

	while (indexOfSubArrayTwo < subArrayTwo) {
		array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}

	// Reference: @ https://www.geeksforgeeks.org/merge-sort/
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

// MAIN SORTING

void bubbleSort(int* array, int size) { 
	// Sort an array with bubblesort

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

void STDSort(int* array, int size) {
	// Sort an array with std::sort

	sort(array, array + size);
}

void heapSort(int* array, int size) {
	// Sort an array with heapsort

	for (int i = (size / 2) - 1; i >= 0; i--)  // Build heap in place
		heapify(array, size, i);

	for (int i = size - 1; i >= 0; i--) {
		swap(array[0], array[i]);

		heapify(array, i, 0);
	}
}

void mergeSort(int* array, int begin, int end) {
	// Sort an array with mergesort

	if (begin >= end)  // Base case of recursion
		return;

	int mid = begin + (end - begin) / 2;

	// Sort the left and right portions

	mergeSort(array, begin, mid);
	mergeSort(array, mid + 1, end);

	merge(array, begin, mid, end);  // Merge the two halves back together
}

void quickSort(int* array, int begin, int end) {
	// Sort an array with quicksort

	if (begin >= end)  // Base case of recursion
		return;

	int pivotIndex = partitionArray(array, begin, end);  // Partition the array

	quickSort(array, begin, pivotIndex - 1);  // Sort left of pivot
	quickSort(array, pivotIndex + 1, end);  // Sort right of pivot
}

// MULTI-MAIN SORTING

void multiBubbleSort(int* array, int size, int threadCount) {
	// Multithreaded Bubble Sort

	vector<thread> threads;

	int maxThreads = threadCount;
	int normalSize = size / maxThreads;

	for (int i = 0; i < (maxThreads - 1); i++) 
		threads.push_back(thread(bubbleSort, array + (i * normalSize), size / maxThreads));
	
	threads.push_back(thread(bubbleSort, array + ((maxThreads - 1) * normalSize), normalSize + (size % maxThreads)));

	for (int i = 0; i < maxThreads; i++) 
		threads[i].join();
	
	threads.clear();

	combineArrays(array, size, threadCount);
}

void multiSTDSort(int* array, int size, int threadCount) { 
	// Multithreaded std::sort

	vector<thread> threads;

	int maxThreads = threadCount;
	int normalSize = size / maxThreads;

	for (int i = 0; i < (maxThreads - 1); i++) 
		threads.push_back(thread(STDSort, array + (i * normalSize), size / maxThreads));
	
	threads.push_back(thread(STDSort, array + ((maxThreads - 1) * normalSize), normalSize + (size % maxThreads)));

	for (int i = 0; i < maxThreads; i++) 
		threads[i].join();
	
	threads.clear();

	combineArrays(array, size, threadCount);
}

void multiHeapSort(int* array, int size, int threadCount) { 
	// Multithreaded Heap Sort

	vector<thread> threads;

	int maxThreads = threadCount;
	int normalSize = size / maxThreads;

	for (int i = 0; i < (maxThreads - 1); i++) 
		threads.push_back(thread(heapSort, array + (i * normalSize), size / maxThreads));
	
	threads.push_back(thread(heapSort, array + ((maxThreads - 1) * normalSize), normalSize + (size % maxThreads)));

	for (int i = 0; i < maxThreads; i++) 
		threads[i].join();
	
	threads.clear();

	combineArrays(array, size, threadCount);
}

void mulitMergeSort(int* array, int size, int threadCount) { 
	// Multithreaded Merge Sort

	vector<thread> threads;

	int maxThreads = threadCount;
	int normalSize = size / maxThreads;

	for (int i = 0; i < (maxThreads - 1); i++) 
		threads.push_back(thread(mergeSort, array, (i * normalSize), (i * normalSize) + normalSize - 1));
	
	if (size % maxThreads == 0) 
		threads.push_back(thread(mergeSort, array, ((maxThreads - 1) * normalSize), ((maxThreads - 1) * normalSize) + normalSize - 1));
	
	else 
		threads.push_back(thread(mergeSort, array, ((maxThreads - 1) * normalSize), ((maxThreads - 1) * normalSize) + (size % maxThreads) - 1));
	
	for (int i = 0; i < maxThreads; i++) 
		threads[i].join();
	
	threads.clear();

	combineArrays(array, size, threadCount);
}

void multiQuickSort(int* array, int size, int threadCount) { 
	// Multithreaded Quick Sort

	vector<thread> threads;

	int maxThreads = threadCount;
	int normalSize = size / maxThreads;

	for (int i = 0; i < (maxThreads - 1); i++) 
		threads.push_back(thread(quickSort, array, (i * normalSize), (i * normalSize) + normalSize - 1));
	
	if (size % maxThreads == 0) 
		threads.push_back(thread(quickSort, array, ((maxThreads - 1) * normalSize), ((maxThreads - 1) * normalSize) + normalSize - 1));
	
	else 
		threads.push_back(thread(quickSort, array, ((maxThreads - 1) * normalSize), ((maxThreads - 1) * normalSize) + (size % maxThreads) - 1));
	
	for (int i = 0; i < maxThreads; i++) 
		threads[i].join();
	
	threads.clear();

	combineArrays(array, size, threadCount);
}