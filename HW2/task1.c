#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 4) return 1;
    
    int I = atoi(argv[2]);
    int N = atoi(argv[3]);
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) return 1;
    
    struct stat st;
    fstat(fd, &st);
    
    size_t buf_size = 4096;
    if (st.st_size > 0 && st.st_size < 4096) buf_size = st.st_size;
    
    char *buf = malloc(buf_size);
    if (!buf) { close(fd); return 1; }
    
    int cur_line = 1;
    int lines_left = N;
    ssize_t bytes_read;
    int done = 0;
    
    while (!done && (bytes_read = read(fd, buf, buf_size)) > 0) {
        int writing = 0;
        size_t write_start = 0;
        
        for (size_t i = 0; i < (size_t)bytes_read; i++) {
            if (cur_line < I) {
                if (buf[i] == '\n') cur_line++;
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