#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t usr1Happened = 0;

void handler();

int main(void) {

	struct sigaction sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	printf("PID = %d: Running..\n", getpid());

	while(usr1Happened != 1){}

	printf("PID = %d: Received USR1.\n", getpid());
	printf("PID = %d: Exiting.\n", getpid());

	return EXIT_SUCCESS;
}

void handler(){
	usr1Happened = 1;
}
