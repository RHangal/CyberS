#include "list_t.h"


/*
 _: New (a la Python __init__)
 inputs: none
 outputs: a new list_t containing no values
 side effects: none
 */
list_t list_new() {
    list_t l = (list_t)malloc(sizeof(*l));
    if(!l) {
        fprintf(stderr,"Memory allocation failed, r.i.p\n");
        return NULL;
    }
    l->head = NULL;
    l->size = 0;

    return l;
}

/*
 _: Free (no Python equivalent)
 inputs: a list_t l
 outputs: nothing
 side effects: frees all memory associated with l
 */
void list_free(list_t l) {
    if(!l) {
        fprintf(stderr, "List is NULL, r.i.p\n");
        return;
    }

    list_node* curr = l->head;
    list_node* next;
    while(curr) {
        next = curr->next;
        /*free(curr->val);*/
        free(curr);
        curr = next;
    }

    free(l);
}

/*
 _: Print (a la Python __str__)
 _: Print (a la Python __str__)
 inputs: a list_t l
 outputs: nothing
 side effects: the elements of l are printed as in python
 example:
 list_print(list_new());
 - should print "[]"
 Note: Prints void *'s as uint64_t's. (use %lu)
 */
void list_print(list_t l) {
    if(!l) {
        fprintf(stderr, "List is NULL, r.i.p\n");
        return;
    }

    list_node* curr = l->head;
    printf("[");
    while(curr) {
        printf("%lu", (uint64_t)(uintptr_t)curr->val);
        if(curr->next) { 
            printf(", ");
        }
        curr = curr->next;
    }
    printf("]\n");
    
}

/*
 0: Append
 inputs: a list_t l, a pointer to an memory object of any type x
 outputs: nothing
 side effects: x is added to the end of l
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_print(l);
 - should print "[1]"
 */
void list_append(list_t l, void *x) {
    if(!l) {
        fprintf(stderr, "List is NULL, r.i.p\n");
        return;
    }
    list_node* curr = l->head;
    list_node* node = (list_node*)malloc(sizeof(list_node));
    if(!node) {
        fprintf(stderr, "Memory allocation failed, r.i.p\n");
        return;
    }

    node->val = x;
    node->next = NULL;
    
    if(!curr) {
        l->head = node;
    } else {
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = node;
    }
    
    l->size++;

}

/*
 1: Extend
 inputs: two list_ts, l1 and l2
 outputs: nothing
 side effects: all elements of l2 are appended to l1
 example:
 list_t l1 = list_new();
 uint64_t *val = 1;
 list_append(l1, val);
 list_t l2 = list_new();
 *val = 2;
 list_append(l2, val);
 list_print(l1);
 - should print "[1, 2]"
 */
void list_extend(list_t l1, list_t l2) {
    if(!l1) {
        printf("l1 is NULL, r.i.p");
        return;
    }
    if(!l2 || !l2->head) {
        printf("l2 is NULL, r.i.p");
        return;
    }
    
    if(!l1->head) {
        l1->head = l2->head;
    } else {
        list_node* curr = l1->head;
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = l2->head;
    }

    l1->size+= l2->size;
}

/*
 2: Insert
 inputs: a list_t l, size_t list index i, and a pointer to an memory object of any type x
 outputs: nothing
 side effects: x is added to l with index i and all elements are preserved in l
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_insert(l, 0, val);
 list_print(l);
 - should print "[2, 1]"
 */
void list_insert(list_t l, size_t i, void *x) {
    if(!l) {
        fprintf(stderr, "List is NULL, r.i.p\n");
        return;
    }
    if(i > l->size) {
        fprintf(stderr, "Index out of range, r.i.p\n");
        return;
    }
    list_node* node = (list_node*)malloc(sizeof(list_node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed, r.i.p\n");
        return;
    }
    node->val = x;
    size_t index;
    list_node* curr;
    list_node* next;
    if(i == 0){
        node->next = l->head;
        l->head = node;
        l->size++;
    } else if (i != l->size) {
        index = 0;
        curr = l->head;
        while(index != i-1) {
            curr = curr->next;
            index++;
        }
        next = curr->next;
        curr->next = node;
        node->next = next;
        l->size++;
    } else {
        free(node); 
        list_append(l,x);
    }
}
        


         


/*
 3: Remove
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: TRUE if an instance of x is removed, FALSE otherwise
 side effects: the first instance of x is removed from l
 example:
 list_t l = list_new();
 uint64_t *val = 2;
 list_append(l, val);
 list_append(l, val);
 *val = 1;
 list_print(l1);
 list_remove(l, 1, val);
 list_print(l);
 - should return True
 - should print "[2, 1, 2]" then "[1, 2]"
 */
bool list_remove(list_t l, void *x) {
    if(!l || !l->head) {
        fprintf(stderr, "NULL list, r.i.p\n");
        return FALSE;
    }
    list_node* curr = l->head;
    list_node* prev;
    if((uint64_t)(uintptr_t)curr->val == (uint64_t)(uintptr_t)x) {
        l->head = curr->next;
        free(curr);
        l->size--;
        return TRUE;
    } else {
        prev = curr;
        curr = curr->next;
        while(curr){
            if((uint64_t)(uintptr_t)curr->val == (uint64_t)(uintptr_t)x){
                prev->next = curr->next;
                free(curr);
                l->size--;
                return TRUE;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return FALSE;
}
    


/*
 4: Pop
 inputs: a list_t l, and a size_t list index i
 outputs: The element at index i, or exit(1) if i is out of range
 side effects: remove the element at index i
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_insert(l, 0, val);
 list_print(l);
 printf("%d\n", pop(l, 1));
 list_print(l);
 - should print "[1, 2]" then "2" then "[1]"
 */
void *list_pop(list_t l, size_t i) {
    if(!l || !l->head) {
        fprintf(stderr, "NULL list, r.i.p\n");
        return NULL; 
    }

    if(i >= l->size) {
        fprintf(stderr, "Index out of range\n");
        exit(1);
    }
    list_node* curr = l->head;
    list_node* prev;
    size_t index;
    void* val;
    if(i == 0){
        val = curr->val;
        l->head = curr->next;
        free(curr);
        l->size--;
        return val;
    } else {
        index = 1;
        prev = curr;
        curr = curr->next;
        while(index != i) {
            prev = curr;
            curr = curr->next;
            index++;
        }
        val = curr->val;
        prev->next = curr->next;
        free(curr);
        l->size--;
        return val;
    }
}







/*
 5: Clear
 inputs: a list_t l
 outputs: nothing
 side effects: l contains no elements
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 clear(l)
 - should print "[]"
 */
void list_clear(list_t l) {
   if(!l) {
        fprintf(stderr, "List is NULL, r.i.p\n");
        return;
    }

    list_node* curr = l->head;
    list_node* next;
    while(curr) {
        next = curr->next;
        /*free(curr->val);*/
        free(curr);
        curr = next;
    }

    l->head = NULL;
    l->size = 0;
}

/*
 6: Index
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: a size_t i giving the index of x in l, or exit(1) if x is not in l.
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 printf("%d\n", index(l, val)));
 - should print "0"
 */
size_t list_index(list_t l, void *x);

/*
 7: Count
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: The number of times x occurs in l
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_append(l, val);
 list_count(l, val);
 - should return 2
 */
uint64_t list_count(list_t l, void *x);

/* ~: Sort
 "Extra credit" for a sorting algorithm
 More "Extra credit" for an O(n*log(n)) sort
 */
/* void sort(list_t l, bool (greater_than)(void *, void *)) */

/*
 8: Reverse
 inputs: a list_t l
 outputs: nothing
 side effects: the elements of l are reversed
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_append(l, val);
 list_print(l);
 list_reverse(l)
 list_print(l);
 - should print "[1, 2]" then "[2, 1]"
 */
void list_reverse(list_t l);

/*
 9: Copy
 inputs: a list_t l
 outputs: a list_t r containing all the elements of l
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_t r = copy(l);
 list_append(l, val);
 list_print(l);
 list_print(r);
 - should print "[1, 1]" then "[1]"
 */
list_t list_copy(list_t l);



