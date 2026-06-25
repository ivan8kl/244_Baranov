#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void append(char **buf, size_t *len, size_t *cap, const char *str, size_t str_len) {
    while (*len + str_len + 1 > *cap) {
        *cap *= 2;
        *buf = realloc(*buf, *cap);
    }
    memcpy(*buf + *len, str, str_len);
    *len += str_len;
    (*buf)[*len] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regex> <text> <replacement>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *text = argv[2];
    const char *replacement = argv[3];

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Invalid regex\n");
        return 1;
    }

    size_t cap = strlen(text) + 1;
    size_t len = 0;
    char *buf = malloc(cap);
    buf[0] = '\0';

    size_t pos = 0;
    regmatch_t match;

    while (pos <= strlen(text)) {
        if (regexec(&regex, text + pos, 1, &match, 0) != 0) {
            append(&buf, &len, &cap, text + pos, strlen(text) - pos);
            break;
        }

        append(&buf, &len, &cap, text + pos, match.rm_so);
        append(&buf, &len, &cap, replacement, strlen(replacement));

        if (match.rm_so == match.rm_eo) {
            if (pos < strlen(text)) {
                append(&buf, &len, &cap, text + pos, 1);
                pos++;
            } else {
                break;
            }
        } else {
            pos += match.rm_eo;
        }
    }

    printf("%s\n", buf);
    free(buf);
    regfree(&regex);
    return 0;
}