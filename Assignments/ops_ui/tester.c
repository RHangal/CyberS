#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../4096_t/4096_t.h"
#include "ops_ui.h"

/* hex string to  4096_t */
uint64_t bigh2i(char *h, uint64_t *n) {
	size_t l, i = 0, o; /* len, index, offset */
	memset(n, 0, BYTES);
	h += 2;
	l = strlen(h);
	o = l - 16;
	while (l > o) {
		sscanf(h + o, "%llx", &n[i]);
		h[o] = 0;
		i++;
		o -= 16;
	}
	sscanf(h, "%llx", &n[i]);
	return 0;
}

uint64_t bigout(uint64_t *n) {
	size_t i = S - 1;
	for (; i < S; i--) {
		printf("%016llx", n[i]);
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv) {
	uint64_t a[S], n, c[S];
	if (argc != 4) {
		exit(1);
	}
	bigh2i(argv[1], a);
	n = atoll(argv[2]);
	switch (argv[3][0]) {
		case 'A':
			add_ui(a, n, c);
			return bigout(c);
		case 'S':
			sub_ui(a, n, c);
			return bigout(c);
		case 'M':
			mul_ui(a, n, c);
			return bigout(c);
		case 'Q':
			quo_ui(a, n, c);
			return bigout(c);
		case 'R':
			rem_ui(a, n, c);
			return bigout(c);
	}
	return 0;
}

