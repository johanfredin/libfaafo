#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>

static int exception = 0;
static jmp_buf env;

#define RuntimeException 1
#define NumberFormatException 2
#define Catch(e) else if (exception == e)
#define Throw(e) longjmp(env, e)
#define Try if ((exception = setjmp(env)) == 0)

int is_number(const char* s, const int len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

void mess_up(const char *n) {

    if (!is_number(n, 255)) {
        Throw(NumberFormatException);
    }
    const int num = (int) *n;
    if (num <= 0) {
        Throw(RuntimeException);
    }
    printf("All good jefe %d\n", num);
}

int main(void) {
    Try {
        mess_up("hehe");
    } Catch(RuntimeException) {
        puts("You messed up");
    } Catch(NumberFormatException) {
        puts("You messed up with the number");
    }
    return 0;
}