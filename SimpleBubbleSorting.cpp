#include <iostream>
#include <vector>
#include <utility> // Required for std::swap

// Function to perform Bubble Sort
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    // Outer loop for passes
    for (int i = 0; i < n - 1; ++i) {
        // Flag to optimize the sort: if no two elements are swapped
        // by the inner loop, then the array is already sorted.
        bool swapped = false;

        // Inner loop for comparisons and swaps
        // The largest element 'bubbles up' to its correct position
        // at the end of each pass, so we don't need to check the
        // last 'i' elements in the next pass.
        for (int j = 0; j < n - 1 - i; ++j) {
            // Compare adjacent elements
            if (arr[j] > arr[j + 1]) {
                // Swap them if they are in the wrong order
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped in inner loop, then break
        if (swapped == false)
            break;
    }
}

// Function to print the array
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}

// Main function to test the sorting
int main() {
    // Initialize an array (vector in C++)
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90};

    std::cout << "Original array: ";
    printArray(data);

    // Perform the sort
    bubbleSort(data);

    std::cout << "Sorted array: ";
    printArray(data);

    return 0;
}