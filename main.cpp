// main.cpp
#include "basicIO.h"
#include "Student.h"
#include "StudentDatabase.h"
#include "ParallelSort.h"
#include "Iterator.h"
#include "CSVReader.h"
#include "StringUtils.h"

// Parse functions for different types
unsigned int parseUnsignedInt(const char* str) {
    return (unsigned int)my_atoi(str);
}

const char* parseString(const char* str) {
    static char buffer[256];
    int i = 0;
    while (str[i] && i < 255) {
        buffer[i] = str[i];
        i++;
    }
    buffer[i] = '\0';
    return buffer;
}

// Display student information
template<typename RollNumType, typename CourseIDType>
void displayStudent(const Student<RollNumType, CourseIDType>& student);

// Specialization for unsigned int roll number and const char* course
template<>
void displayStudent(const Student<unsigned int, const char*>& student) {
    io.outputstring("Name: ");
    io.outputstring(student.getName());
    io.outputstring(", Roll: ");
    io.outputint(student.getRollNumber());
    io.outputstring(", Branch: ");
    io.outputstring(student.getBranch());
    io.outputstring(", Year: ");
    io.outputint(student.getStartingYear());
    
    if (student.getCompletedCoursesCount() > 0) {
        io.outputstring(", Completed: [");
        for (int i = 0; i < student.getCompletedCoursesCount(); i++) {
            const auto& course = student.getCompletedCourse(i);
            io.outputstring(course.getCourseId());
            io.outputstring("(");
            io.outputint(course.getGrade());
            io.outputstring(")");
            if (i < student.getCompletedCoursesCount() - 1) {
                io.outputstring(", ");
            }
        }
        io.outputstring("]");
    }
    io.terminate();
}

// Specialization for unsigned int roll number and int course
template<>
void displayStudent(const Student<unsigned int, int>& student) {
    io.outputstring("Name: ");
    io.outputstring(student.getName());
    io.outputstring(", Roll: ");
    io.outputint(student.getRollNumber());
    io.outputstring(", Branch: ");
    io.outputstring(student.getBranch());
    io.outputstring(", Year: ");
    io.outputint(student.getStartingYear());
    
    if (student.getCompletedCoursesCount() > 0) {
        io.outputstring(", Completed: [");
        for (int i = 0; i < student.getCompletedCoursesCount(); i++) {
            const auto& course = student.getCompletedCourse(i);
            io.outputint(course.getCourseId());
            io.outputstring("(");
            io.outputint(course.getGrade());
            io.outputstring(")");
            if (i < student.getCompletedCoursesCount() - 1) {
                io.outputstring(", ");
            }
        }
        io.outputstring("]");
    }
    io.terminate();
}

void printMenu() {
    io.outputstring("\n========================================\n");
    io.outputstring("  University ERP System - Main Menu\n");
    io.outputstring("========================================\n");
    io.outputstring("1. Template Class Examples (Q1 & Q2)\n");
    io.outputstring("2. Load and Sort 3000 Students (Q3)\n");
    io.outputstring("3. Show Iterator Views (Q4)\n");
    io.outputstring("4. Query by Course Grade (Q5)\n");
    io.outputstring("5. Run All Operations\n");
    io.outputstring("6. Exit\n");
    io.outputstring("========================================\n");
    io.outputstring("Enter choice (1-6): ");
}

// Global database for menu operations
StudentDatabase<unsigned int, const char*> globalDB;
bool dataLoaded = false;

void option1_TemplateExamples() {
    io.outputstring("\n=== Questions 1 & 2: Template Class ===\n");
    io.terminate();
    
    io.outputstring("\n--- IIIT-Delhi (String course codes) ---\n");
    Student<unsigned int, const char*> s1("Adarsh Kumar", 2020101, "CSE", 2020);
    s1.addCompletedCourse(Course<const char*>("OOPD", "OOP", 9));
    s1.addCompletedCourse(Course<const char*>("DSA", "Data Structures", 10));
    displayStudent(s1);
    
    Student<unsigned int, const char*> s2("Priya Sharma", 2020102, "ECE", 2020);
    s2.addCompletedCourse(Course<const char*>("EM", "EM Theory", 8));
    displayStudent(s2);
    
    io.outputstring("\n--- IIT-Delhi (Integer course codes) ---\n");
    Student<unsigned int, int> s3("Rahul Verma", 2020201, "CSE", 2020);
    s3.addCompletedCourse(Course<int>(101, "Programming", 9));
    s3.addCompletedCourse(Course<int>(102, "Digital Logic", 10));
    displayStudent(s3);
    
    Student<unsigned int, int> s4("Sneha Patel", 2020202, "ME", 2020);
    s4.addCompletedCourse(Course<int>(201, "Mechanics", 7));
    displayStudent(s4);
    
    io.outputstring("\nTemplate flexibility verified!\n");
}

void option2_LoadAndSort() {
    io.outputstring("\n=== Question 3: Parallel Sorting ===\n");
    io.terminate();
    
    if (!dataLoaded) {
        io.outputstring("Generating CSV with 3000 records...\n");
        CSVReader::generateSampleCSV("students.csv", 3000);
        io.outputstring("CSV generated.\n");
        
        io.outputstring("Loading students...\n");
        CSVReader::loadFromCSV("students.csv", globalDB, parseUnsignedInt, parseString);
        io.outputstring("Loaded ");
        io.outputint(globalDB.getCount());
        io.outputstring(" students\n");
        
        dataLoaded = true;
    } else {
        io.outputstring("Data already loaded: ");
        io.outputint(globalDB.getCount());
        io.outputstring(" students\n");
    }
    
    globalDB.prepareOrderViews();
    
    io.outputstring("\nSorting with 4 threads...\n");
    ParallelSort<unsigned int, const char*> sorter(4);
    sorter.sort(globalDB.getSortedOrder(), globalDB.getCount());
    io.outputstring("Sorting completed.\n");
    
    io.outputstring("\nThread times:\n");
    sorter.printThreadTimes(
        [](const char* s) { io.outputstring(s); },
        [](int i) { io.outputint(i); }
    );
}

void option3_ShowIterators() {
    io.outputstring("\n=== Question 4: Iterator Views ===\n");
    io.terminate();
    
    if (!dataLoaded) {
        io.outputstring("Error: Load data first (Option 2)\n");
        return;
    }
    
    io.outputstring("Total: ");
    io.outputint(globalDB.getCount());
    io.outputstring(" students\n");
    
    io.outputstring("\n--- First 5 in Insertion Order ---\n");
    InsertionOrderIterator<unsigned int, const char*> insertIter(
        globalDB.getInsertionOrder(), globalDB.getCount()
    );
    
    int cnt = 0;
    while (insertIter.hasNext() && cnt < 5) {
        displayStudent(*insertIter.next());
        cnt++;
    }
    
    io.outputstring("\n--- First 5 in Sorted Order ---\n");
    SortedOrderIterator<unsigned int, const char*> sortedIter(
        globalDB.getSortedOrder(), globalDB.getCount()
    );
    
    cnt = 0;
    while (sortedIter.hasNext() && cnt < 5) {
        displayStudent(*sortedIter.next());
        cnt++;
    }
    
    io.outputstring("\nTwo different views shown.\n");
}

void option4_QueryByGrade() {
    io.outputstring("\n=== Question 5: Course Grade Query ===\n");
    io.terminate();
    
    if (!dataLoaded) {
        io.outputstring("Error: Load data first (Option 2)\n");
        return;
    }
    
    io.outputstring("Building index...\n");
    globalDB.buildGradeIndex();
    io.outputstring("Index built.\n");
    
    io.outputstring("\n--- Students with grade >= 9 in OOPD ---\n");
    Student<unsigned int, const char*>** results = nullptr;
    int resultCount = 0;
    
    globalDB.queryByGrade("OOPD", 9, results, resultCount);
    
    io.outputstring("Found ");
    io.outputint(resultCount);
    io.outputstring(" students");
    
    if (resultCount > 0) {
        io.outputstring(" (showing first 10):\n");
        FilteredIterator<unsigned int, const char*> filterIter(results, resultCount);
        int cnt = 0;
        while (filterIter.hasNext() && cnt < 10) {
            displayStudent(*filterIter.next());
            cnt++;
        }
        
        if (resultCount > 10) {
            io.outputstring("... and ");
            io.outputint(resultCount - 10);
            io.outputstring(" more\n");
        }
    } else {
        io.outputstring(".\n");
    }
    
    io.outputstring("\nQuery used O(1) hash index.\n");
}

void option5_RunAll() {
    io.outputstring("\n=== Running All Operations ===\n");
    option1_TemplateExamples();
    option2_LoadAndSort();
    option3_ShowIterators();
    option4_QueryByGrade();
    io.outputstring("\n=== All operations completed ===\n");
}

int main() {
    io.activateInput();
    
    io.outputstring("=========================================\n");
    io.outputstring("  Assignment 4: Templates and Threads\n");
    io.outputstring("  University ERP System\n");
    io.outputstring("=========================================\n");
    
    bool running = true;
    while (running) {
        printMenu();
        int choice = io.inputint();
        
        switch (choice) {
            case 1:
                option1_TemplateExamples();
                break;
            case 2:
                option2_LoadAndSort();
                break;
            case 3:
                option3_ShowIterators();
                break;
            case 4:
                option4_QueryByGrade();
                break;
            case 5:
                option5_RunAll();
                break;
            case 6:
                io.outputstring("\nExiting. Thank you!\n");
                running = false;
                break;
            default:
                io.outputstring("\nInvalid choice. Enter 1-6.\n");
                break;
        }
    }
    
    return 0;
}
