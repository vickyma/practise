#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <bfilter.h>
#include <math_rand_prot.h>

uint64_t u, v, w;
#define myrand()  RAND_NR_NEXT(u, v, w)

const char *usage =
	"%s [ins] [get]\n";

volatile long counter = 0;

static void sig_alarm_handler(int signum)
{
	printf("%ld per sec signum=%d\n", counter, signum);
	counter = 0;
	alarm(1);
}

void register_sig_handler()
{
	struct sigaction sigact;

	sigact.sa_handler = sig_alarm_handler;
	sigact.sa_flags = 0;
	if (sigaction(SIGALRM, &sigact, NULL)) {
		perror("sigaction");
		exit(-1);
	}
	alarm(1);
}

void constant_insert(long ins, long get, long nbit)
{
	long	t;
	FILE	*sfd, *gfd;
	char	buf[32];
	int		ret = 0, hit = 0, nohit = 0;

	struct bloom_filter bf;

	if (bfilter_create(&bf, nbit, ins)) {
		fprintf(stderr, "alloc failed\n");
		return;
	}
	bfilter_zero(&bf);

	sfd = fopen("random.txt", "w+");
	if(sfd == NULL)
		fprintf(stderr, "open file error\n");

	for (t = 0; t < ins; t++) {
		uint64_t num = myrand();

		snprintf(buf, sizeof(buf), "%lu%s", num, "\r\n");
		fwrite(buf, strlen(buf), 1, sfd);
		bfilter_set(&bf, &num, sizeof(num));
		++counter;
	}

	printf("insertion done\n");

	gfd = fopen("random1.txt", "w+");
	if(gfd == NULL)
		fprintf(stderr, "open file error\n");

	for (t = 0; t < get; t++) {
		uint64_t num = myrand();
		snprintf(buf, sizeof(buf), "%lu%s", num, "\r\n");
		fwrite(buf, strlen(buf), 1, gfd);

		ret = bfilter_get(&bf, &num, sizeof(num));
		if(ret == 1)
			hit++;
		else
			nohit++;

		++counter;
	}

	fclose(sfd);
	fclose(gfd);
	sfd = NULL;
	gfd = NULL;

	printf("all done hit=%d, nohit=%d  pfrate=%5lf%%\n", hit, nohit, (double)100 * hit/(hit+nohit));
}

int main(int argc, char *argv[])
{
	int		i = 0;
	long nbit = 4;
	long ins = 5000000;
	long get = 50000000;
	uint64_t seed = time(NULL);

	if (argc > 1)
		ins = atol(argv[1]);
	if (argc > 2)
		get = atol(argv[2]);
	if (argc > 3)
		nbit = atol(argv[3]);

	RAND_NR_INIT(u, v, w, seed);

	register_sig_handler();

	for(i=0;i<10;i++)
		constant_insert(ins, get, ins*nbit);

	printf("passed\n");

	return 0;
}
