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

    /* Test extend */

    printf("\n1. Extend (10 pts)\n");
    printf("Extend [1, 2] with [3, 4], should print:\n");
    printf("[1, 2, 3, 4]\n");
    printf("Does print:\n");
    list_extend(l,l2);
    list_print(l);


    /* Test insert */

    printf("\n2. Insert (10 pts)\n");
    printf("Insert 5 into list_t at position 2, should print:\n");
    printf("[1, 2, 5, 3, 4]\n");
    printf("Does print:\n");
    list_insert(l,2,(void *)5);
    list_print(l);

    
    /* Test remove */

    printf("\n3. Remove (10 pts)\n");
    printf("Remove 5 from list_t, should print:\n");
    printf("[1, 2, 3, 4]\n");
    printf("Does print:\n");
    list_remove(l,(void *)5);
    list_print(l);

    /* Test pop */

    printf("\n4. Pop (10 pts)\n");
    printf("Pop 1th element from list_t, should print:\n");
    printf("[1, 3, 4]\n");
    printf("Does print:\n");
    list_pop(l,1);
    list_print(l);

    /* Test clear */

    printf("\n5. Clear (10 pts)\n");
    printf("Clearing list, should print:\n");
    printf("[]\n");
    printf("Does print:\n");
    list_clear(l);
    list_print(l);

    free(l);
    free(l2);

  

    return 0;
}

