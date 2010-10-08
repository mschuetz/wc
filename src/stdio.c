#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
	int fd;
	if (argc < 3) {
		fprintf(stderr, "too few arguments. %s buf_size filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	size_t buf_size = strtol(argv[1], NULL, 10);
	if (buf_size <= 0) {
		perror("invalid buf_size");
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[2], O_RDONLY)) == -1) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	struct stat st;
	if (stat(argv[2], &st) == -1)
		exit(1);

	char buf[buf_size];
	bool inword = false;
	int lines = 0;
	int words = 0;
	ssize_t len = 0;
	while ((len = read(fd, buf, buf_size))) {
		char *s = buf;
		char *last = buf + len-1;
		while (s <= last) {
			switch (*s) {
			case '\n':
				lines++;
			case ' ':
			case '\t':
			case '\f':
			case '\v':
			case '\r':
				if (inword) {
					inword = false;
					words++;
				}
				break;
			default:
				inword = true;

			}
			s++;
		}
	}

	if (inword)
		words++;

	printf("%d %d %d\n", lines, words, (int)st.st_size);
	exit(EXIT_SUCCESS);
}
