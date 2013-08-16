/*
 *   Empirical measurements of characteristics of
 *   the d-left counting bloom filter.
 *
 *   The program will read integers from stdin and
 *   output the number of unique elements.
 */


#include "bloom.h"
#include <unistd.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
	size_t n = 1000000;
	size_t m = 8;
	int	   index = 0;
	int opt;

	while (true) {
		opt = getopt(argc, argv, "n:m:i:");

		if (opt == -1) break;

		switch (opt) {
			case 'n':
				n = (size_t) strtoul(optarg, NULL, 10);
				break;

			case 'm':
				m = (size_t) strtoul(optarg, NULL, 10);
				break;

			case 'i':
				index = (size_t) strtoul(optarg, NULL, 10);
				break;

			case '?':
				return EXIT_FAILURE;

			default:
				abort();
		}
	}

	bloom_t* B = bloom_alloc(n, m);

	char line[512];

	kmer_t x;
	size_t count = 0, unique_count = 0;
	FILE *fd = NULL;
	char *file = NULL;
	char *files[] = {"random.txt","random1.txt","random2.txt","random3.txt","random4.txt"};

	/*
	 * char files[16] = "random.txt";
	 * if(index > 0)
	 *		snprintf(files, sizeof(files), "random%d.txt", index);
	 */

	if(index)
		file = files[index];
	else
		file = files[0];
	fd = fopen(file, "r");
	if(fd == NULL)
		fprintf(stderr, "fopen error\n");

	while (fgets(line, sizeof(line), fd)) {
		x = (kmer_t) strtoul(line, NULL, 10);
		if (bloom_inc(B, x) == 1) 
			++unique_count;
		else
			printf("%s", line);
		++count;
	}
	printf("\n");
	printf("%zu\t%zu\n", unique_count, count);

	rewind(fd);
	while (fgets(line, sizeof(line), fd)) {
		x = (kmer_t) strtoul(line, NULL, 10);
		//bloom_del(B, x);
		if(bloom_ldec(B, x) == 1)
			--unique_count;
		--count;
	}

	fclose(fd);
	fd = NULL;

	bloom_free(B);

	printf("%zu\t%zu\n", unique_count, count);

	return 0;
}

