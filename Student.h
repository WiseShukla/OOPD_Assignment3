// Student.h
#ifndef STUDENT_H
#define STUDENT_H

#include "StringUtils.h"
#include <utility>

// Template class for Course with flexible course identifier type
template<typename CourseID>
class Course {
private:
    CourseID courseId;
    char* courseName;
    int grade; // -1 means not graded yet, 0-10 for actual grades

public:
    Course() : courseName(nullptr), grade(-1) {}
    
    Course(const CourseID& id, const char* name, int gr = -1) 
        : courseId(id), courseName(nullptr), grade(gr) {
        copyString(courseName, name);
    }
    
    Course(const Course& other) : courseId(other.courseId), courseName(nullptr), grade(other.grade) {
        copyString(courseName, other.courseName);
    }
    
    Course& operator=(const Course& other) {
        if (this != &other) {
            courseId = other.courseId;
            grade = other.grade;
            copyString(courseName, other.courseName);
        }
        return *this;
    }
    
    ~Course() {
        if (courseName) delete[] courseName;
    }
    
    const CourseID& getCourseId() const { return courseId; }
    const char* getCourseName() const { return courseName; }
    int getGrade() const { return grade; }
    void setGrade(int gr) { grade = gr; }
};

// Specialization for Course<const char*> to properly handle string copying
template<>
class Course<const char*> {
private:
    char* courseId;  // Own the string
    char* courseName;
    int grade;

public:
    Course() : courseId(nullptr), courseName(nullptr), grade(-1) {}
    
    Course(const char* id, const char* name, int gr = -1) 
        : courseId(nullptr), courseName(nullptr), grade(gr) {
        copyString(courseId, id);
        copyString(courseName, name);
    }
    
    Course(const Course& other) : courseId(nullptr), courseName(nullptr), grade(other.grade) {
        copyString(courseId, other.courseId);
        copyString(courseName, other.courseName);
    }
    
    Course& operator=(const Course& other) {
        if (this != &other) {
            grade = other.grade;
            copyString(courseId, other.courseId);
            copyString(courseName, other.courseName);
        }
        return *this;
    }
    
    ~Course() {
        if (courseId) delete[] courseId;
        if (courseName) delete[] courseName;
    }
    
    const char* getCourseId() const { return courseId; }
    const char* getCourseName() const { return courseName; }
    int getGrade() const { return grade; }
    void setGrade(int gr) { grade = gr; }
};

// Main Student template class
template<typename RollNumType, typename CourseIDType>
class Student {
private:
    char* name;
    RollNumType rollNumber;
    char* branch;
    int startingYear;
    
    // Dynamic arrays for courses
    Course<CourseIDType>* currentCourses;
    int currentCoursesCount;
    int currentCoursesCapacity;
    
    Course<CourseIDType>* completedCourses;
    int completedCoursesCount;
    int completedCoursesCapacity;
    
    int insertionOrder; // For tracking original order

    void expandCurrentCourses() {
        int newCapacity = currentCoursesCapacity == 0 ? 4 : currentCoursesCapacity * 2;
        Course<CourseIDType>* newArray = new Course<CourseIDType>[newCapacity];
        for (int i = 0; i < currentCoursesCount; ++i) {
            newArray[i] = currentCourses[i];
        }
        if (currentCourses) delete[] currentCourses;
        currentCourses = newArray;
        currentCoursesCapacity = newCapacity;
    }
    
    void expandCompletedCourses() {
        int newCapacity = completedCoursesCapacity == 0 ? 4 : completedCoursesCapacity * 2;
        Course<CourseIDType>* newArray = new Course<CourseIDType>[newCapacity];
        for (int i = 0; i < completedCoursesCount; ++i) {
            newArray[i] = completedCourses[i];
        }
        if (completedCourses) delete[] completedCourses;
        completedCourses = newArray;
        completedCoursesCapacity = newCapacity;
    }

public:
    Student() : name(nullptr), branch(nullptr), startingYear(0),
                currentCourses(nullptr), currentCoursesCount(0), currentCoursesCapacity(0),
                completedCourses(nullptr), completedCoursesCount(0), completedCoursesCapacity(0),
                insertionOrder(0) {}
    
    Student(const char* n, const RollNumType& roll, const char* br, int year)
        : name(nullptr), rollNumber(roll), branch(nullptr), startingYear(year),
          currentCourses(nullptr), currentCoursesCount(0), currentCoursesCapacity(0),
          completedCourses(nullptr), completedCoursesCount(0), completedCoursesCapacity(0),
          insertionOrder(0) {
        copyString(name, n);
        copyString(branch, br);
    }
    
    // Copy constructor
    Student(const Student& other)
        : name(nullptr), rollNumber(other.rollNumber), branch(nullptr), 
          startingYear(other.startingYear),
          currentCoursesCount(other.currentCoursesCount),
          currentCoursesCapacity(other.currentCoursesCapacity),
          completedCoursesCount(other.completedCoursesCount),
          completedCoursesCapacity(other.completedCoursesCapacity),
          insertionOrder(other.insertionOrder) {
        
        copyString(name, other.name);
        copyString(branch, other.branch);
        
        if (other.currentCoursesCapacity > 0) {
            currentCourses = new Course<CourseIDType>[currentCoursesCapacity];
            for (int i = 0; i < currentCoursesCount; ++i) {
                currentCourses[i] = other.currentCourses[i];
            }
        } else {
            currentCourses = nullptr;
        }
        
        if (other.completedCoursesCapacity > 0) {
            completedCourses = new Course<CourseIDType>[completedCoursesCapacity];
            for (int i = 0; i < completedCoursesCount; ++i) {
                completedCourses[i] = other.completedCourses[i];
            }
        } else {
            completedCourses = nullptr;
        }
    }
    
    // Assignment operator
    Student& operator=(const Student& other) {
        if (this != &other) {
            // Clean up existing data
            if (name) delete[] name;
            if (branch) delete[] branch;
            if (currentCourses) delete[] currentCourses;
            if (completedCourses) delete[] completedCourses;
            
            // Copy data
            rollNumber = other.rollNumber;
            startingYear = other.startingYear;
            insertionOrder = other.insertionOrder;
            
            copyString(name, other.name);
            copyString(branch, other.branch);
            
            currentCoursesCount = other.currentCoursesCount;
            currentCoursesCapacity = other.currentCoursesCapacity;
            completedCoursesCount = other.completedCoursesCount;
            completedCoursesCapacity = other.completedCoursesCapacity;
            
            if (other.currentCoursesCapacity > 0) {
                currentCourses = new Course<CourseIDType>[currentCoursesCapacity];
                for (int i = 0; i < currentCoursesCount; ++i) {
                    currentCourses[i] = other.currentCourses[i];
                }
            } else {
                currentCourses = nullptr;
            }
            
            if (other.completedCoursesCapacity > 0) {
                completedCourses = new Course<CourseIDType>[completedCoursesCapacity];
                for (int i = 0; i < completedCoursesCount; ++i) {
                    completedCourses[i] = other.completedCourses[i];
                }
            } else {
                completedCourses = nullptr;
            }
        }
        return *this;
    }
    
    ~Student() {
        if (name) delete[] name;
        if (branch) delete[] branch;
        if (currentCourses) delete[] currentCourses;
        if (completedCourses) delete[] completedCourses;
    }
    
    // Getters
    const char* getName() const { return name; }
    const RollNumType& getRollNumber() const { return rollNumber; }
    const char* getBranch() const { return branch; }
    int getStartingYear() const { return startingYear; }
    int getInsertionOrder() const { return insertionOrder; }
    void setInsertionOrder(int order) { insertionOrder = order; }
    
    // Course management
    void addCurrentCourse(const Course<CourseIDType>& course) {
        if (currentCoursesCount >= currentCoursesCapacity) {
            expandCurrentCourses();
        }
        currentCourses[currentCoursesCount++] = course;
    }
    
    void addCompletedCourse(const Course<CourseIDType>& course) {
        if (completedCoursesCount >= completedCoursesCapacity) {
            expandCompletedCourses();
        }
        completedCourses[completedCoursesCount++] = course;
    }
    
    int getCurrentCoursesCount() const { return currentCoursesCount; }
    int getCompletedCoursesCount() const { return completedCoursesCount; }
    
    const Course<CourseIDType>& getCurrentCourse(int index) const {
        return currentCourses[index];
    }
    
    const Course<CourseIDType>& getCompletedCourse(int index) const {
        return completedCourses[index];
    }
    
    // Check if student has grade >= threshold in a specific course
    bool hasGradeInCourse(const CourseIDType& courseId, int minGrade) const {
        for (int i = 0; i < completedCoursesCount; ++i) {
            if (completedCourses[i].getCourseId() == courseId && 
                completedCourses[i].getGrade() >= minGrade) {
                return true;
            }
        }
        return false;
    }
};

// Comparison helper for different types
template<typename T>
bool isLessThan(const T& a, const T& b);

// Specialization for const char*
template<>
inline bool isLessThan<const char*>(const char* const& a, const char* const& b) {
    return my_strcmp(a, b) < 0;
}

// Specialization for unsigned int
template<>
inline bool isLessThan<unsigned int>(const unsigned int& a, const unsigned int& b) {
    return a < b;
}

// Specialization for int
template<>
inline bool isLessThan<int>(const int& a, const int& b) {
    return a < b;
}

#endif