// CSVReader.h
#ifndef CSV_READER_H
#define CSV_READER_H

#include "StringUtils.h"
#include "Student.h"
#include "StudentDatabase.h"

// Forward declarations
extern "C" long syscall3(long number, long arg1, long arg2, long arg3);
extern "C" long syscall4(long number, long arg1, long arg2, long arg3, long arg4);

// Helper function for comparing course IDs (handles different types)
template<typename CourseIDType>
inline bool courseIdsEqual(const CourseIDType& a, const CourseIDType& b) {
    return a == b;  // Default: use operator==
}

// Specialization for const char*
template<>
inline bool courseIdsEqual<const char*>(const char* const& a, const char* const& b) {
    return my_strcmp(a, b) == 0;  // Compare string contents
}

// Simple CSV parser
class CSVReader {
private:
    static void parseCSVLine(const char* line, char** fields, int maxFields, int& fieldCount) {
        fieldCount = 0;
        int lineLen = my_strlen(line);
        int fieldStart = 0;
        bool inQuotes = false;
        
        for (int i = 0; i <= lineLen && fieldCount < maxFields; i++) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;
            } else if ((line[i] == ',' && !inQuotes) || line[i] == '\0') {
                // Extract field
                int fieldLen = i - fieldStart;
                fields[fieldCount] = new char[fieldLen + 1];
                
                int destIdx = 0;
                for (int j = fieldStart; j < i; j++) {
                    if (line[j] != '"') {
                        fields[fieldCount][destIdx++] = line[j];
                    }
                }
                fields[fieldCount][destIdx] = '\0';
                
                fieldCount++;
                fieldStart = i + 1;
            }
        }
    }
    
    static void freeFields(char** fields, int count) {
        for (int i = 0; i < count; i++) {
            if (fields[i]) delete[] fields[i];
        }
    }

public:
    // Load students from CSV file
    template<typename RollNumType, typename CourseIDType>
    static bool loadFromCSV(const char* filename, 
                           StudentDatabase<RollNumType, CourseIDType>& db,
                           RollNumType (*parseRollNum)(const char*),
                           CourseIDType (*parseCourseId)(const char*)) {
        // Open file (syscall 2)
        long fd = syscall3(2, (long)filename, 0, 0); // O_RDONLY = 0
        if (fd < 0) {
            return false;
        }
        
        // Read file content
        const int BUFFER_SIZE = 65536;
        char* buffer = new char[BUFFER_SIZE];
        char* fileContent = new char[1];
        fileContent[0] = '\0';
        int totalSize = 0;
        
        long bytesRead;
        while ((bytesRead = syscall3(0, fd, (long)buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytesRead] = '\0';
            
            char* newContent = new char[totalSize + bytesRead + 1];
            if (totalSize > 0) {
                my_strcpy(newContent, fileContent);
            }
            my_strcpy(newContent + totalSize, buffer);
            totalSize += bytesRead;
            
            delete[] fileContent;
            fileContent = newContent;
        }
        
        // Close file (syscall 3)
        syscall3(3, fd, 0, 0);
        delete[] buffer;
        
        // Parse CSV content
        int lineStart = 0;
        bool firstLine = true;
        
        for (int i = 0; i <= totalSize; i++) {
            if (fileContent[i] == '\n' || fileContent[i] == '\0') {
                if (i > lineStart) {
                    // Extract line
                    int lineLen = i - lineStart;
                    char* line = new char[lineLen + 1];
                    for (int j = 0; j < lineLen; j++) {
                        line[j] = fileContent[lineStart + j];
                    }
                    line[lineLen] = '\0';
                    
                    // Skip header line
                    if (firstLine) {
                        firstLine = false;
                    } else {
                        // Parse line
                        const int MAX_FIELDS = 20;
                        char* fields[MAX_FIELDS];
                        int fieldCount;
                        
                        parseCSVLine(line, fields, MAX_FIELDS, fieldCount);
                        
                        if (fieldCount >= 4) {
                            // Create student: name, rollNum, branch, year
                            RollNumType rollNum = parseRollNum(fields[1]);
                            int year = my_atoi(fields[3]);
                            
                            Student<RollNumType, CourseIDType> student(
                                fields[0], rollNum, fields[2], year
                            );
                            
                            // Parse completed courses if present
                            if (fieldCount > 4) {
                                for (int f = 4; f < fieldCount; f += 2) {
                                    if (f + 1 < fieldCount) {
                                        // For const char* types, Course constructor will copy the string
                                        CourseIDType courseId = parseCourseId(fields[f]);
                                        int grade = my_atoi(fields[f + 1]);
                                        Course<CourseIDType> course(courseId, fields[f], grade);
                                        student.addCompletedCourse(course);
                                    }
                                }
                            }
                            
                            db.addStudent(student);
                        }
                        
                        freeFields(fields, fieldCount);
                    }
                    
                    delete[] line;
                }
                lineStart = i + 1;
                
                if (fileContent[i] == '\0') break;
            }
        }
        
        delete[] fileContent;
        return true;
    }
    
    // Generate sample CSV for testing
    static void generateSampleCSV(const char* filename, int numRecords) {
        // Open file for writing (syscall 2: open, flags: O_WRONLY|O_CREAT|O_TRUNC = 0x241, mode: 0644)
        long fd = syscall4(2, (long)filename, 0x241, 0644, 0);
        if (fd < 0) return;
        
        // Write header
        const char* header = "Name,RollNumber,Branch,Year,Course1,Grade1,Course2,Grade2\n";
        syscall3(1, fd, (long)header, my_strlen(header));
        
        // Write records
        const char* branches[] = {"CSE", "ECE", "CSAM", "CSD", "CSB"};
        const char* courses[] = {"OOPD", "DSA", "OS", "CN", "DBMS", "AI", "ML"};
        
        for (int i = 0; i < numRecords; i++) {
            char line[256];
            int pos = 0;
            
            // Name
            const char* namePrefix = "Student";
            for (int j = 0; namePrefix[j]; j++) line[pos++] = namePrefix[j];
            
            // Convert i to string
            char numStr[20];
            int numLen = 0;
            int temp = i + 1;
            if (temp == 0) {
                numStr[numLen++] = '0';
            } else {
                int digits[20];
                int digitCount = 0;
                while (temp > 0) {
                    digits[digitCount++] = temp % 10;
                    temp /= 10;
                }
                for (int j = digitCount - 1; j >= 0; j--) {
                    numStr[numLen++] = '0' + digits[j];
                }
            }
            numStr[numLen] = '\0';
            
            for (int j = 0; j < numLen; j++) line[pos++] = numStr[j];
            line[pos++] = ',';
            
            // Roll number (use a scrambled pattern for unsorted data)
            int rollNum = 2020000 + ((i * 7919) % numRecords); // Prime number for scrambling
            temp = rollNum;
            numLen = 0;
            int digits[20];
            int digitCount = 0;
            while (temp > 0) {
                digits[digitCount++] = temp % 10;
                temp /= 10;
            }
            for (int j = digitCount - 1; j >= 0; j--) {
                line[pos++] = '0' + digits[j];
            }
            line[pos++] = ',';
            
            // Branch
            const char* branch = branches[i % 5];
            for (int j = 0; branch[j]; j++) line[pos++] = branch[j];
            line[pos++] = ',';
            
            // Year
            int year = 2020 + (i % 4);
            temp = year;
            numLen = 0;
            digitCount = 0;
            while (temp > 0) {
                digits[digitCount++] = temp % 10;
                temp /= 10;
            }
            for (int j = digitCount - 1; j >= 0; j--) {
                line[pos++] = '0' + digits[j];
            }
            line[pos++] = ',';
            
            // Course 1 - ensure OOPD appears frequently with good grades
            const char* course1 = (i % 3 == 0) ? "OOPD" : courses[i % 7];
            for (int j = 0; course1[j]; j++) line[pos++] = course1[j];
            line[pos++] = ',';
            
            // Grade 1 - ensure many students get 9 or 10
            int grade1 = 6 + (i % 5);  // Grades: 6,7,8,9,10
            if (grade1 >= 10) {
                line[pos++] = '1';
                line[pos++] = '0';
            } else {
                line[pos++] = '0' + grade1;
            }
            line[pos++] = ',';
            
            // Course 2
            const char* course2 = courses[(i + 1) % 7];
            for (int j = 0; course2[j]; j++) line[pos++] = course2[j];
            line[pos++] = ',';
            
            // Grade 2
            int grade2 = 6 + (i % 5);
            if (grade2 >= 10) {
                line[pos++] = '1';
                line[pos++] = '0';
            } else {
                line[pos++] = '0' + grade2;
            }
            line[pos++] = '\n';
            
            line[pos] = '\0';
            
            syscall3(1, fd, (long)line, pos);
        }
        
        // Close file
        syscall3(3, fd, 0, 0);
    }
};

#endif
