// main.cpp - Complete with Q1 and Q2 implementations (minimal output)
#include "basicIO.h"
#include "Student.h"
#include "StudentDatabase.h"
#include "CSVReader.h"
#include "ParallelSort.h"
#include "Iterator.h"

// Lambda functions for parsing
const char* parseRollNum(const char* str) { return str; }
const char* parseCourseId(const char* str) { return str; }

// ============================================================================
// Q1: Generic Student Records Demo
// ============================================================================
void handleQ1() {
    io.outputstring("\n========================================\n");
    io.outputstring(" Question 1: Generic Student Records\n");
    io.outputstring("========================================\n\n");
    
    io.outputstring("Demonstrating template flexibility with different data types:\n\n");
    
    // Example 1: IIIT Delhi student with string roll number and string course codes
    io.outputstring("--- Example 1: IIIT Delhi Student ---\n");
    io.outputstring("Type: Student<const char*, const char*>\n");
    Student<const char*, const char*> iiitStudent("Rajesh Kumar", "MT21045", "CSE", 2021);
    
    // Add current course (IIIT style - string acronym)
    iiitStudent.addCurrentCourse(Course<const char*>("OOPD", "Object Oriented Programming & Design", -1));
    
    // Add completed courses
    iiitStudent.addCompletedCourse(Course<const char*>("DSA", "Data Structures & Algorithms", 9));
    iiitStudent.addCompletedCourse(Course<const char*>("OS", "Operating Systems", 10));
    
    io.outputstring("Name: ");
    io.outputstring(iiitStudent.getName());
    io.outputstring("\nRoll: ");
    io.outputstring(iiitStudent.getRollNumber());
    io.outputstring(" (string type)");
    io.outputstring("\nBranch: ");
    io.outputstring(iiitStudent.getBranch());
    io.outputstring("\nYear: ");
    io.outputint(iiitStudent.getStartingYear());
    io.outputstring("\nCurrent Course: ");
    if (iiitStudent.getCurrentCoursesCount() > 0) {
        io.outputstring(iiitStudent.getCurrentCourse(0).getCourseId());
    }
    io.outputstring("\nCompleted Courses:\n");
    for (int i = 0; i < iiitStudent.getCompletedCoursesCount(); i++) {
        io.outputstring("  - ");
        io.outputstring(iiitStudent.getCompletedCourse(i).getCourseId());
        io.outputstring(" (Grade: ");
        io.outputint(iiitStudent.getCompletedCourse(i).getGrade());
        io.outputstring(")\n");
    }
    
    io.outputstring("\n--- Example 2: IIT Delhi Student ---\n");
    io.outputstring("Type: Student<unsigned int, int>\n");
    Student<unsigned int, int> iitStudent("Priya Sharma", 2021045, "CS", 2021);
    
    // Add current course (IIT style - integer code)
    iitStudent.addCurrentCourse(Course<int>(401, "Advanced Algorithms", -1));
    
    // Add completed courses (IIT style - integer codes)
    iitStudent.addCompletedCourse(Course<int>(101, "Introduction to Programming", 8));
    iitStudent.addCompletedCourse(Course<int>(202, "Data Structures", 9));
    
    io.outputstring("Name: ");
    io.outputstring(iitStudent.getName());
    io.outputstring("\nRoll: ");
    io.outputint((int)iitStudent.getRollNumber());
    io.outputstring(" (unsigned int type)");
    io.outputstring("\nBranch: ");
    io.outputstring(iitStudent.getBranch());
    io.outputstring("\nYear: ");
    io.outputint(iitStudent.getStartingYear());
    io.outputstring("\nCurrent Course: ");
    if (iitStudent.getCurrentCoursesCount() > 0) {
        io.outputint(iitStudent.getCurrentCourse(0).getCourseId());
    }
    io.outputstring("\nCompleted Courses:\n");
    for (int i = 0; i < iitStudent.getCompletedCoursesCount(); i++) {
        io.outputstring("  - ");
        io.outputint(iitStudent.getCompletedCourse(i).getCourseId());
        io.outputstring(" (Grade: ");
        io.outputint(iitStudent.getCompletedCourse(i).getGrade());
        io.outputstring(")\n");
    }
    
    io.outputstring("\n--- Example 3: Mixed Types ---\n");
    io.outputstring("Type: Student<const char*, int>\n");
    Student<const char*, int> mixedStudent("Amit Patel", "PhD22100", "EE", 2022);
    
    // String roll number with integer course codes
    mixedStudent.addCurrentCourse(Course<int>(555, "Machine Learning", -1));
    mixedStudent.addCompletedCourse(Course<int>(333, "Artificial Intelligence", 10));
    
    io.outputstring("Name: ");
    io.outputstring(mixedStudent.getName());
    io.outputstring("\nRoll: ");
    io.outputstring(mixedStudent.getRollNumber());
    io.outputstring(" (string)");
    io.outputstring("\nCourses: integers\n");
    io.outputstring("Current: ");
    io.outputint(mixedStudent.getCurrentCourse(0).getCourseId());
    io.outputstring("\n");
}

// Helper function to check if course already completed (background validation)
bool isCourseAlreadyCompleted(const Student<const char*, const char*>& student, const char* courseId) {
    for (int i = 0; i < student.getCompletedCoursesCount(); i++) {
        if (my_strcmp(student.getCompletedCourse(i).getCourseId(), courseId) == 0) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// Q2: IIIT & IIT Course Handling Demo
// ============================================================================
void handleQ2() {
    io.outputstring("\n========================================\n");
    io.outputstring(" Question 2: IIIT & IIT Course Handling\n");
    io.outputstring("========================================\n\n");
    
    io.outputstring("Demonstrating IIIT-Delhi students taking both:\n");
    io.outputstring("- IIIT courses (string codes like OOPD, DSA)\n");
    io.outputstring("- IIT courses (integer codes like 401, 523)\n\n");
    
    // SCENARIO 1: Valid - Student taking new IIIT course (OOPD) 
    io.outputstring("--- Scenario 1: Valid Case ---\n");
    io.outputstring("IIIT-Delhi Student\n");
    Student<const char*, const char*> validStudent("Sneha Reddy", "MT22089", "CSE", 2022);
    
    // Completed IIIT courses (string codes)
    validStudent.addCompletedCourse(Course<const char*>("DSA", "Data Structures", 9));
    validStudent.addCompletedCourse(Course<const char*>("DBMS", "Database Systems", 8));
    
    // Completed IIT courses - different from current course
    validStudent.addCompletedCourse(Course<const char*>("523", "IIT ML Course", 10));
    validStudent.addCompletedCourse(Course<const char*>("702", "IIT Networks Course", 9));
    
    // Check if OOPD is already completed (background validation)
    const char* currentCourse1 = "OOPD";
    if (!isCourseAlreadyCompleted(validStudent, currentCourse1)) {
        // Only add current course if not already completed
        validStudent.addCurrentCourse(Course<const char*>("OOPD", "Object Oriented Programming", -1));
    }
    
    io.outputstring("Name: ");
    io.outputstring(validStudent.getName());
    io.outputstring("\nRoll: ");
    io.outputstring(validStudent.getRollNumber());
    io.outputstring("\nBranch: ");
    io.outputstring(validStudent.getBranch());
    
    if (validStudent.getCurrentCoursesCount() > 0) {
        io.outputstring("\nCurrent Course: ");
        io.outputstring(validStudent.getCurrentCourse(0).getCourseId());
        io.outputstring(" (IIIT-Delhi)\n");
    }
    
    io.outputstring("\nCompleted Courses:\n");
    io.outputstring("  IIIT Courses:\n");
    for (int i = 0; i < 2 && i < validStudent.getCompletedCoursesCount(); i++) {
        io.outputstring("    - ");
        io.outputstring(validStudent.getCompletedCourse(i).getCourseId());
        io.outputstring(" (Grade: ");
        io.outputint(validStudent.getCompletedCourse(i).getGrade());
        io.outputstring(")\n");
    }
    io.outputstring("  IIT Courses:\n");
    for (int i = 2; i < validStudent.getCompletedCoursesCount(); i++) {
        io.outputstring("    - ");
        io.outputstring(validStudent.getCompletedCourse(i).getCourseId());
        io.outputstring(" (Grade: ");
        io.outputint(validStudent.getCompletedCourse(i).getGrade());
        io.outputstring(")\n");
    }
    
    // SCENARIO 2: Invalid case handled in background (not displayed)
    // Background validation ensures students don't retake completed courses
    // Example: A student who completed "401" cannot take "401" again
    // This validation happens silently without console output
    
    // SCENARIO 2: Another valid case - different IIT course
    io.outputstring("\n--- Scenario 2: Valid Case ---\n");
    io.outputstring("IIIT-Delhi Student\n");
    Student<const char*, const char*> validStudent2("Anjali Mehta", "PhD23050", "CSAM", 2023);
    
    // Completed different courses
    validStudent2.addCompletedCourse(Course<const char*>("ML", "Machine Learning", 10));
    validStudent2.addCompletedCourse(Course<const char*>("401", "IIT Algorithms", 9));
    
    // Check if 815 is already completed (background validation)
    const char* currentCourse2 = "815";
    if (!isCourseAlreadyCompleted(validStudent2, currentCourse2)) {
        // Only add current course if not already completed
        validStudent2.addCurrentCourse(Course<const char*>("815", "IIT Advanced Networks", -1));
    }
    
    io.outputstring("Name: ");
    io.outputstring(validStudent2.getName());
    io.outputstring("\nRoll: ");
    io.outputstring(validStudent2.getRollNumber());
    io.outputstring("\nBranch: ");
    io.outputstring(validStudent2.getBranch());
    
    if (validStudent2.getCurrentCoursesCount() > 0) {
        io.outputstring("\nCurrent Course: ");
        io.outputstring(validStudent2.getCurrentCourse(0).getCourseId());
        io.outputstring(" (IIT course)\n");
    }
    
    io.outputstring("\nCompleted Courses:\n");
    for (int i = 0; i < validStudent2.getCompletedCoursesCount(); i++) {
        io.outputstring("  - ");
        io.outputstring(validStudent2.getCompletedCourse(i).getCourseId());
        io.outputstring(" (Grade: ");
        io.outputint(validStudent2.getCompletedCourse(i).getGrade());
        io.outputstring(")\n");
    }
    
    io.outputstring("\n");
}

// ============================================================================
// Main Function
// ============================================================================
int main() {
    StudentDatabase<const char*, const char*> db;
    bool dataLoaded = false;
    
    while (true) {
        io.outputstring("\n=========================================\n");
        io.outputstring("  Assignment 4: Templates and Threads\n");
        io.outputstring("  University ERP System\n");
        io.outputstring("=========================================\n\n");
        
        io.outputstring("========================================\n");
        io.outputstring("  University ERP System - Main Menu\n");
        io.outputstring("========================================\n");
        io.outputstring("1. Generic Student Records (Q1)\n");
        io.outputstring("2. IIIT & IIT Course Handling (Q2)\n");
        io.outputstring("3. Load and Parallel Sort 3000 Students (Q3)\n");
        io.outputstring("4. Show Iterator Views (Q4)\n");
        io.outputstring("5. Query by Course Grade (Q5)\n");
        io.outputstring("6. Exit\n");
        io.outputstring("========================================\n");
        io.outputstring("Enter choice (1-6): ");
        
        int choice = io.inputint();
        
        switch (choice) {
            case 1:
                handleQ1();
                break;
                
            case 2:
                handleQ2();
                break;
                
            case 3: {
                io.outputstring("\n========================================\n");
                io.outputstring(" Question 3: Parallel Sorting\n");
                io.outputstring("========================================\n\n");
                
                io.outputstring("Generating CSV with 3000 records...\n");
                CSVReader::generateSampleCSV("students.csv", 3000);
                io.outputstring("CSV generated.\n");
                
                io.outputstring("Loading students...\n");
                bool loaded = CSVReader::loadFromCSV("students.csv", db, parseRollNum, parseCourseId);
                
                if (loaded) {
                    io.outputstring("Loaded ");
                    io.outputint(db.getCount());
                    io.outputstring(" students\n");
                    dataLoaded = true;
                    
                    db.prepareOrderViews();
                    
                    while (true) {
                        io.outputstring("\nSelect sort criteria:\n");
                        io.outputstring("1. Sort by Roll Number\n");
                        io.outputstring("2. Sort by Name\n");
                        io.outputstring("3. Return to Main Menu\n");
                        io.outputstring("Enter choice (1-3): ");
                        
                        int sortChoice = io.inputint();
                        
                        if (sortChoice == 3) break;
                        
                        ParallelSort<const char*, const char*> sorter(4);
                        
                        if (sortChoice == 1) {
                            io.outputstring("\nSorting by Roll Number with 4 threads...\n");
                            sorter.sort(db.getSortedOrder(), db.getCount(), 
                                      StudentDatabase<const char*, const char*>::compareByRollNumber);
                        } else if (sortChoice == 2) {
                            io.outputstring("\nSorting by Name with 4 threads...\n");
                            sorter.sort(db.getSortedOrderByName(), db.getCount(), 
                                      StudentDatabase<const char*, const char*>::compareByName);
                        } else {
                            io.outputstring("Invalid choice!\n");
                            continue;
                        }
                        
                        io.outputstring("Sorting completed.\n\n");
                        io.outputstring("Thread times:\n");
                        sorter.printThreadTimes(
                            [](const char* s) { io.outputstring(s); },
                            [](int n) { io.outputint(n); }
                        );
                        io.outputstring("\n");
                    }
                } else {
                    io.outputstring("Failed to load CSV file!\n");
                }
                break;
            }
            
            case 4: {
                if (!dataLoaded) {
                    io.outputstring("\nPlease load data first (Option 3)!\n");
                    break;
                }
                
                io.outputstring("\n========================================\n");
                io.outputstring(" Question 4: Iterator Views\n");
                io.outputstring("========================================\n\n");
                
                io.outputstring("Total: ");
                io.outputint(db.getCount());
                io.outputstring(" students\n\n");
                
                while (true) {
                    io.outputstring("--- Iterator View Menu ---\n");
                    io.outputstring("1. Insertion Order\n");
                    io.outputstring("2. Sorted by Roll Number\n");
                    io.outputstring("3. Sorted by Name\n");
                    io.outputstring("4. Return to Main Menu\n");
                    io.outputstring("Enter choice (1-4): ");
                    
                    int viewChoice = io.inputint();
                    
                    if (viewChoice == 4) break;
                    
                    io.outputstring("\nHow many records to display? (Enter 0 for all) ");
                    int displayCount = io.inputint();
                    
                    if (displayCount == 0 || displayCount > db.getCount()) {
                        displayCount = db.getCount();
                    }
                    
                    Iterator<const char*, const char*>* iterator = nullptr;
                    
                    if (viewChoice == 1) {
                        io.outputstring("\n--- Records in Insertion Order ---\n");
                        iterator = new InsertionOrderIterator<const char*, const char*>(
                            db.getInsertionOrder(), db.getCount());
                    } else if (viewChoice == 2) {
                        io.outputstring("\n--- Records Sorted by Roll Number ---\n");
                        iterator = new SortedOrderIterator<const char*, const char*>(
                            db.getSortedOrder(), db.getCount());
                    } else if (viewChoice == 3) {
                        io.outputstring("\n--- Records Sorted by Name ---\n");
                        iterator = new SortedOrderIterator<const char*, const char*>(
                            db.getSortedOrderByName(), db.getCount());
                    } else {
                        io.outputstring("Invalid choice!\n");
                        continue;
                    }
                    
                    int count = 0;
                    while (iterator->hasNext() && count < displayCount) {
                        Student<const char*, const char*>* student = iterator->next();
                        if (student) {
                            io.outputstring("  Name         : ");
                            io.outputstring(student->getName());
                            io.outputstring("\n  Roll Number  : ");
                            io.outputstring(student->getRollNumber());
                            io.outputstring("\n  Branch       : ");
                            io.outputstring(student->getBranch());
                            io.outputstring("\n  Start Year   : ");
                            io.outputint(student->getStartingYear());
                            io.outputstring("\n");
                            
                            if (student->getCurrentCoursesCount() > 0) {
                                io.outputstring("  Current Course:\n");
                                const Course<const char*>& current = student->getCurrentCourse(0);
                                io.outputstring("    Code       : ");
                                io.outputstring(current.getCourseId());
                                io.outputstring("\n");
                            }
                            
                            int completed = student->getCompletedCoursesCount();
                            if (completed > 0) {
                                io.outputstring("  Previous Courses:\n");
                                for (int i = 0; i < completed; i++) {
                                    const Course<const char*>& course = student->getCompletedCourse(i);
                                    io.outputstring("    Code       : ");
                                    io.outputstring(course.getCourseId());
                                    io.outputstring(", Grade: ");
                                    io.outputint(course.getGrade());
                                    io.outputstring("\n");
                                }
                            }
                            
                            count++;
                        }
                    }
                    
                    delete iterator;
                    io.outputstring("\n");
                }
                break;
            }
            
            case 5: {
                if (!dataLoaded) {
                    io.outputstring("\nPlease load data first (Option 3)!\n");
                    break;
                }
                
                io.outputstring("\n========================================\n");
                io.outputstring(" Question 5: Query by Course Grade\n");
                io.outputstring("========================================\n\n");
                
                io.outputstring("Building grade index...\n");
                db.buildGradeIndex();
                io.outputstring("Index built.\n\n");
                
                io.outputstring("Available Courses:\n\n");
                
                io.outputstring("IIIT Delhi Courses (String Format):\n");
                io.outputstring("  OOPD, DSA, OS, CN, DBMS, AI, ML, NLP, CV, SEC\n");
                io.outputstring("  TOC, COA, ALGO, WEB, MOBILE, CLOUD, IOT, CYBER, GAME, ROBOTICS\n\n");
                
                io.outputstring("IIT Delhi Courses (Integer Format):\n");
                io.outputstring("  101, 202, 303, 401, 523, 601, 702, 815, 920, 1005\n");
                io.outputstring("  111, 222, 333, 444, 555, 666, 777, 888, 999, 1111\n\n");
                
                io.outputstring("Enter course code: ");
                const char* courseInput = io.inputstring();
                
                io.outputstring("\nSearching for students with grade >= 9 in '");
                io.outputstring(courseInput);
                io.outputstring("'...\n\n");
                
                Student<const char*, const char*>** results = nullptr;
                int resultCount = 0;
                
                db.queryByGrade(courseInput, 9, results, resultCount);
                
                if (resultCount == 0) {
                    io.outputstring("No students found with grade >= 9 in this course.\n");
                } else {
                    io.outputstring("Found ");
                    io.outputint(resultCount);
                    io.outputstring(" student(s):\n\n");
                    
                    FilteredIterator<const char*, const char*> it(results, resultCount);
                    
                    int displayCount = 0;
                    while (it.hasNext() && displayCount < 20) {
                        Student<const char*, const char*>* student = it.next();
                        if (student) {
                            io.outputstring("  Name: ");
                            io.outputstring(student->getName());
                            io.outputstring(", Roll: ");
                            io.outputstring(student->getRollNumber());
                            io.outputstring(", Branch: ");
                            io.outputstring(student->getBranch());
                            
                            int completed = student->getCompletedCoursesCount();
                            for (int i = 0; i < completed; i++) {
                                const Course<const char*>& course = student->getCompletedCourse(i);
                                if (my_strcmp(course.getCourseId(), courseInput) == 0) {
                                    io.outputstring(", Grade: ");
                                    io.outputint(course.getGrade());
                                    break;
                                }
                            }
                            io.outputstring("\n");
                            displayCount++;
                        }
                    }
                    
                    if (resultCount > 20) {
                        io.outputstring("\n... and ");
                        io.outputint(resultCount - 20);
                        io.outputstring(" more students\n");
                    }
                }
                
                io.outputstring("\n");
                break;
            }
            
            case 6:
                io.outputstring("\nExiting program. Goodbye!\n");
                return 0;
                
            default:
                io.outputstring("\nInvalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
