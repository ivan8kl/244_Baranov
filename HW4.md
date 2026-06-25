#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} Buffer;

void init_buffer(Buffer *b) {
    b->cap = 128;
    b->data = malloc(b->cap);
    if (b->data) b->data[0] = '\0';
    b->len = 0;
}

void append(Buffer *b, const char *str, size_t len) {
    if (b->len + len + 1 > b->cap) {
        size_t new_cap = b->cap;
        while (new_cap < b->len + len + 1) {
            new_cap *= 2;
        }
        char *new_data = realloc(b->data, new_cap);
        if (!new_data) {
            free(b->data);
            exit(1);
        }
        b->data = new_data;
        b->cap = new_cap;
    }
    memcpy(b->data + b->len, str, len);
    b->len += len;
    b->data[b->len] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regex> <text> <replacement>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *text = argv[2];
    const char *replacement = argv[3];
    size_t text_len = strlen(text);
    size_t repl_len = strlen(replacement);

    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        char errbuf[256];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Could not compile regex: %s\n", errbuf);
        return 1;
    }

    Buffer buf;
    init_buffer(&buf);

    size_t current_pos = 0;
    regmatch_t match;

    while (current_pos <= text_len) {
        ret = regexec(&regex, text + current_pos, 1, &match, 0);
        if (ret != 0) {
            append(&buf, text + current_pos, text_len - current_pos);
            break;
        }

        append(&buf, text + current_pos, match.rm_so);
        append(&buf, replacement, repl_len);
        
        if (match.rm_so == match.rm_eo) {
            if (current_pos < text_len) {
                append(&buf, text + current_pos, 1);
                current_pos++;
            } else {
                break;
            }
        } else {
            current_pos += match.rm_eo;
        }
    }

    printf("%s\n", buf.data);

    free(buf.data);
    regfree(&regex);

    return 0;
}