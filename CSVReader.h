// CSVReader.h - FINAL FIX with explicit field verification
#ifndef CSV_READER_H
#define CSV_READER_H

#include "StringUtils.h"
#include "Student.h"
#include "StudentDatabase.h"

extern "C" long syscall3(long number, long arg1, long arg2, long arg3);
extern "C" long syscall4(long number, long arg1, long arg2, long arg3, long arg4);

template<typename CourseIDType>
inline bool courseIdsEqual(const CourseIDType& a, const CourseIDType& b) {
    return a == b;
}

template<>
inline bool courseIdsEqual<const char*>(const char* const& a, const char* const& b) {
    
    extern int my_stricmp(const char* s1, const char* s2); 
    return my_stricmp(a, b) == 0;
}

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
                int fieldEnd = i;
                while (fieldEnd > fieldStart && my_isspace(line[fieldEnd - 1])) {
                    fieldEnd--;
                }
                
                int fieldLen = fieldEnd - fieldStart;
                fields[fieldCount] = new char[fieldLen + 1];
                
                for (int j = 0; j < fieldLen; j++) {
                    fields[fieldCount][j] = line[fieldStart + j];
                }
                fields[fieldCount][fieldLen] = '\0';
                
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

    static void intToChar(int num, char* buffer) {
        int i = 0;
        if (num == 0) {
            buffer[i++] = '0';
        } else {
            int digits[20];
            int digitCount = 0;
            bool negative = false;
            if (num < 0) {
                negative = true;
                num = -num;
            }
            
            while (num > 0) {
                digits[digitCount++] = num % 10;
                num /= 10;
            }
            
            if (negative) buffer[i++] = '-';
            
            for (int j = digitCount - 1; j >= 0; j--) {
                buffer[i++] = '0' + digits[j];
            }
        }
        buffer[i] = '\0';
    }

    static void uintToChar(unsigned int num, char* buffer) {
        int i = 0;
        if (num == 0) {
            buffer[i++] = '0';
        } else {
            unsigned int digits[20];
            int digitCount = 0;
            
            while (num > 0) {
                digits[digitCount++] = num % 10;
                num /= 10;
            }
            for (int j = digitCount - 1; j >= 0; j--) {
                buffer[i++] = '0' + digits[j];
            }
        }
        buffer[i] = '\0';
    }

public:
    template<typename RollNumType, typename CourseIDType>
    static bool loadFromCSV(const char* filename, 
                           StudentDatabase<RollNumType, CourseIDType>& db,
                           RollNumType (*parseRollNum)(const char*),
                           CourseIDType (*parseCourseId)(const char*)) {
        (void)parseRollNum;
        (void)parseCourseId;

        long fd = syscall3(2, (long)filename, 0, 0);
        if (fd < 0) return false;
        
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
                for (int i = 0; i < totalSize; i++) {
                    newContent[i] = fileContent[i];
                }
            }
            for (int i = 0; i < bytesRead; i++) {
                newContent[totalSize + i] = buffer[i];
            }
            newContent[totalSize + bytesRead] = '\0';
            
            delete[] fileContent;
            fileContent = newContent;
            totalSize += bytesRead;
        }
        
        syscall3(3, fd, 0, 0);
        delete[] buffer;
        
        if (totalSize == 0 || fileContent == nullptr) {
             return false;
        }
        
        int lineStart = 0;
        bool firstLine = true;
        
        for (int i = 0; i <= totalSize; i++) {
            if (fileContent[i] == '\n' || fileContent[i] == '\0') {
                if (i > lineStart) {
                    int lineLen = i - lineStart;
                    char* line = new char[lineLen + 1];
                    for (int j = 0; j < lineLen; j++) {
                        line[j] = fileContent[lineStart + j];
                    }
                    line[lineLen] = '\0';
                    
                    if (firstLine) {
                        firstLine = false;
                    } else {
                        const int MAX_FIELDS = 20;
                        char* fields[MAX_FIELDS];
                        int fieldCount;
                        
                        parseCSVLine(line, fields, MAX_FIELDS, fieldCount);
                        
                        // CSV Format: Name,RollNumber,Branch,Year,CurrentCourse,Course1,Grade1,Course2,Grade2
                        if (fieldCount >= 4) {
                            char* nameCopy = new char[my_strlen(fields[0]) + 1];
                            my_strcpy(nameCopy, fields[0]);
                            
                            char* rollCopy = new char[my_strlen(fields[1]) + 1];
                            my_strcpy(rollCopy, fields[1]);
                            
                            char* branchCopy = new char[my_strlen(fields[2]) + 1];
                            my_strcpy(branchCopy, fields[2]);
                            
                            int year = my_atoi(fields[3]);
                            
                            Student<RollNumType, CourseIDType> student(
                                nameCopy, rollCopy, branchCopy, year
                            );
                            
                            if (fieldCount > 4 && my_strlen(fields[4]) > 0) {
                                char* courseIdCopy = new char[my_strlen(fields[4]) + 1];
                                my_strcpy(courseIdCopy, fields[4]);
                                
                                char* courseNameCopy = new char[my_strlen(fields[4]) + 1];
                                my_strcpy(courseNameCopy, fields[4]);
                                
                                student.addCurrentCourse(Course<CourseIDType>(courseIdCopy, courseNameCopy, -1));
                            }

                            if (fieldCount > 5) {
                                for (int f = 5; f < fieldCount; f += 2) {
                                    if (f + 1 < fieldCount) {
                                        char* completedIdCopy = new char[my_strlen(fields[f]) + 1];
                                        my_strcpy(completedIdCopy, fields[f]);
                                        
                                        char* completedNameCopy = new char[my_strlen(fields[f]) + 1];
                                        my_strcpy(completedNameCopy, fields[f]);
                                        
                                        int grade = my_atoi(fields[f + 1]);
                                        student.addCompletedCourse(Course<CourseIDType>(completedIdCopy, completedNameCopy, grade));
                                    }
                                }
                            }
                            
                            db.addStudent(student);
                            
                            delete[] nameCopy;
                            // **FIX HERE: DO NOT delete rollCopy. Student object must own this memory.**
                            // delete[] rollCopy; 
                            delete[] branchCopy;
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
    
    // COMPLETELY REWRITTEN - Building string step by step with clear field markers
    static void generateSampleCSV(const char* filename, int numRecords) {
        long fd = syscall4(2, (long)filename, 0x241, 0644, 0);
        if (fd < 0) return;
        
        // Write header
        const char* header = "Name,RollNumber,Branch,Year,CurrentCourse,Course1,Grade1,Course2,Grade2\n";
        syscall3(1, fd, (long)header, my_strlen(header));
        
        const char* names[] = {
            "Aarav", "Vivaan", "Aditya", "Vihaan", "Arjun",
            "Sai", "Arnav", "Ayaan", "Krishna", "Ishaan",
            "Shaurya", "Atharv", "Advik", "Pranav", "Reyansh",
            "Aaradhya", "Diya", "Saanvi", "Ananya", "Pari",
            "Navya", "Angel", "Ira", "Prisha", "Anaya",
            "Ravi", "Priya", "Amit", "Sneha", "Karan",
            "Nisha", "Rahul", "Divya", "Meera", "Rohan",
            "Anjali", "Vikram", "Pooja", "Aryan", "Kavya",
            "Aakash", "Simran", "Varun", "Tanvi", "Nikhil",
            "Ritu", "Siddharth", "Neha", "Kunal", "Shruti"
        };
        
        const char* surnames[] = {
            "Sharma", "Verma", "Gupta", "Singh", "Patel",
            "Kumar", "Reddy", "Yadav", "Jain", "Nair",
            "Mehta", "Desai", "Shah", "Agarwal", "Chopra",
            "Malhotra", "Kapoor", "Bansal", "Khanna", "Bhatia",
            "Sethi", "Arora", "Saxena", "Mishra", "Pandey",
            "Tiwari", "Chauhan", "Rajput", "Thakur", "Rathore",
            "Joshi", "Kulkarni", "Deshpande", "Patil", "Pawar",
            "Jadhav", "Shinde", "More", "Kale", "Bhosale",
            "Iyer", "Menon", "Pillai", "Das", "Roy",
            "Ghosh", "Mukherjee", "Chatterjee", "Banerjee", "Sen",
            "Khan", "Ali", "Ahmed", "Hussain", "Malik",
            "Sinha", "Choudhury", "Dutta", "Bose", "Chakraborty"
        };
        
        const char* branches[] = {"CSE", "ECE", "CSAM", "CSD", "CSB"};
        const char* iitBranches[] = {"CS", "EE", "ME", "CE", "CH"};
        
        const char* iiitCourses[] = {
            "OOPD", "DSA", "OS", "CN", "DBMS",
            "AI", "ML", "NLP", "CV", "SEC",
            "TOC", "COA", "ALGO", "WEB", "MOBILE",
            "CLOUD", "IOT", "CYBER", "GAME", "ROBOTICS"
        };
        
        const char* iitCourses[] = {
            "101", "202", "303", "401", "523",
            "601", "702", "815", "920", "1005",
            "111", "222", "333", "444", "555",
            "666", "777", "888", "999", "1111"
        };
        
        int iitCounter = 0;
        char lineBuffer[512];
        
        for (int i = 0; i < numRecords; i++) {
            int pos = 0;
            
            bool isIITStudent = (i % 150 == 0 && iitCounter < 20);
            if (isIITStudent) iitCounter++;
            
            // === Build complete record string ===
            
            // 1. NAME FIELD
            int nameIdx = i % 50;
            int surnameIdx = (i / 50) % 60;
            const char* firstName = names[nameIdx];
            const char* lastName = surnames[surnameIdx];
            
            // Copy first name
            int j = 0;
            while (firstName[j]) {
                lineBuffer[pos++] = firstName[j++];
            }
            lineBuffer[pos++] = ' ';
            
            // Copy last name
            j = 0;
            while (lastName[j]) {
                lineBuffer[pos++] = lastName[j++];
            }
            lineBuffer[pos++] = ',';  // End of Name field
            
            // 2. ROLL NUMBER FIELD
            char rollBuffer[32];
            if (isIITStudent) {
                // IIT: numeric roll
                unsigned int rollNum = 2021001 + iitCounter - 1;
                uintToChar(rollNum, rollBuffer);
            } else {
                // IIIT: MT21XXX or PhD21XXX
                int rpos = 0;
                const char* prefix = (i % 3 == 0) ? "PhD" : "MT";
                
                j = 0;
                while (prefix[j]) {
                    rollBuffer[rpos++] = prefix[j++];
                }
                
                // Year part
                int yearSuffix = 21 + (i / 600);
                char yearStr[8];
                intToChar(yearSuffix, yearStr);
                
                // Pad year to 2 digits
                if (yearStr[1] == '\0') {
                    rollBuffer[rpos++] = '0';
                }
                j = 0;
                while (yearStr[j]) {
                    rollBuffer[rpos++] = yearStr[j++];
                }
                
                // Serial number (3 digits)
                int serialNum = (i % 600) + 1;
                char serialStr[8];
                intToChar(serialNum, serialStr);
                
                int serialLen = my_strlen(serialStr);
                for (int pad = 0; pad < 3 - serialLen; pad++) {
                    rollBuffer[rpos++] = '0';
                }
                j = 0;
                while (serialStr[j]) {
                    rollBuffer[rpos++] = serialStr[j++];
                }
                
                rollBuffer[rpos] = '\0';
            }
            
            // Copy roll number to line
            j = 0;
            while (rollBuffer[j]) {
                lineBuffer[pos++] = rollBuffer[j++];
            }
            lineBuffer[pos++] = ',';  // End of Roll Number field
            
            // 3. BRANCH FIELD
            const char* branch = isIITStudent ? iitBranches[i % 5] : branches[i % 5];
            j = 0;
            while (branch[j]) {
                lineBuffer[pos++] = branch[j++];
            }
            lineBuffer[pos++] = ',';  // End of Branch field
            
            // 4. YEAR FIELD
            int year = 2020 + (i % 4);
            char yearBuf[8];
            intToChar(year, yearBuf);
            j = 0;
            while (yearBuf[j]) {
                lineBuffer[pos++] = yearBuf[j++];
            }
            lineBuffer[pos++] = ',';  // End of Year field
            
            // 5. CURRENT COURSE FIELD
            const char* currentCourse = (i % 2 == 0) ? iiitCourses[i % 20] : iitCourses[i % 20];
            j = 0;
            while (currentCourse[j]) {
                lineBuffer[pos++] = currentCourse[j++];
            }
            lineBuffer[pos++] = ',';  // End of Current Course field
            
            // 6. COMPLETED COURSE 1
            const char* completedCourse1 = ((i + 1) % 2 == 0) ? iiitCourses[(i + 3) % 20] : iitCourses[(i + 3) % 20];
            j = 0;
            while (completedCourse1[j]) {
                lineBuffer[pos++] = completedCourse1[j++];
            }
            lineBuffer[pos++] = ',';
            
            // 7. GRADE 1
            int grade1 = 6 + (i % 5);
            char gradeBuf[4];
            intToChar(grade1, gradeBuf);
            j = 0;
            while (gradeBuf[j]) {
                lineBuffer[pos++] = gradeBuf[j++];
            }
            lineBuffer[pos++] = ',';
            
            // 8. COMPLETED COURSE 2
            const char* completedCourse2 = ((i + 2) % 2 == 0) ? iiitCourses[(i + 7) % 20] : iitCourses[(i + 7) % 20];
            j = 0;
            while (completedCourse2[j]) {
                lineBuffer[pos++] = completedCourse2[j++];
            }
            lineBuffer[pos++] = ',';
            
            // 9. GRADE 2
            int grade2 = 6 + ((i + 1) % 5);
            intToChar(grade2, gradeBuf);
            j = 0;
            while (gradeBuf[j]) {
                lineBuffer[pos++] = gradeBuf[j++];
            }
            
            // End of line
            lineBuffer[pos++] = '\n';
            lineBuffer[pos] = '\0';
            
            // Write line to file
            syscall3(1, fd, (long)lineBuffer, pos);
        }
        
        syscall3(3, fd, 0, 0);
    }
};

#endif