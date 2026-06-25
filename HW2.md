# HW2

## Task1

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 4) {
        write(2, "Usage: prog <file> <I> <N>\n", 26);
        return 1;
    }

    int I = atoi(argv[2]);
    int N = atoi(argv[3]);

    if (I < 1 || N < 1) {
        write(2, "I and N must be >= 1\n", 20);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(2, "open error\n", 11);
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        write(2, "fstat error\n", 12);
        close(fd);
        return 1;
    }

    size_t buf_size = BUF_SIZE;
    if (st.st_size > 0 && st.st_size < BUF_SIZE) {
        buf_size = st.st_size;
    }

    char *buf = malloc(buf_size);
    if (!buf) {
        close(fd);
        return 1;
    }

    int cur_line = 1;
    int lines_left = N;
    ssize_t bytes_read;
    int done = 0;

    while (!done && (bytes_read = read(fd, buf, buf_size)) > 0) {
        int writing = 0;
        size_t write_start = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            if (cur_line < I) {
                if (buf[i] == '\n') {
                    cur_line++;
                }
            } else if (cur_line == I) {
                if (!writing) {
                    writing = 1;
                    write_start = i;
                }
                if (buf[i] == '\n') {
                    lines_left--;
                    if (lines_left == 0) {
                        write(1, buf + write_start, i - write_start + 1);
                        done = 1;
                        break;
                    }
                }
            }
        }
        if (!done && writing) {
            write(1, buf + write_start, bytes_read - write_start);
        }
    }

    free(buf);
    close(fd);
    return 0;
}

---

## Task2

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "Usage: prog <file>\n", 19);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(2, "open error\n", 11);
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        write(2, "fstat error\n", 12);
        close(fd);
        return 1;
    }

    off_t mid = st.st_size / 2;
    lseek(fd, mid, SEEK_SET);

    char buf[BUF_SIZE];
    ssize_t bytes_read;
    int done = 0;

    while (!done && (bytes_read = read(fd, buf, BUF_SIZE)) > 0) {
        int found = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            if (buf[i] == '\n') {
                write(1, buf, i + 1);
                done = 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            write(1, buf, bytes_read);
        }
    }

    close(fd);
    return 0;
}
