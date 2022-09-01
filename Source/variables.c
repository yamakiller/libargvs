#include "variables.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#ifdef _WIN32
#ifdef _WIN64
#define ALIGN_BIT 8
#define ALIGN_BIT_8
#else
#define ALIGN_BIT 4
#define ALIGN_BIT_4
#endif
#elif __APPLE__
#define ALIGN_BIT 8
#define ALIGN_BIT_8
#elif __linux__
#ifdef __x86_64__
#define ALIGN_BIT 8
#define ALIGN_BIT_8
#elif __i386__
#define ALIGN_BIT 4
#define ALIGN_BIT_4
#endif
#endif

struct avariable* av_malloc(int extern_size) {
    size_t msz = sizeof(struct avariable) + extern_size;
    struct avariable* ptr = (struct avariable*)_aligned_malloc(msz, ALIGN_BIT);
    assert(ptr);
    ptr->type = VT_NONE;
    return ptr;
}


bool av_isnull(struct avariable* ptr) {
    if (ptr->type == VT_NONE) return true;
    return false;
}

struct avariable* av_null() {
    struct avariable*  ptr = av_malloc(0);
    ptr->type = VT_NONE;
    return ptr;
}

struct avariable* av_string(const char* value) {
    struct avariable*  ptr = av_malloc((int)strlen(value) + 1);
    ptr->type = VT_STRING;
    memcpy(&ptr->value[0], value, strlen(value));
    ptr->value[strlen(value)] = '\0';
    return ptr;
}


struct avariable* av_int32(int value) {
    struct avariable*  ptr = av_malloc(0);
    ptr->type = VT_INT32;
    memcpy(&ptr->value[0], &value, sizeof(int));
    return ptr;
}

struct avariable* av_int64(INT64 value) {
#ifndef ALIGN_BIT_8
    struct avariable*  ptr = av_malloc(0);
#else
    struct avariable*  ptr = av_malloc(4);
#endif
    ptr->type = VT_INT64;
    memcpy(&ptr->value[0], &value, sizeof(INT64));
    return ptr;
}

struct avariable* av_float(float value) {
    struct avariable*  ptr = av_malloc(0);
    ptr->type = VT_FLOAT;
    memcpy(&ptr->value[0], &value, sizeof(float));
    return ptr;
}

struct avariable* av_double(double value) {
#ifndef ALIGN_BIT_8
    struct avariable*  ptr = av_malloc(0);
#else
    struct avariable*  ptr = av_malloc(4);
#endif
    ptr->type = VT_DOUBLE;
    memcpy(&ptr->value[0], &value, sizeof(double ));
    return ptr;
}

struct avariable* av_bool(bool value) {
    struct avariable*  ptr = av_malloc(0);
    ptr->type = VT_BOOL;
    ptr->value[0] = value ? 1 : 0;
    return ptr;
}

struct avariable* av_array(const struct argvs_array *ar) {
    struct avariable*  ptr = av_malloc(sizeof(struct argvs_array));
    ptr->type = VT_STRING_ARRAY;
    struct argvs_array *dest = (struct argvs_array*)&ptr->value[0];
    init_array(dest, 0);
    for (int i=0;i < get_array_size(ar); i++) {
        append_array(dest, construct_string(get_array_index(ar, i)));
    }

    return ptr;
}

const char* av_get_string(struct avariable* ptr) {
    return ptr->type == VT_NONE ? NULL : (const char*)&ptr->value[0];
}

char av_get_int8(struct avariable* ptr) {
    return ptr->type == VT_NONE ? (char)0 : ptr->value[0];
}

short av_get_int16(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return 0;
    }

    short result = 0;
    memcpy(&result, &ptr->value[0], sizeof(short));
    return result;
}

int av_get_int32(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return 0;
    }

    int result = 0;
    memcpy(&result, &ptr->value[0], sizeof(int));
    return result;
}

INT64 av_get_int64(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return 0;
    }

    INT64 result = 0;
    memcpy(&result, &ptr->value[0], sizeof(INT64));
    return result;
}

float av_get_float(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return 0.0f;
    }

    float result = 0;
    memcpy(&result, &ptr->value[0], sizeof(float));
    return result;
}

double av_get_double(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return 0.0f;
    }

    double result = 0;
    memcpy(&result, &ptr->value[0], sizeof(double));
    return result;
}

bool av_get_bool(struct avariable* ptr) {
    if (ptr->type == VT_NONE) {
        return false;
    }

    return ptr->value[0] == 0 ? false : true;
}

enum evt av_get_type(struct avariable* ptr) {
    return ptr->type;
}

int  av_string_length(const struct avariable* ptr) {
    if (ptr->type != VT_STRING) {
        return 0;
    }


    return (int)strlen((const char*)&ptr->value[0]);
}

unsigned int av_get_array_size(const struct avariable* ptr) {
    if (ptr->type != VT_STRING_ARRAY) {
        return 0;
    }

    return ((const struct argvs_array*)&ptr->value[0])->size;
}

const char* av_get_array_string_index(const struct avariable* ptr, int index) {
    if (ptr->type != VT_STRING_ARRAY) {
        return NULL;
    }

    return get_array_index((const struct argvs_array*)&ptr->value[0], index);
}

const struct argvs_array* av_get_array_string(const struct avariable* ptr) {
    if (ptr->type != VT_STRING_ARRAY) {
        return NULL;
    }

    return (const struct argvs_array*)&ptr->value[0];
}


void av_destory(struct avariable* ptr) {
    if (ptr == NULL) {
        return;
    }

    if (ptr->type == VT_STRING_ARRAY) {
        struct argvs_array* ar = ((struct argvs_array*)&ptr->value[0]);
        int sz = get_array_size((const struct argvs_array*)ar);
        for (int i=0;i<sz;i++) {
            char *p = get_array_index((const struct argvs_array*)ar, i);
            if (p == NULL) {continue;}
            free(p);
            set_array_index(ar, i, NULL);
        }

        if (ar->array) {
            free(ar->array);
            ar->array = NULL;
        }
    }
    
    _aligned_free(ptr);
    ptr = NULL;
}


const char* av_type_to_string(enum evt type) {
    switch(type) {
        case VT_STRING:
            return "string";
        case VT_INT64:
            return "int64";
        case VT_INT32:
            return "int";
        case VT_FLOAT:
            return "float";
        case VT_DOUBLE:
            return "double";
        case VT_BOOL:
            return "boolean";
        case VT_STRING_ARRAY:
            return "string array";
        default:
            return "null";
    }
}