# Student ERP System

A comprehensive C++ template-based Enterprise Resource Planning (ERP) system for managing student records across multiple universities (IIIT Delhi and IIT Delhi). This system demonstrates advanced C++ concepts including templates, multi-threading, iterators, and low-level system calls.

## Features

### Core Functionality
- **Generic Template Design**: Supports multiple data types for roll numbers (string/integer) and course IDs
- **Multi-threaded Parallel Sorting**: 4-thread parallel merge sort implementation with performance metrics
- **Iterator Pattern**: Multiple iteration strategies (insertion order, sorted by roll number, sorted by name)
- **Efficient Indexing**: Fast course-grade queries using hash-based indexing
- **CSV Operations**: Automatic generation and parsing of large student datasets (3000+ records)

### Supported Operations
1. **Generic Student Records** - Demonstrate template flexibility with different data types
2. **Cross-University Course Handling** - IIIT and IIT students with different course formats
3. **Parallel Sorting** - Sort 3000 students by roll number or name using 4 threads
4. **Iterator Views** - Display records in different orders (insertion, sorted by roll, sorted by name)
5. **Course Grade Queries** - Find high-performing students (grade ≥ 9) in specific courses

## System Requirements


### Compiler & Tools
- **g++** with C++17 support (version 7.0+)
- **GNU Assembler (as)** for syscall assembly
- **make** utility
- **pthread** library (usually pre-installed)


## Project Structure

```
.
├── main.cpp              # Main program with menu interface
├── Student.h             # Student and Course template classes
├── StudentDatabase.h     # Database management and indexing
├── CSVReader.h           # CSV parsing and generation
├── ParallelSort.h        # Multi-threaded merge sort implementation
├── Iterator.h            # Iterator pattern implementations
├── StringUtils.h/cpp     # String utility functions
├── basicIO.h/cpp         # Low-level I/O operations
├── syscall.s             # Assembly syscall wrappers
├── Makefile              # Build configuration
└── README.md             # This file
```

## Building the Project

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd student-erp-system

# Build the project
make

# Run the program
./student_erp
```

### Make Commands
```bash
make          # Build the project
make clean    # Remove object files and executable
make cleanall # Remove all generated files including CSV
make run      # Build and run the program

```

## Usage Guide

### Main Menu
When you run the program, you'll see:

```
=========================================
  Assignment 4: Templates and Threads
  University ERP System
=========================================

========================================
  University ERP System - Main Menu
========================================
1. Generic Student Records (Q1)
2. IIIT & IIT Course Handling (Q2)
3. Load and Parallel Sort 3000 Students (Q3)
4. Show Iterator Views (Q4)
5. Query by Course Grade (Q5)
6. Exit
========================================
Enter choice (1-6):
```

### Option 1: Generic Student Records
Demonstrates template flexibility with three examples:
- IIIT Delhi student (string roll, string course codes)
- IIT Delhi student (integer roll, integer course codes)
- Mixed types (string roll, integer course codes)

### Option 2: IIIT & IIT Course Handling
Shows how students from different universities can take courses from both institutions:
- IIIT courses: OOPD, DSA, OS, CN, DBMS, AI, ML, etc.
- IIT courses: 101, 202, 303, 401, 523, etc.

### Option 3: Parallel Sorting
1. Automatically generates `students.csv` with 3000 records
2. Loads all student data into memory
3. Choose sorting criteria:
   - Sort by Roll Number
   - Sort by Name (case-insensitive)
4. Displays thread execution times in microseconds/milliseconds

**Note**: CSV generation happens automatically - no manual file creation needed!

### Option 4: Iterator Views
Display student records in different orders:
- **Insertion Order**: As they were added to the database
- **Sorted by Roll Number**: Lexicographic/numeric order
- **Sorted by Name**: Alphabetical order (case-insensitive)

You can choose how many records to display (default: all).

### Option 5: Course Grade Queries
Find students with grade ≥ 9 in a specific course:

**Available Courses:**
- **IIIT**: OOPD, DSA, OS, CN, DBMS, AI, ML, NLP, CV, SEC, TOC, COA, ALGO, WEB, MOBILE, CLOUD, IOT, CYBER, GAME, ROBOTICS
- **IIT**: 101, 202, 303, 401, 523, 601, 702, 815, 920, 1005, 111, 222, 333, 444, 555, 666, 777, 888, 999, 1111

## Sample Data Format

### CSV Structure
```csv
Name,RollNumber,Branch,Year,CurrentCourse,Course1,Grade1,Course2,Grade2
Aarav Sharma,2021001,CS,2020,OOPD,401,6,NLP,7
Vivaan Sharma,MT21002,ECE,2021,202,DBMS,7,920,8
```

### Data Distribution (3000 records)
- **IIIT Students** (~2980): MT/PhD prefix + 2-digit year + 3-digit serial
- **IIT Students** (~20): Pure numeric roll numbers
- **Branches**: CSE, ECE, CSAM, CSD, CSB (IIIT) / CS, EE, ME, CE, CH (IIT)
- **Years**: 2020-2023
- **Grades**: 6-10 (for completed courses)

## Technical Implementation

### Template Specialization
```cpp
// Supports both string and numeric identifiers
Student<const char*, const char*> iiitStudent;  // String-based
Student<unsigned int, int> iitStudent;          // Integer-based
Student<const char*, int> mixedStudent;         // Mixed types
```

### Multi-threading
- 4 parallel threads divide dataset into chunks
- Each thread sorts its chunk independently using merge sort
- Main thread merges sorted chunks sequentially
- Thread execution times measured using `clock_gettime` syscall

### Case-Insensitive Sorting
Names are sorted alphabetically ignoring case:
- "Aarav" < "Aditya" < "Angel" < "Anjali"

## Performance Metrics

### Typical Performance (3000 records)
- CSV Generation: < 50ms
- CSV Loading: ~100-200ms
- Parallel Sort (4 threads): ~5-15ms per thread
- Query by Grade: < 1ms (with indexing)

### Thread Distribution
Each of 4 threads processes ~750 records, with timing displayed in appropriate units:
- Microseconds (< 1ms)
- Milliseconds (1ms - 1s)
- Seconds (> 1s)


##Run time Issues

**CSV Not Found**
- Don't worry! The program auto-generates it
- Use Option 3 first to create `students.csv`



