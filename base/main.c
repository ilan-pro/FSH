

#include "String.h"
#include "new.h"
#include <stdio.h>
int main(){

    String s = obj_new(_String_);
    printf("%s\n", obj_to_string(s));
        
    if (obj_instance_of(s, _String_)){
        printf("AHA ! C'est un String !\n");
    }
    else{
        printf("Ce n'est pas un String ??\n");
    }

    obj_destroy(s);
}
