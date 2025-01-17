#include "check.h"

uev_t timer, file;
int counter = 10;

static void cb(uev_t *w, void *arg, int events)
{
	if (UEV_ERROR == events)
		fprintf(stderr, "timer watcher failed, ignoring ...\n");

	printf("cb counter:%d\n", counter);

	if (counter--)
		return;

	uev_exit(w->ctx);
}

int main(void)
{
	uev_ctx_t ctx;
	FILE *fp;

	uev_init(&ctx);
	uev_timer_init(&ctx, &timer, cb, NULL, 1000, 100);

	uev_dump(&ctx);

	fp = fopen("/dev/one", "r");
	if (fp)
		uev_io_init(&ctx, &file, cb, NULL, fileno(fp), UEV_READ);
	else
		printf("/dev/one not exist!\n");

	uev_run(&ctx, 0);

	uev_exit(&ctx); /* Should not hang event loop, troglobit/uftpd#16 */

	return 0;
}
