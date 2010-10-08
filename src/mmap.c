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

int main(int argc, char **argv) {
	struct stat st;
	if (stat(argv[1], &st) == -1)
		exit(1);

	int fd;
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		exit(1);
	char *file = mmap((caddr_t) 0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	printf("file = %p\n", file);
	if (file == (void *) -1)
		exit(1);

	madvise(file, st.st_size, MADV_SEQUENTIAL | MADV_WILLNEED);
	char *s = file;
	char *last = file + st.st_size-1;

	bool inword = false;
	int lines = 0;
	int words = 0;
	//      do {
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
	//      } while(__builtin_expect(++s <= end, 1));
	printf("%d %d %d\n", lines, words, (int) st.st_size);
	return EXIT_SUCCESS;
}
