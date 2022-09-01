#ifndef LIB_ARGVS_UTILS_H_H
#define LIB_ARGVS_UTILS_H_H

#include "array.h"

#ifdef __linux__
#define INT64 long
#elif WIN32
#define INT64 long long
#endif

enum evt {
    VT_INT32,
    VT_INT64,
    VT_FLOAT,
    VT_DOUBLE,
    VT_BOOL,
    VT_STRING,
    VT_STRING_ARRAY,
    VT_NONE,
};

#pragma pack(1)
struct avariable {
    enum evt type;
    char value[1];
};
#pragma pack()

char *tolower_string(char* str);

int   equal_string(const char* key1, const char* key2);
char *construct_string(const char* value);
char *construct_key_string(const  char* value);
char *construct_flag_string(const char* value);

struct argvs_array *split_string(const char* source,const char* delimiter);
void destory_split_string(struct argvs_array *p);

//
const char* get_app_name(const char* file_path);
const char* get_app_path(const char* file_path);
const char* get_app_expand(const char* file_name);

#endif