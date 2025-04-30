#include <stdio.h>
#include <stdlib.h>
int main() {
    FILE *fp;
    char buffer[100];
    fp = fopen("data.txt", "w+");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    fputs("Hello, File Handling in C!", fp);
    rewind(fp);
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("File contents: %s\n", buffer);
    } else {
        printf("Could not read from file.\n");
    }
    fclose(fp);
    return 0;
}
