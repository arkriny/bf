/* bfi - brainfuck interpreter */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char tape[65536];
char src[1 << 20];
size_t src_size, head;
int balance, c;

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "bfi: no input file\n");
		return 2;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "bfi: open %s: %s\n", argv[1], strerror(errno));
		return 1;
	}
	src_size = fread(src, 1, sizeof(src), fp);
	fclose(fp);

	for (size_t i = 0; i < src_size; ++i) {
		switch (src[i]) {
			case '>':
				if (head == sizeof(tape) - 1) {
					fprintf(stderr, "bfi: max tape size %zu exceeded\n", sizeof(tape));
					return 1;
				}
				++head;
				break;
			case '<':
				if (head == 0) {
					fprintf(stderr, "bfi: tape pointer underflow\n");
					return 1;
				}
				--head;
				break;
			case '+':
				++tape[head];
				break;
			case '-':
				--tape[head];
				break;
			case '.':
				putchar(tape[head]);
				break;
			case ',':
				if ((c = getchar()) != EOF) {
					tape[head] = c;
				}
				break;
			case '[':
				if (tape[head] != 0) {
					break;
				}
				balance = 1;
				++i;
				for (; src[i] < src_size; ++i) {
					if (src[i] == ']') {
						--balance;
						if (balance == 0) {
							break;
						}
					} else if (src[i] == '[') {
						++balance;
					}
				}
				if (i == src_size) {
					fprintf(stderr, "bfi: unmatched [\n");
					return 1;
				}
				break;
			case ']':
				if (tape[head] == 0) {
					break;
				}
				balance = 1;
				for (; i > 0; --i) {
					if (src[i-1] == '[') {
						--balance;
						if (balance == 0) {
							break;
						}
					} else if (src[i-1] == ']') {
						++balance;
					}
				}
				if (i == 0) {
					fprintf(stderr, "bfi: unmatched ]\n");
					return 1;
				}
				--i;
				break;
		}
	}
	return 0;
}
