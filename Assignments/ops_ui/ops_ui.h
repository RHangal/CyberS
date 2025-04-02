/* ops_ui.h */

#ifndef _ops_ui_H
#define _ops_ui_H

#include <stdint.h>

int bigmax(uint64_t *a, uint64_t *b);
int mid_ui(uint64_t *big, uint64_t lil);
uint64_t add_ui(uint64_t *big, uint64_t lil, uint64_t *out);
uint64_t sub_ui(uint64_t *big, uint64_t lil, uint64_t *out);
uint64_t mul_ui(uint64_t *big, uint64_t lil, uint64_t *out);
uint64_t quo_ui(uint64_t *big, uint64_t lil, uint64_t *out);
uint64_t rem_ui(uint64_t *big, uint64_t lil, uint64_t *out);

#endif /* _ops_u_H */
