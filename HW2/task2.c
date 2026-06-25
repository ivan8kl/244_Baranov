#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) return 1;
    
    struct stat st;
    if (fstat(fd, &st) < 0) { close(fd); return 1; }
    
    lseek(fd, st.st_size / 2, SEEK_SET);
    
    char buf[4096];
    ssize_t bytes_read;
    int done = 0;
    
    while (!done && (bytes_read = read(fd, buf, 4096)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buf[i] == '\n') {
                write(1, buf, i + 1);
                done = 1;
                break;
            }
        }
        if (!done) write(1, buf, bytes_read);
    }
    
    close(fd);
    return 0;
}