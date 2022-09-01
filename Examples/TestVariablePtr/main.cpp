//
// Created by 29627 on 2022/8/31.
//
#include "libargvs.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>


int main(int argc, char **argv) {
    argvs_init();
    argvs_destory();
    std::cout << "argv : " << "结束" << std::endl;
}
