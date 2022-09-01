#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STRING_MAX 511
#define STRING_KEY_MAX STRING_MAX - 1
#define STRING_FLAG_MAX STRING_MAX - 2

#ifdef _WIND32
#define PATH_DELIMITER '\\'
#else
#define PATH_DELIMITER '/'
#endif

char *tolower_string(char* str) {
    char *orign=str;
    while((*orign) != '\0') {
        if ((*orign) >= 'A' && (*orign) <= 'Z') {
            (*orign) += 32;
        }
        orign++;
    }

    return str;
}

int equal_string(const char* key1, const char* key2) {
    if (strncmp(key1, key2, STRING_MAX) == 0) {
        return 1;
    }
    return 0;
}

char *construct_string(const char* value) {
    if (value == NULL) { return NULL; }
    int length = strlen(value) > STRING_MAX ? STRING_MAX : (int)strlen(value);
    char *result = malloc(length + 1);
    assert(result);
    memcpy(result, value, length);
    result[length] = '\0';

    return result;
}

char *construct_key_string(const  char* value) {
    if (value == NULL) { return NULL; }
    int length = strlen(value) > STRING_KEY_MAX ? STRING_KEY_MAX : (int)strlen(value);
    char *result = malloc(length + 2);
    assert(result);
    result[0] = '-';
    memcpy((void*)&result[1], (void const *)value, (size_t)length);
    result[length + 1] = '\0';
    return result;
}

char *construct_flag_string(const char* value) {
    if (value == NULL) { return NULL; }
    int length = strlen(value) > STRING_FLAG_MAX ? STRING_FLAG_MAX : (int)strlen(value);
    char *result = malloc(length + 3);
    assert(result);
    result[0] = '-';
    result[1] = '-';
    memcpy((void*)&result[2], (void const *)value, (size_t)length);
    result[length + 2] = '\0';
    return result;
}

struct argvs_array *split_string(const char* source,const char* delimiter) {
   const char* spstr = strtok((char*)source, delimiter);
   if (spstr == NULL) { return NULL; }
   struct argvs_array* result = construct_array(0);
   while(spstr) {
       append_array(result, construct_string(spstr));
       spstr = strtok(NULL, delimiter);
   }

   return result;
}

void destory_split_string(struct argvs_array *p) {
    if (p == NULL) {return;}
    int sz = get_array_size(p);
    for (int i = 0;i < sz;i++) {
        void* pdat = get_array_index(p, i);
        if (pdat == NULL) {
            continue;
        }

        free(pdat);
        set_array_index(p, i, NULL);
    }
    destory_array(p);
}

const char* get_app_name(const char* file_path) {
    int length = (int)strlen(file_path);
    assert(length <= STRING_MAX);


    char *p0 = strrchr(file_path, PATH_DELIMITER);
    if (p0 == NULL) {
        char *p1= strrchr(file_path, '.');
        if (p1 == NULL) {
            return  construct_string(file_path);
        }


        char *px = strtok((char*)file_path, ".");
        if (px == NULL) {
            return construct_string(file_path);
        }
        return construct_string(px);
    }


    char temp[STRING_MAX + 1];
    memset(temp, 0, STRING_MAX + 1);
    strcpy(temp, p0 + 1);

    char *px = strtok((char*)temp, ".");
    if (px == NULL) {
        return construct_string(temp);
    }

    return construct_string(px);
}

const char* get_app_path(const char* file_path) {
    int length = (int)strlen(file_path);
    assert(length <= STRING_MAX);
    char *p0 = strrchr(file_path, PATH_DELIMITER);
    if (p0 == NULL) {
        return construct_string(".");
    }

    int sub_len = (int)(p0 - file_path);
    char *result = (char*)malloc(sub_len + 1);
    assert(result);
    memcpy(result, file_path, sub_len);
    result[sub_len] = '\0';

    return result;
}

const char* get_app_expand(const char* file_name) {
    int length = (int)strlen(file_name);
    assert(length <= STRING_MAX);
    char *p0 = strrchr(file_name, '.');
    if (p0 == NULL) {
        return construct_string("");
    }

    return construct_string(p0 + 1);
}
