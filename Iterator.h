// Iterator.h
#ifndef ITERATOR_H
#define ITERATOR_H

#include "Student.h"

// Base iterator interface
template<typename RollNumType, typename CourseIDType>
class Iterator {
public:
    virtual ~Iterator() {}
    virtual bool hasNext() const = 0;
    virtual Student<RollNumType, CourseIDType>* next() = 0;
    virtual void reset() = 0;
};

// Insertion order iterator
template<typename RollNumType, typename CourseIDType>
class InsertionOrderIterator : public Iterator<RollNumType, CourseIDType> {
private:
    Student<RollNumType, CourseIDType>** students;
    int size;
    int currentIndex;

public:
    InsertionOrderIterator(Student<RollNumType, CourseIDType>** stds, int sz)
        : students(stds), size(sz), currentIndex(0) {}
    
    bool hasNext() const override {
        return currentIndex < size;
    }
    
    Student<RollNumType, CourseIDType>* next() override {
        if (hasNext()) {
            return students[currentIndex++];
        }
        return nullptr;
    }
    
    void reset() override {
        currentIndex = 0;
    }
};

// Sorted order iterator
template<typename RollNumType, typename CourseIDType>
class SortedOrderIterator : public Iterator<RollNumType, CourseIDType> {
private:
    Student<RollNumType, CourseIDType>** students;
    int size;
    int currentIndex;

public:
    SortedOrderIterator(Student<RollNumType, CourseIDType>** stds, int sz)
        : students(stds), size(sz), currentIndex(0) {}
    
    bool hasNext() const override {
        return currentIndex < size;
    }
    
    Student<RollNumType, CourseIDType>* next() override {
        if (hasNext()) {
            return students[currentIndex++];
        }
        return nullptr;
    }
    
    void reset() override {
        currentIndex = 0;
    }
};

// Filtered iterator for course grade queries
template<typename RollNumType, typename CourseIDType>
class FilteredIterator : public Iterator<RollNumType, CourseIDType> {
private:
    Student<RollNumType, CourseIDType>** students;
    int size;
    int currentIndex;

public:
    FilteredIterator(Student<RollNumType, CourseIDType>** stds, int sz)
        : students(stds), size(sz), currentIndex(0) {}
    
    bool hasNext() const override {
        return currentIndex < size;
    }
    
    Student<RollNumType, CourseIDType>* next() override {
        if (hasNext()) {
            return students[currentIndex++];
        }
        return nullptr;
    }
    
    void reset() override {
        currentIndex = 0;
    }
};

#endif
