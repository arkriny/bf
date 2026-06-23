/* bf2c - brainfuck to C compiler */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char src[1 << 20];
size_t src_size;

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "bf2c: no input file\n");
		return 2;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "bf2c: open %s: %s\n", argv[1], strerror(errno));
		return 1;
	}
	src_size = fread(src, 1, sizeof(src), fp);
	fclose(fp);

	printf("#include <stdio.h>\n");
	printf("unsigned char t[65536], *h=t;\n");
	printf("int c;\n");
	printf("int main(void) {\n");
	for (size_t i = 0; i < src_size; ++i) {
		switch (src[i]) {
			case '>': printf("++h;\n"); break;
			case '<': printf("--h;\n"); break;
			case '+': printf("++*h;\n"); break;
			case '-': printf("--*h;\n"); break;
			case '.': printf("putchar(*h);\n"); break;
			case ',': printf("if ((c = getchar()) != EOF) *h = c;\n"); break;
			case '[': printf("while (*h != 0) {\n"); break;
			case ']': printf("}\n"); break;
		}
	}
	printf("return 0;\n");
	printf("}\n");
	return 0;
}
