#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 360
#define FILE_PATH "/path/to/buffer"
#define BAR_WIDTH 40

int main(void) {
    FILE *f = fopen(FILE_PATH, "rb");
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

    uint8_t buffer[BUFFER_SIZE];
    fread(buffer, 1, BUFFER_SIZE, f);
    fclose(f);

    int buckets[27] = {0};
    int valid = 0, min = 255, max = 0;
    long sum = 0;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        uint8_t t = buffer[i];
        if (t > 0) {
            valid++;
            sum += t;
            if (t < min) min = t;
            if (t > max) max = t;
            int b;
            if (t < 40) b = 0;
            else if (t >= 90) b = 26;
            else b = (t - 40) / 2 + 1;
            buckets[b]++;
        }
    }

    if (valid == 0) {
        printf("No data in buffer.\n");
        return 0;
    }

    printf("Thermal Summary (6m window)\n");
    printf("Min: %dC  Max: %dC  Avg: %liC\n", min, max, sum / valid);
    printf("Samples: %d/%d\n\n", valid, BUFFER_SIZE);

    int max_bucket = 0;
    for (int i = 0; i < 27; i++) {
        if (buckets[i] > max_bucket) max_bucket = buckets[i];
    }

    for (int i = 0; i < 27; i++) {
        if (i == 0)
            printf("   <40C |");
        else if (i < 26)
            printf("%3d-%2dC |", 40 + (i - 1) * 2, 40 + (i - 1) * 2 + 1);
        else
            printf("   90+C |");

        int bars = max_bucket ? buckets[i] * BAR_WIDTH / max_bucket : 0;
        for (int j = 0; j < bars; j++) putchar('#');
        if (buckets[i] > 0) printf(" %d", buckets[i]);
        putchar('\n');
    }

    return 0;
}
