#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t usr1Happened = 0;

void handler();

int main(void) {
	int i = 0;
	int rv;
	pid_t pid = -1;
	int numChildren = 0;

	struct sigaction sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	printf("Enter the number of children:\n");
	scanf("%d", &numChildren);

	printf("PID = %d: Parent Running..\n", getpid());

	for(i = 0; i < numChildren; i++){
		switch(pid = fork()){
		case 0:
			printf("PID = %d: Child running...\n", getpid());
			while(usr1Happened != 1){}
			printf("PID = %d: Child received USR1.\n", getpid());
			printf("PID = %d: Child Exiting.\n", getpid());
			exit(1);
		case -1:
			perror("fork");
			exit(-1);
		}
	}

	while((pid = wait(&rv)) > 0);

	printf("PID = %d: Children finished, parent exiting..\n", getpid());
	return EXIT_SUCCESS;
}

void handler(){
	usr1Happened = 1;
}
