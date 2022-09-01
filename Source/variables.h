//
// Created by 29627 on 2022/8/25.
//

#ifndef LIB_ARGVS_VARS_H
#define LIB_ARGVS_VARS_H


#include "utils.h"
#include <stdbool.h>


struct avariable* av_null();
struct avariable* av_string(const char* value);
struct avariable* av_int32(int value);
struct avariable* av_int64(INT64 value);
struct avariable* av_float(float value);
struct avariable* av_double(double value);
struct avariable* av_bool(bool value);
struct avariable* av_array(const struct argvs_array *ar);



const char* av_get_string(struct avariable* ptr);
int av_get_int32(struct avariable* ptr);
INT64 av_get_int64(struct avariable* ptr);
float av_get_float(struct avariable* ptr);
double av_get_double(struct avariable* ptr);
bool av_get_bool(struct avariable* ptr);
unsigned int av_get_array_size(const struct avariable* ptr);
const char* av_get_array_string_index(const struct avariable* ptr, int index);
const struct argvs_array* av_get_array_string(const struct avariable* ptr);

bool av_isnull(struct avariable* ptr);
enum evt av_get_type(struct avariable* ptr);
int  av_string_length(const struct avariable* ptr);
void av_destory(struct avariable* ptr);
const char* av_type_to_string(enum evt type);


#endif //LIB_ARGVS_VARS_H
