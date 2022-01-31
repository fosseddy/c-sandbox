#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void print_dir_files(char *path, size_t level)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }

    errno = 0;
    struct dirent *item;

    // padding
    for (size_t i = 0; i < level * 4; ++i) printf(" ");
    printf("%s\n", path);

    while ((item = readdir(dir)) != NULL) {
        char *name = item->d_name;
        unsigned char type = item->d_type;

        if (strcmp(name, ".") == 0 ||
                strcmp(name, "..") == 0 ||
                strcmp(name, ".git") == 0) continue;

        if (type == DT_DIR) {
            size_t size = strlen(path) + strlen(name) + 2;
            char newpath[size];
            sprintf(newpath, "%s/%s", path, name);
            print_dir_files(newpath, level + 1);
        } else {
            // padding
            for (size_t i = 0; i < (level + 1) * 4; ++i) printf(" ");
            printf("%s\n", name);
        }
    }

    if (errno != 0) {
        fprintf(stderr, "%s\n", strerror(errno));
    }

    closedir(dir);
}

int main(void)
{
    print_dir_files("/home/fosseddy/programming/c-sandbox", 0);
    return 0;
}
