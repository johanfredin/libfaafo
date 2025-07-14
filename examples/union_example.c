#include <stdio.h>
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} VariantType;

typedef struct Variant {
    VariantType type;

    union {
        int as_integer;
        float as_float;
        char *as_string;
    } data;
} Variant;

static void Variant_print(const Variant *variant) {
    switch (variant->type) {
        case TYPE_INT:
            printf("INT: %d\n", variant->data.as_integer);
            break;
        case TYPE_FLOAT:
            printf("FLOAT: %f\n", variant->data.as_float);
            break;
        case TYPE_STRING:
            printf("STRING: %s\n", variant->data.as_string);
            break;
        default:
            printf("UNKNOWN TYPE\n");
    }
}


int main(int argc, char *argv[]) {
    const Variant v_int = {.type = TYPE_INT, .data.as_integer = 10};
    const Variant v_float = {.type = TYPE_FLOAT, .data.as_float = 15.5f};
    const Variant v_string = {.type = TYPE_STRING, .data.as_string = "hello"};

    Variant_print(&v_int);
    Variant_print(&v_float);
    Variant_print(&v_string);
}
