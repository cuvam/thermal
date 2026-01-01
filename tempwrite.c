#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 360
#define FILE_PATH "/path/to/buffer"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <temp>\n", argv[0]);
        return 1;
    }

    uint8_t temp = (uint8_t)atoi(argv[1]);

    FILE *f = fopen(FILE_PATH, "r+b");
    if (!f) {
        f = fopen(FILE_PATH, "w+b");
        if (!f) {
            perror("fopen");
            return 1;
        }
        uint8_t zeros[BUFFER_SIZE + 4] = {0};
        fwrite(zeros, 1, BUFFER_SIZE + 4, f);
        rewind(f);
    }

    uint32_t index;
    fread(&index, sizeof(uint32_t), 1, f);

    fseek(f, 4 + index, SEEK_SET);
    fwrite(&temp, 1, 1, f);

    index++;
    if (index >= BUFFER_SIZE) {
        index = 0;
    }

    rewind(f);
    fwrite(&index, sizeof(uint32_t), 1, f);

    fclose(f);
    return 0;
}
