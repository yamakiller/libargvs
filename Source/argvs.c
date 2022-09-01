#include "argvs.h"
#include "utils.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define ARGVS_SUCCESS 0
#define ARGVS_ERROR_CMD_EXISTED_CODE (-1)
#define ARGVS_ERROR_CMD_EXISTED_STR "argvs the set command already exists"
#define ARGVS_ERROR_KEY_FLAGS_EXISTED_CODE (-2)
#define ARGVS_ERROR_KEY_FLAGS_EXISTED_STR "argvs the set key/flag already exists"
#define ARGVS_ERROR_CMD_DOES_NOT_EXIST_CODE (-3)
#define ARGVS_ERROR_CMD_DOES_NOT_EXIST_STR "argvs the specified command is undefined"
#define ARGVS_ERROR_MISSING_PARAMETERS_CODE (-4)
#define ARGVS_ERROR_MISSING_PARAMETERS_STR "argvs the missing required parameters"

#define RELEASE(p) if (p) {free(p);p=NULL;}

#define PRINT_BUFFER_MAX 1024

struct argvs {
	char* name;
	char* path;
	int                  option_size;
	struct option* option_head_ptr;
	struct option* option_tail_ptr;
};


static struct argvs* __ptr_argv_ = NULL;

bool __argvs_option_exits(struct argvs* args, const char* cmd) {
	if (args->option_head_ptr == NULL) { return false; }
	struct option* ptr = args->option_head_ptr;
	while (ptr != NULL) {
		if (equal_string(ptr->cmd, cmd)) {
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

struct option* __argvs_option_get(struct argvs* args, const char* cmd) {
	if (args->option_head_ptr == NULL) { return NULL; }
	struct option* ptr = args->option_head_ptr;
	while (ptr != NULL) {
		if (equal_string(ptr->cmd, cmd)) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

bool __argvs_option_flags_exits(struct option* opt, const char* key, const char* flag) {
	if (opt->param_head_ptr == NULL) { return false; }
	struct param* ptr = opt->param_head_ptr;
	while (ptr != NULL) {
		if (ptr->key != NULL && key != NULL) {
			if (equal_string(ptr->key, key)) {
				return true;
			}
		}

		if (ptr->flag != NULL && flag != NULL) {
			if (equal_string(ptr->flag, flag)) {
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}


struct param* __argvs_option_flags_find(struct option* opt, const char* kf) {
	if (opt->param_head_ptr == NULL) { return NULL; }
	struct param* ptr = opt->param_head_ptr;
	while (ptr != NULL) {
		if (ptr->key != NULL && kf != NULL) {
			if (equal_string(ptr->key, kf)) {
				return ptr;
			}
		}

		if (ptr->flag != NULL && kf != NULL) {
			if (equal_string(ptr->flag, kf)) {
				return ptr;
			}
		}
		ptr = ptr->next;
	}
	return NULL;
}

int __argvs_option_push(struct argvs* pargv, const char* cmd, const char* content, argvs_callback callback) {
	//查找这个命令是否已存在
	if (__argvs_option_exits(pargv, cmd)) {
		return ARGVS_ERROR_CMD_EXISTED_CODE;
	}

	struct option* opt = construct_option(cmd, content, callback);
	if (pargv->option_head_ptr == NULL) {
		pargv->option_head_ptr = pargv->option_tail_ptr = opt;
	}
	else {
		pargv->option_tail_ptr->next = opt;
		pargv->option_tail_ptr = opt;
	}
	pargv->option_size++;
	return ARGVS_SUCCESS;
}

int __argvs_option_flags_push(struct option* opt,
	const char* key,
	const char* flag,
	const char* content,
	bool required,
	enum evt type,
	const void* defaults,
	struct avariable*** out) {
	if (__argvs_option_flags_exits(opt, key, flag)) {
		return ARGVS_ERROR_KEY_FLAGS_EXISTED_CODE;
	}

	struct param* par = construct_param(key, flag, content, required, type, defaults);
	if (opt->param_head_ptr == NULL) {
		opt->param_head_ptr = opt->param_tail_ptr = par;
	}
	else {
		opt->param_tail_ptr->next = par;
		opt->param_tail_ptr = par;
	}
	*out = &par->value;
	return ARGVS_SUCCESS;
}

int __argvs_flags(const char* cmd,
	const char* key,
	const char* flag,
	const char* content,
	bool required,
	enum evt type,
	const void* defaults,
	struct  avariable*** out) {
	struct option* opt = __argvs_option_get(__ptr_argv_, cmd);
	if (opt == NULL) {
		return ARGVS_ERROR_CMD_DOES_NOT_EXIST_CODE;
	}

	return __argvs_option_flags_push(opt, key, flag, content, required, type, defaults, out);
}

void __argvs_param_value_push(struct param* par, const char* value) {
	switch (par->type) {
	case VT_STRING:
		par->value = av_string(value);
		break;
	case VT_INT32:
		par->value = av_int32(strtol(value, NULL, 10));
		break;
	case VT_INT64:
		par->value = av_int64(strtoll(value, NULL, 10));
		break;
	case VT_FLOAT:
		par->value = av_float(strtof(value, NULL));
		break;
	case VT_DOUBLE:
		par->value = av_double(strtod(value, NULL));
		break;
	case VT_BOOL:
		par->value = av_bool(strncmp(tolower_string((char*)value), "true", 6) == 0 ? true : false);
		break;
	case VT_STRING_ARRAY:
		par->value = av_array((struct argvs_array*)value);
		break;
	default:
		par->value = av_null();
		break;
	}
}

/**
 * 初始化参数系统
 */
void argvs_init() {
	if (__ptr_argv_ == NULL) {
		__ptr_argv_ = malloc(sizeof(struct argvs));
		assert(__ptr_argv_);
	}

	__ptr_argv_->name = NULL;
	__ptr_argv_->path = NULL;
	__ptr_argv_->option_size = 0;
	__ptr_argv_->option_head_ptr = __ptr_argv_->option_tail_ptr = NULL;
}

/**
 * 销毁参数系统
 */
void argvs_destory() {
	if (__ptr_argv_ == NULL) {
		return;
	}

	RELEASE(__ptr_argv_->name);
	RELEASE(__ptr_argv_->path);

	struct option* ptr = NULL;
	while (__ptr_argv_->option_head_ptr) {
		ptr = __ptr_argv_->option_head_ptr->next;
		destory_option(__ptr_argv_->option_head_ptr);
		__ptr_argv_->option_head_ptr = ptr;
	}
	__ptr_argv_->option_tail_ptr = NULL;
}

/**
 * 获取参数系统错误信息
 * @param noerr
 * @return
 */
const char* argvs_errno(int noerr) {
	switch (noerr) {
	case ARGVS_SUCCESS:
		return "success";
	case ARGVS_ERROR_CMD_EXISTED_CODE:
		return ARGVS_ERROR_CMD_EXISTED_STR;
	case ARGVS_ERROR_KEY_FLAGS_EXISTED_CODE:
		return ARGVS_ERROR_KEY_FLAGS_EXISTED_STR;
	case ARGVS_ERROR_CMD_DOES_NOT_EXIST_CODE:
		return ARGVS_ERROR_CMD_DOES_NOT_EXIST_STR;
	case ARGVS_ERROR_MISSING_PARAMETERS_CODE:
		return ARGVS_ERROR_MISSING_PARAMETERS_STR;
	default:
		return "unknown";
	}
}

/**
 * 打印帮助
 */
void argvs_help_print() {

}

void __argvs_print_option(struct option* opt) {
	char print_buffer[PRINT_BUFFER_MAX];
	memset(print_buffer, 0, PRINT_BUFFER_MAX);
	snprintf(print_buffer, PRINT_BUFFER_MAX, "%s Use:\n  ", opt->cmd);
	struct param* par = opt->param_head_ptr;

	const int temp_max = 256;
	char tmp[256];
	int  prvlen = (int)strlen(print_buffer);
	while (par) {
		snprintf(tmp, temp_max, "%s,%s %s %s %s\n",
			par->key ? par->key : par->flag,
			par->flag ? par->flag : par->key,
			av_type_to_string(par->type),
			par->content ? par->content : "",
			par->required ? "required" : "");

		for (int i = 0; i < prvlen; i++) {
			strcat(tmp, " ");
		}
		strcat(print_buffer, tmp);
		par = par->next;
	}
}

void __argvs_print_error(struct option* opt, struct param* par) {
	fprintf(stderr, "%s option input param error:%s[%s] is required %s\n",
		opt->cmd,
		par->key ? par->key : par->flag,
		par->flag ? par->flag : par->key,
		av_type_to_string(par->type));
	__argvs_print_option(opt);
}

/**
 * 解析参数
 * @param argc
 * @param argv
 * @return
 */
int argvs_parse(int argc, char** argv) {
	assert(__ptr_argv_);
	//1.解析命令，并获得完整路劲及文件名
	__ptr_argv_->name = (char*)get_app_name(argv[0]);
	__ptr_argv_->path = (char*)get_app_path(argv[0]);
	if (argc <= 1) {
		return ARGVS_SUCCESS;
	}

	//2.获取命令
	struct option* opt = __argvs_option_get(__ptr_argv_, argv[1]);
	if (opt == NULL) {
		return ARGVS_ERROR_CMD_DOES_NOT_EXIST_CODE;
	}

	for (int i = 2; i < argc; ) {
		const char* flags = NULL;
		struct argvs_array* flags_array = split_string(argv[i], "=");
		if (flags_array == NULL) {
			flags = argv[i];
		}
		else {
			flags = (const char*)get_array_index(flags_array, 0);
		}

		struct param* par = __argvs_option_flags_find(opt, flags);
		if (par == NULL) {
			i++;
			destory_split_string(flags_array);
			continue;
		}

		struct argvs_array* params_array = NULL;
		if (flags_array != NULL) {
			params_array = split_string((const char*)get_array_index(flags_array, 1), ",");
			if (params_array == NULL) {
				params_array = construct_array(0);
				append_array(params_array, construct_string((const char*)get_array_index(flags_array, 1)));
			}
			i++;
		}
		else {
			if ((i + 1) < argc) {
				if (__argvs_option_flags_find(opt, argv[i + 1]) != NULL) {
					i++;
					continue;
				}

				params_array = split_string((const char*)get_array_index(flags_array, 1), ",");
				if (params_array == NULL) {
					params_array = construct_array(0);
					append_array(params_array, argv[i + 1]);
				}

				i += 2;
			}
			else {
				i++;
				continue;
			}
		}
		//=================================================
		if (par->type == VT_STRING_ARRAY) {
			__argvs_param_value_push(par, (char*)params_array);
		}
		else {
			__argvs_param_value_push(par, get_array_index(params_array, 0));
		}
		destory_split_string(params_array);
	}

	//检测命令合法性
	struct param* current_param = opt->param_head_ptr;
	while (current_param) {
		if (current_param->required &&
			current_param->defaults == NULL &&
			(current_param->value == NULL || av_isnull(current_param->value))) {
			__argvs_print_error(opt, current_param);
			return ARGVS_ERROR_MISSING_PARAMETERS_CODE;
		}
		else if (current_param->defaults != NULL &&
			(current_param->value == NULL || av_isnull(current_param->value)))
		{
			current_param->value = current_param->defaults;
		}

		current_param = current_param->next;
	}
	//调用指令执行回调函数
	opt->callback();


	return ARGVS_SUCCESS;
}

int argvs_option_append(const char* cmd, const char* content, argvs_callback callback) {
	assert(__ptr_argv_);
	return __argvs_option_push(__ptr_argv_, cmd, content, callback);
}

int  argvs_option_flags_append_string(const char* cmd,
                                      const char* key,
                                      const char* flag,
                                      const char* content,
                                      bool required,
                                      const char* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_STRING, defaults, out);
}

int  argvs_option_flags_append_int64(const char* cmd,
                                     const char* key,
                                     const char* flag,
	                                 const char* content,
                                     bool required,
                                     const INT64* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_INT64, defaults, out);
}

int  argvs_option_flags_append_int32(const char* cmd,
                                     const char* key,
                                     const char* flag,
	                                 const char* content,
                                     bool required,
                                     const int* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_INT32, defaults, out);
}

int  argvs_option_flags_append_bool(const char* cmd,
                                    const char* key,
                                    const char* flag,
	                                const char* content,
                                    bool required,
                                    const bool* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_BOOL, defaults, out);
}

int  argvs_option_flags_append_float(const char* cmd,
                                     const char* key,
                                     const char* flag,
	                                 const char* content,
                                     bool required,
                                     const float* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_FLOAT, defaults, out);
}

int  argvs_option_flags_append_double(const char* cmd,
                                      const char* key,
                                      const char* flag,
	                                  const char* content,
                                      bool required,
                                      const double* defaults,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_DOUBLE, defaults, out);
}

int  argvs_option_flags_append_array_string(const char* cmd,
                                            const char* key,
                                            const char* flag,
	                                        const char* content,
                                            bool required,
	struct avariable*** out) {
	assert(__ptr_argv_);
	return __argvs_flags(cmd, key, flag, content, required, VT_STRING_ARRAY, NULL, out);
}


const char* argvs_av_get_string(struct avariable** ptr) {
    return av_get_string(*ptr);
}

int argvs_av_get_int32(struct avariable** ptr) {
    return av_get_int32(*ptr);
}

INT64 argvs_av_get_int64(struct avariable** ptr) {
    return av_get_int64(*ptr);
}

float argvs_av_get_float(struct avariable** ptr) {
    return av_get_float(*ptr);
}

double argvs_av_get_double(struct avariable** ptr) {
    return av_get_double(*ptr);
}


bool argvs_av_get_bool(struct avariable** ptr) {
    return av_get_bool(*ptr);
}

unsigned int argvs_av_get_array_size(const struct avariable** ptr) {
    return av_get_array_size(*ptr);
}

const char* argvs_av_get_array_string_index(const struct avariable** ptr, int index) {
    return av_get_array_string_index(*ptr, index);
}




