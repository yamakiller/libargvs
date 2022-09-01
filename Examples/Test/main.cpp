#include "libargvs.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <assert.h>

#define STRING_MAX 511
#define STRING_KEY_MAX STRING_MAX - 1
#define STRING_FLAG_MAX STRING_MAX - 2
#define ALIGN_BIT 8

char* construct_string(const char* value) {
	if (value == NULL) { return NULL; }
	int length = strlen(value) > STRING_MAX ? STRING_MAX : (int)strlen(value);
	char* result = (char*)malloc(length + 1);
	assert(result);
	memcpy(result, value, length);
	result[length] = '\0';

	return result;
}
struct avariable* av_malloc(int extern_size) {
	size_t msz = sizeof(struct avariable) + extern_size;
	struct avariable* ptr = (struct avariable*)_aligned_malloc(msz, ALIGN_BIT);
	assert(ptr);
	ptr->type = VT_NONE;
	return ptr;
}

avariable** out_str = NULL;
avariable** out_i64 = NULL;
avariable** out_i32 = NULL;
avariable** out_i16 = NULL;
avariable** out_i8 = NULL;
avariable** out_bool = NULL;
avariable** out_float = NULL;
avariable** out_double = NULL;
avariable** out_array_string = NULL;

avariable* test = NULL;

void callback() {
	std::cout << "out_str ret: " << argvs_av_get_string(out_str) << std::endl;
	//std::cout << "out_i64 ret: " << av_get_int64(av_conversion(out_i64)) << std::endl;
	//std::cout << "out_i32 ret: " << av_get_int32(av_conversion(out_i32)) << std::endl;
	//std::cout << "out_bool ret: " << av_get_bool(av_conversion(out_bool)) << std::endl;
	//std::cout << "out_float ret: " << av_get_float(av_conversion(out_float)) << std::endl;
	//std::cout << "out_double ret: " << av_get_double(av_conversion(out_double)) << std::endl;
	//std::cout << "out_array_string ret: " << av_get_int32(**out_array_string) << std::endl;
}


int main(int argc, char** argv)
{
    argvs_init();
	argvs_option_append("test", "test", callback);
    fprintf(stderr, "");
	argvs_option_flags_append_string("test", "T", "Type", "....", true, "Release", &out_str);

	/*const INT64 i64 = -1;
	out_i64 = (avariable**)malloc(sizeof(avariable**) * 1);
	argvs_option_flags_append_int64("test", "I64", "Int64", "....", false, &i64, out_i64);

	const int i32 = -1;
	out_i32 = (avariable**)malloc(sizeof(avariable**) * 1);
	argvs_option_flags_append_int32("test", "I32", "Int32", "....", false, &i32, out_i32);


	const bool b = true;
	out_bool = (avariable**)malloc(sizeof(avariable**) * 1);
	argvs_option_flags_append_bool("test", "B", "Bool", "....", false, &b, out_bool);

	const float f =(float) 0.1;
	out_float = (avariable**)malloc(sizeof(avariable**) * 1);
	argvs_option_flags_append_float("test", "F", "Float", "....", false, &f, out_float);

	const double d = 0.1;
	out_double = (avariable**)malloc(sizeof(avariable**) * 1);
	argvs_option_flags_append_double("test", "D", "Double", "....", false, &d, out_double);

	//out_array_string = (avariable***)malloc(sizeof(avariable***) * 1);
	//argvs_option_flags_append_array_string("test", "A", "Array", "....", false,out_array_string);*/
	
	int ret = argvs_parse(argc, argv);
	std::cout << "argvs_parse ret: " << 0 << std::endl;
	argvs_destory();

}