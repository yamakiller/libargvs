//
// Created by 29627 on 2022/8/27.
//

#ifndef LIB_ARGVS_ARRAY_H
#define LIB_ARGVS_ARRAY_H


/**
 * 数组
 */
struct argvs_array {
    void          **array;
    unsigned int  size;
    unsigned int  cap;
};

struct argvs_array *construct_array(unsigned int cap);
void  init_array(struct argvs_array* ar, unsigned int cap);
void  destory_array(struct argvs_array *p);
void  append_array(struct argvs_array *p, void *data);
void  remove_array_index(struct argvs_array *p, int index);
void* get_array_index(const struct  argvs_array *p, int index);
void  set_array_index(struct argvs_array *p, int index, void *data);
int   get_array_size(const struct  argvs_array *p);
int   get_array_cap(const struct  argvs_array *p);


#endif //LIB_ARGVS_ARRAY_H
