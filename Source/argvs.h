//
// Created by 29627 on 2022/8/25.
//


#ifndef LIB_ARGVS_ARGVS_H
#define LIB_ARGVS_ARGVS_H

#include "options.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void        argvs_init();
void        argvs_destory();
void        argvs_help_print();
int         argvs_parse(int argc, char** argv);
const char* argvs_errno(int noerr);

int  argvs_option_append(const char *cmd, const char *content, argvs_callback callback);
int  argvs_option_flags_append_string(const char *cmd,
                                      const char *key,
                                      const char *flag,
                                      const char *content,
                                      bool required,
                                      const char* defaults,
                                      struct avariable*** out);
int  argvs_option_flags_append_int64(const char *cmd,
                                     const char *key,
                                     const char *flag,
                                     const char *content,
                                     bool required,
                                     const INT64*  defaults,
                                     struct avariable*** out);
int  argvs_option_flags_append_int32(const char *cmd, const char *key,  const char *flag,
                                     const char *content, bool required,  const int*    defaults,
                                     struct avariable*** out);
int  argvs_option_flags_append_bool(const char *cmd, const char *key,   const char *flag,
                                    const char *content, bool required,   const bool*   defaults,
                                    struct avariable*** out);
int  argvs_option_flags_append_float(const char *cmd, const char *key,  const char *flag,
                                     const char *content, bool required,  const float*  defaults,
                                     struct avariable*** out);
int  argvs_option_flags_append_double(const char *cmd, const char *key, const char *flag,
                                      const char *content, bool required, const double* defaults,
                                      struct avariable*** out);
int  argvs_option_flags_append_array_string(const char *cmd, const char *key, const char *flag,
                                            const char *content, bool required, struct avariable*** out);

const char* argvs_av_get_string(struct avariable** ptr);
char argvs_av_get_int8(struct avariable** ptr);
short argvs_av_get_int16(struct avariable** ptr);
int argvs_av_get_int32(struct avariable** ptr);
INT64 argvs_av_get_int64(struct avariable** ptr);
float argvs_av_get_float(struct avariable** ptr);
double argvs_av_get_double(struct avariable** ptr);
bool argvs_av_get_bool(struct avariable** ptr);
unsigned int argvs_av_get_array_size(const struct avariable** ptr);
const char* argvs_av_get_array_string_index(const struct avariable** ptr, int index);



#ifdef __cplusplus
} // closing brace for extern "C"
#endif // __cplusplus

#endif