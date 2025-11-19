// ParallelSort.h
#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include "Student.h"
#include <pthread.h>

// Thread data structure
template<typename RollNumType, typename CourseIDType>
struct ThreadData {
    Student<RollNumType, CourseIDType>** array;
    int left;
    int right;
    int threadId;
    long long timeElapsed; // in microseconds
};

// Get current time in microseconds
inline long long getCurrentTimeMicros() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000LL + ts.tv_nsec / 1000;
}

// Merge function for merge sort
template<typename RollNumType, typename CourseIDType>
void merge(Student<RollNumType, CourseIDType>** arr, int left, int mid, int right) {
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
        if (isLessThan(L[i]->getRollNumber(), R[j]->getRollNumber())) {
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
void mergeSort(Student<RollNumType, CourseIDType>** arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Thread function for sorting
template<typename RollNumType, typename CourseIDType>
void* threadSort(void* arg) {
    ThreadData<RollNumType, CourseIDType>* data = 
        static_cast<ThreadData<RollNumType, CourseIDType>*>(arg);
    
    long long startTime = getCurrentTimeMicros();
    
    mergeSort(data->array, data->left, data->right);
    
    long long endTime = getCurrentTimeMicros();
    data->timeElapsed = endTime - startTime;
    
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
    
    void sort(Student<RollNumType, CourseIDType>** array, int size) {
        int chunkSize = size / numThreads;
        
        // Create threads for sorting
        for (int i = 0; i < numThreads; i++) {
            threadData[i].array = array;
            threadData[i].left = i * chunkSize;
            threadData[i].right = (i == numThreads - 1) ? (size - 1) : ((i + 1) * chunkSize - 1);
            threadData[i].threadId = i;
            threadData[i].timeElapsed = 0;
            
            pthread_create(&threads[i], nullptr, 
                          threadSort<RollNumType, CourseIDType>, 
                          &threadData[i]);
        }
        
        // Wait for all threads to complete
        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], nullptr);
        }
        
        // Merge the sorted chunks
        for (int i = 1; i < numThreads; i++) {
            int left = 0;
            int mid = i * chunkSize - 1;
            int right = (i == numThreads - 1) ? (size - 1) : ((i + 1) * chunkSize - 1);
            merge(array, left, mid, right);
        }
    }
    
    void printThreadTimes(void (*outputFunc)(const char*), void (*outputIntFunc)(int)) {
        for (int i = 0; i < numThreads; i++) {
            outputFunc("Thread ");
            outputIntFunc(threadData[i].threadId);
            outputFunc(" took ");
            outputIntFunc((int)(threadData[i].timeElapsed)); // Show in microseconds
            outputFunc(" microseconds (");
            outputIntFunc((int)(threadData[i].timeElapsed / 1000)); // Also show milliseconds
            outputFunc(" ms)\n");
        }
    }
};

#endif
