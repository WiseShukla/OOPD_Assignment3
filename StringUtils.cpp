// StringUtils.cpp
#include "StringUtils.h"

unsigned int my_strlen(const char* str) {
    if (!str) return 0;
    unsigned int len = 0;
    while (str[len]) len++;
    return len;
}

void my_strcpy(char* dest, const char* src) {
    if (!dest || !src) return;
    while ((*dest++ = *src++));
}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}


const char* my_strstr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return nullptr;
    if (*needle == '\0') return haystack;
    
    while (*haystack != '\0') {
        const char* h = haystack;
        const char* n = needle;
        
        while (*h == *n && *n != '\0') {
            h++;
            n++;
        }
        
        if (*n == '\0') {
            return haystack;
        }
        
        haystack++;
    }
    
    return nullptr;
}




char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

int my_atoi(const char* str) {
    if (!str || !*str) return 0;
    
    int result = 0;
    int sign = 1;
    int i = 0;
    
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}

int my_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}


void copyString(char*& dest, const char* src) {
    if (dest) delete[] dest;
    if (src) {
        dest = new char[my_strlen(src) + 1];
        my_strcpy(dest, src);
    } else {
        dest = nullptr;
    }
}
