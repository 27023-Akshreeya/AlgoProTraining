#include <stdio.h>
#include <stdbool.h>

int globalVar = 100;
static int fileVar = 50;

void func(char input){
    printf("Inside function scope demo\nthe input passed to this scope is: %c\n",input);
    printf("behavior of goto statements\n");
    
    goto goto_message;
        printf("this line is skipped\n");

    goto_message:
        printf("Jumped to this message\n");
}

void GlobalScopeDemo(){
    printf("Demonstration of global variable\n");
    printf("Global variable can be accessed from any function or file : %d\n",globalVar);
    printf("File scoped varible only accessible in this file: %d\n", fileVar);
}
int main()
{

    int local_int = 42;
    char local_char = 'A';
    float local_float = 12.34f;
    double local_double = 98.7654321;
    bool local_bool = true;

    printf("Demonstration of local variable\n");
    printf("int: %d (Size: %zu bytes)\n", local_int, sizeof(local_int));
    printf("char: %c (Size: %zu byte)\n", local_char, sizeof(local_char));
    printf("float: %.2f (Size: %zu bytes)\n", local_float, sizeof(local_float));
    printf("double: %.7f (Size: %zu bytes)\n", local_double, sizeof(local_double));
    printf("bool: %d (Size: %zu byte)\n", local_bool, sizeof(local_bool));

    printf("Block scope Demo\n");
    {
        int globalVar = 999;
        int blockOnlyVar = 10;
        printf("inside block global variable: %d, block only variable: %d\n",globalVar,blockOnlyVar);
    }
    printf("outside block where global variable is still : %d\n",globalVar);

    GlobalScopeDemo();
    func('c');

    return 0;
}