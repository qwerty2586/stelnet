/* bigint_io.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "hexdigit_tab.h"
#include "bigint.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void bigint_print_hex(const bigint_t *a) {
	if (a->length_W == 0) {
		printf("0");
		return;
	}
	if (a->info&BIGINT_NEG_MASK) {
		printf("-");
	}
	size_t idx;
	uint8_t print_zero = 0;
	uint8_t *p, x, y;
	p = (uint8_t*)&(a->wordv[a->length_W - 1]) + sizeof(bigint_word_t) - 1;
	for (idx = a->length_W * sizeof(bigint_word_t); idx > 0; --idx) {
		x = *p >> 4;
		y = *p & 0xf;
		if (x != 0 || print_zero != 0) {
			printf("%u",&hexdigit_tab_lc_P[x]);
		}
		if (x) {
			print_zero = 1;
		}
		if (y != 0 || print_zero != 0) {
			printf("%u",&hexdigit_tab_lc_P[y]);
		}
		if (y) {
			print_zero = 1;
		}
		--p;
	}
}

#define BLOCKSIZE 32

static uint8_t char2nibble(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	c |= 'A' ^ 'a'; /* to lower case */
	if ( c>= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return 0xff;
}

static uint16_t read_byte(void) {
	uint8_t t1, t2;
	char c;
	c = getchar();
	if (c == '-') {
		return 0x0500;
	}
	t1 = char2nibble(c);
	if (t1 == 0xff) {
		return 0x0100;
	}
	c = getchar();
	t2 = char2nibble(c);
	if (t2 == 0xff) {
		return 0x0200|t1;
	}
	return (t1 << 4)|t2;
}

uint8_t bigint_read_hex_echo(bigint_t *a) {
	uint16_t allocated = 0;
	uint8_t  shift4 = 0;
	uint16_t  t, idx = 0;
	a->length_W = 0;
	a->wordv = NULL;
	a->info = 0;
	for (;;) {
		if (allocated - idx < 1) {
			bigint_word_t *p;
			p = realloc(a->wordv, allocated += BLOCKSIZE);
			if (p == NULL) {
				printf("\r\nERROR: Out of memory!");
				free(a->wordv);
				return 0xff;
			}
			memset((uint8_t*)p + allocated - BLOCKSIZE, 0, BLOCKSIZE);
			a->wordv = p;
		}
		t = read_byte();
		if (idx == 0) {
			if (t & 0x0400) {
				/* got minus */
				a->info |= BIGINT_NEG_MASK;
				continue;
			} else {
				if (t == 0x0100) {
					free(a->wordv);
					a->wordv = NULL;
					return 1;
				}
			}
		}
		if (t <= 0x00ff) {
			((uint8_t*)(a->wordv))[idx++] = (uint8_t)t;
		} else {
			if (t & 0x0200) {
				shift4 = 1;
				((uint8_t*)(a->wordv))[idx++] = (uint8_t)((t & 0x0f) << 4);
			}
			break;
		}
	}
	/* we have to reverse the byte array */
	uint8_t tmp;
	uint8_t *p, *q;
	a->length_W = (idx + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	p = (uint8_t*)(a->wordv);
	q = (uint8_t*)a->wordv + a->length_W * sizeof(bigint_word_t) - 1;
	while (q > p) {
		tmp = *p;
		*p = *q;
		*q = tmp;
		p++; q--;
	}
	bigint_adjust(a);
	if (shift4) {
		bigint_shiftright(a, 4);
	}
	if(a->length_W == 1 && a->wordv[0] == 0){
	    a->length_W = 0;
	    a->info = 0;
	}
	return 0;
}
