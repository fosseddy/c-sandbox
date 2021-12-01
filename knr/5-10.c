#include <stdio.h>
#include <stdlib.h>

void stack_push(int);
int stack_pop(void);
void stack_print(void);

int main(int argc, char **argv)
{
    (void) argc;

    ++argv;

    while (*argv) {
        switch (*argv[0]) {
            case '+': {
                int a = stack_pop();
                int b = stack_pop();
                stack_push(a + b);
            } break;

            case '-': {
                int b = stack_pop();
                int a = stack_pop();
                stack_push(a - b);
            } break;

            case '*': {
                int a = stack_pop();
                int b = stack_pop();
                stack_push(a * b);
            } break;

            case '/': {
                int b = stack_pop();
                int a = stack_pop();
                stack_push(a / b);
            } break;

            default:
                stack_push(atoi(*argv));
                break;
        }

        ++argv;
    }

    stack_print();

    return 0;
}

#define STACK_CAP 1000
static int stack[STACK_CAP] = {0};
static int sp = 0;

void stack_print(void)
{
    printf("%i\n", stack[--sp]);
}

void stack_push(int val)
{
    stack[sp] = val;
    ++sp;
}

int stack_pop(void)
{
    return stack[--sp];
}
