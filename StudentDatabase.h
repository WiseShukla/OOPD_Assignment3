// StudentDatabase.h
#ifndef STUDENT_DATABASE_H
#define STUDENT_DATABASE_H

#include "Student.h"

// Helper function for comparing course IDs (from CSVReader.h)
template<typename CourseIDType>
inline bool courseIdsEqual(const CourseIDType& a, const CourseIDType& b);

// Simple hash table for quick student lookup by course and grade
template<typename RollNumType, typename CourseIDType>
class CourseGradeIndex {
private:
    struct IndexEntry {
        CourseIDType courseId;
        int minGrade;
        Student<RollNumType, CourseIDType>** students;
        int count;
        int capacity;
        
        IndexEntry() : students(nullptr), count(0), capacity(0) {}
        
        ~IndexEntry() {
            if (students) delete[] students;
        }
        
        void addStudent(Student<RollNumType, CourseIDType>* student) {
            if (count >= capacity) {
                int newCapacity = capacity == 0 ? 4 : capacity * 2;
                Student<RollNumType, CourseIDType>** newArray = 
                    new Student<RollNumType, CourseIDType>*[newCapacity];
                for (int i = 0; i < count; ++i) {
                    newArray[i] = students[i];
                }
                if (students) delete[] students;
                students = newArray;
                capacity = newCapacity;
            }
            students[count++] = student;
        }
    };
    
    IndexEntry* entries;
    int entriesCount;
    int entriesCapacity;

public:
    CourseGradeIndex() : entries(nullptr), entriesCount(0), entriesCapacity(0) {}
    
    ~CourseGradeIndex() {
        if (entries) delete[] entries;
    }
    
    void buildIndex(Student<RollNumType, CourseIDType>* students, int count) {
        // Clear existing index
        if (entries) delete[] entries;
        entries = nullptr;
        entriesCount = 0;
        entriesCapacity = 0;
        
        // Build new index
        for (int i = 0; i < count; ++i) {
            int completedCount = students[i].getCompletedCoursesCount();
            for (int j = 0; j < completedCount; ++j) {
                const auto& course = students[i].getCompletedCourse(j);
                int grade = course.getGrade();
                if (grade >= 9) { // Only index high-performing students
                    addToIndex(course.getCourseId(), 9, &students[i]);
                }
            }
        }
    }
    
    void addToIndex(const CourseIDType& courseId, int minGrade, 
                    Student<RollNumType, CourseIDType>* student) {
        // Find existing entry or create new one
        int entryIndex = -1;
        for (int i = 0; i < entriesCount; ++i) {
            // Use courseIdsEqual for proper comparison
            if (courseIdsEqual(entries[i].courseId, courseId) && entries[i].minGrade == minGrade) {
                entryIndex = i;
                break;
            }
        }
        
        if (entryIndex == -1) {
            // Create new entry
            if (entriesCount >= entriesCapacity) {
                int newCapacity = entriesCapacity == 0 ? 8 : entriesCapacity * 2;
                IndexEntry* newArray = new IndexEntry[newCapacity];
                // Manually copy old entries to avoid double-free on array destruction
                for (int i = 0; i < entriesCount; ++i) {
                    newArray[i].courseId = entries[i].courseId;
                    newArray[i].minGrade = entries[i].minGrade;
                    newArray[i].students = entries[i].students;
                    newArray[i].count = entries[i].count;
                    newArray[i].capacity = entries[i].capacity;
                    // Reset old entry so its destructor doesn't free the dynamically allocated student array
                    entries[i].students = nullptr;
                    entries[i].count = 0;
                    entries[i].capacity = 0;
                }
                if (entries) delete[] entries;
                entries = newArray;
                entriesCapacity = newCapacity;
            }
            entryIndex = entriesCount++;
            entries[entryIndex].courseId = courseId;
            entries[entryIndex].minGrade = minGrade;
        }
        
        entries[entryIndex].addStudent(student);
    }
    
    void query(const CourseIDType& courseId, int minGrade,
               Student<RollNumType, CourseIDType>**& results, int& resultCount) {
        results = nullptr;
        resultCount = 0;
        
        for (int i = 0; i < entriesCount; ++i) {
            // Use courseIdsEqual for proper comparison
            if (courseIdsEqual(entries[i].courseId, courseId) && entries[i].minGrade == minGrade) {
                results = entries[i].students;
                resultCount = entries[i].count;
                return;
            }
        }
    }
};

// Database class to manage students
template<typename RollNumType, typename CourseIDType>
class StudentDatabase {
private:
    Student<RollNumType, CourseIDType>* students;
    int count;
    int capacity;
    
    Student<RollNumType, CourseIDType>** insertionOrder;
    Student<RollNumType, CourseIDType>** sortedOrder;
    Student<RollNumType, CourseIDType>** sortedOrderByName; // NEW: for sorting by name
    
    CourseGradeIndex<RollNumType, CourseIDType> gradeIndex;

    void expand() {
        int newCapacity = capacity == 0 ? 100 : capacity * 2;
        Student<RollNumType, CourseIDType>* newArray = 
            new Student<RollNumType, CourseIDType>[newCapacity];
        
        for (int i = 0; i < count; ++i) {
            newArray[i] = students[i];
        }
        
        if (students) delete[] students;
        students = newArray;
        capacity = newCapacity;
    }

public:
    StudentDatabase() : students(nullptr), count(0), capacity(0),
                       insertionOrder(nullptr), sortedOrder(nullptr), sortedOrderByName(nullptr) {} // UPDATED
    
    ~StudentDatabase() {
        if (students) delete[] students;
        if (insertionOrder) delete[] insertionOrder;
        if (sortedOrder) delete[] sortedOrder;
        if (sortedOrderByName) delete[] sortedOrderByName; // UPDATED
    }
    
    void addStudent(const Student<RollNumType, CourseIDType>& student) {
        if (count >= capacity) {
            expand();
        }
        students[count] = student;
        students[count].setInsertionOrder(count);
        count++;
    }
    
    int getCount() const { return count; }
    
    Student<RollNumType, CourseIDType>* getStudents() { return students; }
    
    const Student<RollNumType, CourseIDType>& getStudent(int index) const {
        return students[index];
    }
    
    void prepareOrderViews() {
        if (insertionOrder) delete[] insertionOrder;
        if (sortedOrder) delete[] sortedOrder;
        if (sortedOrderByName) delete[] sortedOrderByName; // UPDATED
        
        insertionOrder = new Student<RollNumType, CourseIDType>*[count];
        sortedOrder = new Student<RollNumType, CourseIDType>*[count];
        sortedOrderByName = new Student<RollNumType, CourseIDType>*[count]; // UPDATED
        
        for (int i = 0; i < count; ++i) {
            insertionOrder[i] = &students[i];
            sortedOrder[i] = &students[i];
            sortedOrderByName[i] = &students[i]; // UPDATED
        }
    }
    
    Student<RollNumType, CourseIDType>** getInsertionOrder() {
        return insertionOrder;
    }
    
    Student<RollNumType, CourseIDType>** getSortedOrder() {
        return sortedOrder;
    }
    
    Student<RollNumType, CourseIDType>** getSortedOrderByName() {
        return sortedOrderByName;
    }

    // NEW: Comparison function for Roll Number
    static bool compareByRollNumber(const Student<RollNumType, CourseIDType>& a, 
                                    const Student<RollNumType, CourseIDType>& b) {
        // Delegate to Student class method
        return Student<RollNumType, CourseIDType>::compareByRollNumber(a, b);
    }
    
    // NEW: Comparison function for Name
    static bool compareByName(const Student<RollNumType, CourseIDType>& a, 
                              const Student<RollNumType, CourseIDType>& b) {
        // Delegate to Student class method
        return Student<RollNumType, CourseIDType>::compareByName(a, b);
    }
    
    void buildGradeIndex() {
        gradeIndex.buildIndex(students, count);
    }
    
    void queryByGrade(const CourseIDType& courseId, int minGrade,
                      Student<RollNumType, CourseIDType>**& results, int& resultCount) {
        gradeIndex.query(courseId, minGrade, results, resultCount);
    }
};

#endif