// ParallelSort.h
#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include "Student.h"
#include <pthread.h>

// System call definitions for clock_gettime (Linux x86-64 syscall number 228)
#define SYS_CLOCK_GETTIME 228
#define CLOCK_MONOTONIC 1

// Declare syscall3 external function from syscall.s
extern "C" long syscall3(long number, long arg1, long arg2, long arg3);

// Thread data structure
template<typename RollNumType, typename CourseIDType>
struct ThreadData {
    Student<RollNumType, CourseIDType>** array;
    int left;
    int right;
    int threadId;
    long long timeElapsed; // in microseconds
    bool (*compare)(const Student<RollNumType, CourseIDType>&, const Student<RollNumType, CourseIDType>&);
};

// Get current time in microseconds
inline long long getCurrentTimeMicros() {
    struct timespec {
        long tv_sec;
        long tv_nsec;
    };
    struct timespec ts;
    
    long result = syscall3(SYS_CLOCK_GETTIME, CLOCK_MONOTONIC, (long)&ts, 0); 
    
    if (result < 0) {
        return 0;
    }
    
    return (long long)ts.tv_sec * 1000000LL + ts.tv_nsec / 1000LL;
}

// Merge function for merge sort
template<typename RollNumType, typename CourseIDType>
void merge(Student<RollNumType, CourseIDType>** arr, int left, int mid, int right,
           bool (*compare)(const Student<RollNumType, CourseIDType>&, const Student<RollNumType, CourseIDType>&)) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    Student<RollNumType, CourseIDType>** L = new Student<RollNumType, CourseIDType>*[n1];
    Student<RollNumType, CourseIDType>** R = new Student<RollNumType, CourseIDType>*[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (compare(*L[i], *R[j])) { 
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

// Merge sort implementation
template<typename RollNumType, typename CourseIDType>
void mergeSort(Student<RollNumType, CourseIDType>** arr, int left, int right, 
               bool (*compare)(const Student<RollNumType, CourseIDType>&, const Student<RollNumType, CourseIDType>&)) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, compare);
        mergeSort(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

// Thread function for sorting
template<typename RollNumType, typename CourseIDType>
void* threadSort(void* arg) {
    ThreadData<RollNumType, CourseIDType>* data = 
        static_cast<ThreadData<RollNumType, CourseIDType>*>(arg);
    
    if (data->left <= data->right) {
        long long startTime = getCurrentTimeMicros();
        
        mergeSort(data->array, data->left, data->right, data->compare); 
        
        long long endTime = getCurrentTimeMicros();
        
        if (endTime >= startTime) {
            data->timeElapsed = endTime - startTime;
        } else {
            data->timeElapsed = 0;
        }
        
        if (data->timeElapsed == 0) {
            data->timeElapsed = 1; 
        }

    } else {
        data->timeElapsed = 0;
    }
    
    return nullptr;
}

// Parallel sort with multiple threads
template<typename RollNumType, typename CourseIDType>
class ParallelSort {
private:
    int numThreads;
    pthread_t* threads;
    ThreadData<RollNumType, CourseIDType>* threadData;

public:
    ParallelSort(int threads = 2) : numThreads(threads), threads(nullptr), threadData(nullptr) {
        this->threads = new pthread_t[numThreads];
        this->threadData = new ThreadData<RollNumType, CourseIDType>[numThreads];
    }
    
    ~ParallelSort() {
        if (threads) delete[] threads;
        if (threadData) delete[] threadData;
    }
    
    // FIXED: Corrected sort method with proper merging
    void sort(Student<RollNumType, CourseIDType>** array, int size,
              bool (*compare)(const Student<RollNumType, CourseIDType>&, const Student<RollNumType, CourseIDType>&)) {
        
        if (size <= 0) return;
        
        int chunkSize = size / numThreads;
        
        // Create threads for sorting
        for (int i = 0; i < numThreads; i++) {
            threadData[i].array = array;
            threadData[i].left = i * chunkSize;
            threadData[i].right = (i == numThreads - 1) ? (size - 1) : ((i + 1) * chunkSize - 1);
            
            if (threadData[i].left > threadData[i].right) {
                threadData[i].right = threadData[i].left - 1;
            }

            threadData[i].threadId = i;
            threadData[i].timeElapsed = 0;
            threadData[i].compare = compare;
            
            pthread_create(&threads[i], nullptr, 
                          threadSort<RollNumType, CourseIDType>, 
                          &threadData[i]);
        }
        
        // Wait for all threads to complete
        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], nullptr);
        }
        
        // FIXED: Correct sequential merging of sorted chunks
        // After threads complete, we have numThreads sorted subarrays
        // We need to merge them sequentially
        
        // Merge chunks one by one into the final sorted array
        for (int i = 1; i < numThreads; i++) {
            // Calculate boundaries for merging
            int leftStart = 0;
            int leftEnd = threadData[i - 1].right;
            int rightStart = threadData[i].left;
            int rightEnd = threadData[i].right;
            
            // Only merge if both chunks have elements
            if (leftEnd >= leftStart && rightEnd >= rightStart) {
                // Merge from leftStart to rightEnd, with mid at leftEnd
                merge(array, leftStart, leftEnd, rightEnd, compare);
            }
        }
    }
    
    // Improved time display logic
    void printThreadTimes(void (*outputFunc)(const char*), void (*outputIntFunc)(int)) {
        for (int i = 0; i < numThreads; i++) {
            long long microSeconds = threadData[i].timeElapsed;
            
            outputFunc("Thread ");
            outputIntFunc(threadData[i].threadId);
            outputFunc(" took ");
            
            if (microSeconds == 0) {
                outputFunc("0 microseconds (Empty Chunk)\n");
                continue;
            }
            
            // Display time in the most appropriate unit
            if (microSeconds < 1000) {
                // Less than 1ms - show only microseconds
                outputIntFunc((int)microSeconds);
                outputFunc(" microseconds\n");
            } else if (microSeconds < 1000000) {
                // Less than 1 second - show milliseconds with precision
                long long milliseconds = microSeconds / 1000;
                int remaining_us = (int)(microSeconds % 1000);
                
                outputIntFunc((int)milliseconds);
                outputFunc(".");
                
                if (remaining_us < 10) {
                    outputFunc("00");
                } else if (remaining_us < 100) {
                    outputFunc("0");
                }
                outputIntFunc(remaining_us);
                outputFunc(" ms\n");
            } else {
                // 1 second or more - show seconds with precision
                int seconds = (int)(microSeconds / 1000000);
                int remaining_ms = (int)((microSeconds % 1000000) / 1000);
                
                outputIntFunc(seconds);
                outputFunc(".");
                
                if (remaining_ms < 10) {
                    outputFunc("00");
                } else if (remaining_ms < 100) {
                    outputFunc("0");
                }
                outputIntFunc(remaining_ms);
                outputFunc(" s\n");
            }
        }
    }
};

#endif