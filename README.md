#C版命名行库
##兼容Windows、Linux、Mac

examples
````
struct avariable** cmode = NULL;
    
void callback() {
    char* cmode_str = av_get_string(cmode);
}

main(int argc, char** argv) {  
    argvs_init();  
    argvs_option_append("test", "test", callback); 
	argvs_option_flags_append_string("test", "M", "Mode", "....", true, "Release", cmode);
    argvs_destory();  
}
````
