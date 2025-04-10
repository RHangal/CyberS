/* autograder.c */

/* tests list_t.c functionality */

#include "list_t.h"

int main()
{
    list_t l = list_new(), l2;

    /* Test new and print */

    printf("\nInitial Test\n");
    printf("Make new and print list_t should print:\n");
    printf("[]\n");
    printf("Does print:\n");
    list_print(l);

    /* Test free */

    list_free(l);
    printf("Free ran without error.\n");

    /* Test append */

    printf("\n0. Append (10 pts)\n");
    printf("Append 2 to [1] and 4 to [3], should print:\n");
    printf("[1, 2]\n");
    printf("[3, 4]\n");
    printf("Does print:\n");
    l = list_new();
    list_append(l,(void *)1);
    list_append(l,(void *)2);
    l2 = list_new();
    list_append(l2,(void *)3);
    list_append(l2,(void *)4);
    list_print(l);
    list_print(l2);

    free(l);
    free(l2);

  

    return 0;
}

