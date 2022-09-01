#include "options.h"
#include "utils.h"
#include <stdlib.h>
#include <assert.h>

#define RELEASE(p) if (p) {free(p);p=NULL;}

void destory_param(struct param *p) {
    if (p == NULL) { return; }
    RELEASE(p->key);
    RELEASE(p->flag);
    RELEASE(p->content);
    if (p->defaults == p->value) {
        av_destory(p->defaults);
        p->value = NULL;
    }
    else {
        av_destory(p->defaults);
        av_destory(p->value);
    }
}

struct option* construct_option(const char* cmd, const char *content, argvs_callback callback) {
    struct option* opt = malloc(sizeof(struct option));
    assert(opt);
    opt->cmd = construct_string(cmd);
    opt->content = construct_string(content);
    opt->callback = callback;
    opt->next     = NULL;
    opt->param_head_ptr = NULL;
    opt->param_tail_ptr = NULL;
    return opt;
}

void destory_option(struct option* opt) {
    if (opt == NULL) { return; }
    RELEASE(opt->cmd);
    RELEASE(opt->content);
    opt->callback = NULL;
    struct param *par = NULL;
    while(opt->param_head_ptr) {
        par = opt->param_head_ptr->next;
        destory_param(opt->param_head_ptr);
        opt->param_head_ptr = par;
    }
    opt->param_tail_ptr = NULL;
    free(opt);
    opt = NULL;
}

struct param* construct_param(const char* key,
                              const char* flag,
                              const char *content,
                              bool required,
                              enum evt type,
                              const void* defaults) {
    struct param* par = malloc(sizeof(struct param));
    assert(par);
    par->key = construct_key_string(key);
    par->flag = construct_flag_string(flag);
    par->content = construct_string(content);
    par->required = required;
    par->type = type;
    par->next     = NULL;
    par->value = NULL;
    if (defaults == NULL || type == VT_NONE) {
        par->defaults = NULL;
    } else {
        switch(type) {
            case VT_STRING:
                par->defaults = av_string(defaults);
                break;
            case VT_INT32:
                par->defaults = av_int32(*((int*)defaults));
                break;
            case VT_INT64:
                par->defaults = av_int64(*((INT64*)defaults));
                break;
            case VT_FLOAT:
                par->defaults = av_float(*((float *)defaults));
                break;
            case VT_DOUBLE:
                par->defaults = av_double(*((double *)defaults));
                break;
            case VT_BOOL:
                par->defaults = av_bool(*((bool *)defaults));
                break;
            case VT_STRING_ARRAY:
                par->defaults = NULL;
                break;
            default:
                break;
        }
    }

    return par;
}