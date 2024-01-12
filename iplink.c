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
    // Fork the parent process
    pid_t pid = fork();

    // Exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Create a new session ID
    pid_t sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory to root
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen("iplink.log", "rb");
    if (fp) {
        read_scores(fp);
    }
    FILE *fp = fopen("iplink.log", "wb");
    if (!fp)
        error();
    write_scores(fp);
    // Open a log file for writing
    int log_fd = open("iplink.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        exit(EXIT_FAILURE);
    }

    // Redirect standard output and error to the log file
    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);

    // Close standard file descriptors
    close(STDIN_FILENO);

    // Optional: Close the log file descriptor
    // close(log_fd);
}


int main(void) {
    pthread_t tsthread;

    // Create the daemon
    daemonize();

    // Set the signal handler
    signal(SIGINT, sig_handler);

    while (keep_running) {
        timestamp();
        pthread_create(&tsthread, NULL, iplink, NULL);
        sleep(15);
    }

    return EXIT_SUCCESS;
}
