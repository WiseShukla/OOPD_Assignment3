// Student.h 
#ifndef STUDENT_H
#define STUDENT_H

#include "StringUtils.h"

// Course template
template<typename CourseIDType>
class Course {
private:
    CourseIDType courseId;
    char* courseName;
    int grade;

public:
    Course() : courseId(), courseName(nullptr), grade(-1) {}
    
    Course(CourseIDType id, const char* name, int gr) 
        : courseId(id), courseName(nullptr), grade(gr) {
        if (name) {
            courseName = new char[my_strlen(name) + 1];
            my_strcpy(courseName, name);
        }
    }
    
    Course(const Course& other) : courseId(other.courseId), courseName(nullptr), grade(other.grade) {
        if (other.courseName) {
            courseName = new char[my_strlen(other.courseName) + 1];
            my_strcpy(courseName, other.courseName);
        }
    }
    
    Course& operator=(const Course& other) {
        if (this != &other) {
            if (courseName) delete[] courseName;
            courseId = other.courseId;
            grade = other.grade;
            if (other.courseName) {
                courseName = new char[my_strlen(other.courseName) + 1];
                my_strcpy(courseName, other.courseName);
            } else {
                courseName = nullptr;
            }
        }
        return *this;
    }
    
    ~Course() {
        if (courseName) delete[] courseName;
    }
    
    CourseIDType getCourseId() const { return courseId; }
    const char* getCourseName() const { return courseName; }
    int getGrade() const { return grade; }
};

// Student template
template<typename RollNumType, typename CourseIDType>
class Student {
private:
    char* name;
    RollNumType rollNumber;
    char* branch;
    int startingYear;
    int insertionOrder;
    
    Course<CourseIDType>* currentCourses;
    int currentCoursesCount;
    int currentCoursesCapacity;
    
    Course<CourseIDType>* completedCourses;
    int completedCoursesCount;
    int completedCoursesCapacity;

public:
    Student() : name(nullptr), rollNumber(), branch(nullptr), startingYear(0), insertionOrder(0),
                currentCourses(nullptr), currentCoursesCount(0), currentCoursesCapacity(0),
                completedCourses(nullptr), completedCoursesCount(0), completedCoursesCapacity(0) {}
    
    Student(const char* n, RollNumType roll, const char* br, int year)
        : name(nullptr), rollNumber(roll), branch(nullptr), startingYear(year), insertionOrder(0),
          currentCourses(nullptr), currentCoursesCount(0), currentCoursesCapacity(0),
          completedCourses(nullptr), completedCoursesCount(0), completedCoursesCapacity(0) {
        if (n) {
            name = new char[my_strlen(n) + 1];
            my_strcpy(name, n);
        }
        if (br) {
            branch = new char[my_strlen(br) + 1];
            my_strcpy(branch, br);
        }
    }
    
    Student(const Student& other)
        : name(nullptr), rollNumber(other.rollNumber), branch(nullptr), 
          startingYear(other.startingYear), insertionOrder(other.insertionOrder),
          currentCourses(nullptr), currentCoursesCount(0), currentCoursesCapacity(0),
          completedCourses(nullptr), completedCoursesCount(0), completedCoursesCapacity(0) {
        if (other.name) {
            name = new char[my_strlen(other.name) + 1];
            my_strcpy(name, other.name);
        }
        if (other.branch) {
            branch = new char[my_strlen(other.branch) + 1];
            my_strcpy(branch, other.branch);
        }
        
        if (other.currentCoursesCount > 0) {
            currentCoursesCapacity = other.currentCoursesCount;
            currentCourses = new Course<CourseIDType>[currentCoursesCapacity];
            for (int i = 0; i < other.currentCoursesCount; i++) {
                currentCourses[i] = other.currentCourses[i];
            }
            currentCoursesCount = other.currentCoursesCount;
        }
        
        if (other.completedCoursesCount > 0) {
            completedCoursesCapacity = other.completedCoursesCount;
            completedCourses = new Course<CourseIDType>[completedCoursesCapacity];
            for (int i = 0; i < other.completedCoursesCount; i++) {
                completedCourses[i] = other.completedCourses[i];
            }
            completedCoursesCount = other.completedCoursesCount;
        }
    }
    
    Student& operator=(const Student& other) {
        if (this != &other) {
            if (name) delete[] name;
            if (branch) delete[] branch;
            if (currentCourses) delete[] currentCourses;
            if (completedCourses) delete[] completedCourses;
            
            rollNumber = other.rollNumber;
            startingYear = other.startingYear;
            insertionOrder = other.insertionOrder;
            
            if (other.name) {
                name = new char[my_strlen(other.name) + 1];
                my_strcpy(name, other.name);
            } else {
                name = nullptr;
            }
            
            if (other.branch) {
                branch = new char[my_strlen(other.branch) + 1];
                my_strcpy(branch, other.branch);
            } else {
                branch = nullptr;
            }
            
            currentCoursesCount = 0;
            currentCoursesCapacity = 0;
            currentCourses = nullptr;
            if (other.currentCoursesCount > 0) {
                currentCoursesCapacity = other.currentCoursesCount;
                currentCourses = new Course<CourseIDType>[currentCoursesCapacity];
                for (int i = 0; i < other.currentCoursesCount; i++) {
                    currentCourses[i] = other.currentCourses[i];
                }
                currentCoursesCount = other.currentCoursesCount;
            }
            
            completedCoursesCount = 0;
            completedCoursesCapacity = 0;
            completedCourses = nullptr;
            if (other.completedCoursesCount > 0) {
                completedCoursesCapacity = other.completedCoursesCount;
                completedCourses = new Course<CourseIDType>[completedCoursesCapacity];
                for (int i = 0; i < other.completedCoursesCount; i++) {
                    completedCourses[i] = other.completedCourses[i];
                }
                completedCoursesCount = other.completedCoursesCount;
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
    RollNumType getRollNumber() const { return rollNumber; }
    const char* getBranch() const { return branch; }
    int getStartingYear() const { return startingYear; }
    int getInsertionOrder() const { return insertionOrder; }
    
    void setInsertionOrder(int order) { insertionOrder = order; }
    
    // Course management
    void addCurrentCourse(const Course<CourseIDType>& course) {
        if (currentCoursesCount >= currentCoursesCapacity) {
            int newCapacity = currentCoursesCapacity == 0 ? 4 : currentCoursesCapacity * 2;
            Course<CourseIDType>* newArray = new Course<CourseIDType>[newCapacity];
            for (int i = 0; i < currentCoursesCount; i++) {
                newArray[i] = currentCourses[i];
            }
            if (currentCourses) delete[] currentCourses;
            currentCourses = newArray;
            currentCoursesCapacity = newCapacity;
        }
        currentCourses[currentCoursesCount++] = course;
    }
    
    void addCompletedCourse(const Course<CourseIDType>& course) {
        if (completedCoursesCount >= completedCoursesCapacity) {
            int newCapacity = completedCoursesCapacity == 0 ? 4 : completedCoursesCapacity * 2;
            Course<CourseIDType>* newArray = new Course<CourseIDType>[newCapacity];
            for (int i = 0; i < completedCoursesCount; i++) {
                newArray[i] = completedCourses[i];
            }
            if (completedCourses) delete[] completedCourses;
            completedCourses = newArray;
            completedCoursesCapacity = newCapacity;
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
    
    // FIXED: Comparison functions
    static bool compareByRollNumber(const Student& a, const Student& b) {
        return compareRollNumbers(a.rollNumber, b.rollNumber);
    }
    
    static bool compareByName(const Student& a, const Student& b) {
        // Compare names lexicographically (MUST BE CASE-INSENSITIVE)
        if (a.name && b.name) {
            
            int cmp = my_stricmp(a.name, b.name); 
            
            if (cmp != 0) {
                return cmp < 0;
            }
            // If names are equal, compare by roll number
            return compareRollNumbers(a.rollNumber, b.rollNumber);
        }
        return a.name != nullptr;
    }
private:
    // Helper function to compare roll numbers (handles both string and int types)
    template<typename T>
    static bool compareRollNumbers(const T& a, const T& b) {
        return a < b;  // Default: use operator<
    }
    
    // Specialization for const char* (string comparison)
    static bool compareRollNumbers(const char* const& a, const char* const& b) {
        return my_strcmp(a, b) < 0;
    }
};

#endif