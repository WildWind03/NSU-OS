#define LINE_COUNTS 10

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/fcntl.h>

char sem1Name[] = "/sem1";
char sem2Name[] = "/sem2";

int main() {
	sem_unlink(sem1Name);
	sem_unlink(sem2Name);
	sem_t *sem1 = sem_open(sem1Name, O_CREAT | O_EXCL, 0666, 0);
	sem_t *sem2 = sem_open(sem2Name, O_CREAT | O_EXCL, 0666, 1);

	if (NULL == sem1 || NULL == sem2) {
			perror("Can not create semaphore");
			return -1;
	}

	int result = fork();

	if (result < 0) {
		perror ("Error while fork");
		return -1;
	}

	if (0 != result) {
		int i;
		for (i = 0; i < LINE_COUNTS; ++i) {
			sem_wait(sem1);
			printf("Main Thread - %d\n", i);
			sem_post(sem2);
		}
	}

	else {
		int i;
		for (i = 0; i < LINE_COUNTS; ++i) {
			sem_wait(sem2);
			printf("The second thread - %d\n", i);
			sem_post(sem1);
		}
	}

		sem_close(sem1);
		sem_close(sem2);

	return 0;
}
