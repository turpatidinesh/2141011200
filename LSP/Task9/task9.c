#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
int main() {
    DIR *dp;
    struct dirent *entry;
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }

    printf("Current Working Directory: %s\n", cwd);

    dp = opendir(".");
    if (dp == NULL) {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    printf("Contents of current directory:\n");
    while ((entry = readdir(dp)) != NULL) {
        printf(" - %s\n", entry->d_name);
    }
    closedir(dp);

    if (chdir("/tmp") != 0) {
        perror("chdir to /tmp failed");
        exit(EXIT_FAILURE);
    }
    printf("Changed directory to /tmp\n");

    if (mkdir("testdir", 0755) != 0) {
        if (errno == EEXIST) {
            printf("Directory 'testdir' already exists.\n");
        } else {
            perror("mkdir failed");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Directory 'testdir' created with 0755 permissions.\n");
    }

    return 0;
}
