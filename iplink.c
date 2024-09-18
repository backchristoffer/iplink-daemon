#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

void timestamp() {
    time_t t;
    time(&t);
    printf("\n%s\n", ctime(&t));
    fflush(stdout);
}

void *iplink(void *arg) {
    int r = system("ip -s link");
    return NULL;
}

void daemonize() {
    pid_t pid = fork();
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    /// Need to debug this part - why is no .log created?
    FILE *fptr;
    fptr = fopen("/var/log/iplink.log", "rb+");
    if(fptr == NULL)
    {
        fptr = fopen("/var/log/iplink.log", "wb");
    }
    int log_fd = open("/var/log/iplink.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        exit(EXIT_FAILURE);
    }


    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);
    close(STDIN_FILENO);
}

int main(void) {
    pthread_t tsthread;
    daemonize();
    signal(SIGINT, sig_handler);
    while (keep_running) {
        timestamp();
        pthread_create(&tsthread, NULL, iplink, NULL);
        sleep(15);
    }
    return EXIT_SUCCESS;
}
