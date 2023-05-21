#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *ThreadHandler(void *__x) {
	int fd = (int)__x;
	for (;;) {
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET((int)fd, &rfds);

		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int n = select(fd+1, &rfds, 0, 0, &tv);

		if (n == -1) {
			printf("Error in select\n");
			pthread_exit (0);
		}

		if (!n) continue;

		if (FD_ISSET(fd, &rfds)) {
			char buf[1024];
			int n = read(fd, buf, sizeof(buf) - 1);
			if (n == -1) {
				printf("Error in read\n");
				pthread_exit (0);
			}
	
			buf[n] = '\0';
			printf("Got data: %s", buf);
		}
	}
}

int main(void) {
	char buf[8];
	int r, w;
	int fds[2];
	pipe(fds);
	w = fds[1];
	r = fds[0];

	pthread_t thread;
	pthread_create(&thread, 0, ThreadHandler, (void *)r);
	for (;;) {
		char buf[1024];
		fgets(buf, sizeof(buf) - 1, stdin);
		write(w, buf, strlen(buf));
	}

	return (0);
}
