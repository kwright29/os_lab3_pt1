#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argx, char **argv) {
	int pipefd[2];
	int pid;

	pipe(pipefd);

	if (pipe(pipefd) == -1){
		fprintf(stderr, "pipe failed");
		return 1;
	}

	char *cat_args[] = {"cat", "scores", NULL};
	char *grep_args[] = {"grep", argv[1], NULL};

	pid = fork();

	if (pid == 0){
		char *sorted[] = {"sort", NULL};
		int pipefd2[2];
		int pid2;

		if (pipe(pipefd2) == -1){
			fprintf(stderr, "pipe failed");
			return 1;
		}

		close(pipefd[1]);

		pid2 = fork();
		if (pid2 == 0) {
      dup2(pipefd2[0], 0);
      close(pipefd2[1]);
      execvp("sort", sorted);
    }  else {
      dup2(pipefd[0],0);
      close(pipefd[1]);
      dup2(pipefd2[1], 1);
      close(pipefd2[0]);
      execvp("grep", grep_args);
    }
	}
	else {
    dup2(pipefd[1], 1);
    close(pipefd[0]);
    execvp("cat", cat_args);

  }
}
