//
// Created by 29627 on 2022/8/25.
//

#ifndef LIB_ARGVS_OPTIONS_H
#define LIB_ARGVS_OPTIONS_H

#include "variables.h"
#include "array.h"

typedef void (*argvs_callback)();

struct param {
    char               *key;
    char               *flag;
    char               *content;
    bool                required;
    enum   evt          type;
    struct avariable*   defaults;
    struct avariable*   value;
    struct param*       next;
};

struct option {
    char           *cmd;
    char           *content;
    argvs_callback callback;
    struct option  *next;
    struct param   *param_head_ptr;
    struct param   *param_tail_ptr;
};

struct option* construct_option(const char* cmd, const char *content, argvs_callback callback);
void destory_option(struct option* opt);

struct param* construct_param(const char* key,
        const char* flag, const char *content, bool required, enum evt type, const void* defaults);


#endif