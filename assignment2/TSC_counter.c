#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <x86intrin.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execlp("ping", "ping", "-c", "1", "8.8.8.8", (char *)NULL);
        perror("execlp");
        _exit(1);
    }

    uint64_t count = 0;

    while (1) {
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == -1) {
            perror("waitpid");
            return 1;
        }

        if (result == pid)
            break;

        (void)__rdtsc();
        count++;
    }

    printf("TSC reads during ping: %" PRIu64 "\n", count);
    return 0;
}