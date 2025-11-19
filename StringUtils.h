// StringUtils.h
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

unsigned int my_strlen(const char* str);
void my_strcpy(char* dest, const char* src);
int my_strcmp(const char* s1, const char* s2);
const char* my_strstr(const char* haystack, const char* needle);
char my_tolower(char c);
int my_atoi(const char* str);
int my_isspace(char c);
void copyString(char*& dest, const char* src);

#endif
