#include "../include/MIPS_Instruction.h"
#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define TEST_FILE "test_cases.txt"
#define LOG_FILE "test_log.txt"

// Remove trailing newline
void strip_newline(char* str) {
    size_t len = strlen(str);
    if (len && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Capture disassembled instruction into a string instead of stdout
void getDisassembled(char* output) {
    freopen("tmp_disassm.txt", "w", stdout);
    printAssm();
    fflush(stdout);

    // Restore stdout
    freopen("/dev/tty", "a", stdout);

    FILE* temp = fopen("tmp_disassm.txt", "r");
    fgets(output, MAX_LINE, temp);
    strip_newline(output);
    fclose(temp);
}

int main() {
    FILE* file = fopen(TEST_FILE, "r");
    FILE* log = fopen(LOG_FILE, "w");
    if (!file || !log) {
        printf("Failed to open input/output files\n");
        return 1;
    }

    char line[MAX_LINE];
    int total = 0, passed = 0;

    while (fgets(line, sizeof(line), file)) {
        strip_newline(line);

        // Skip blank or comment lines
        if (strlen(line) == 0 || line[0] == '#') continue;

        total++;
        initInstructs();

        // Encode
        parseAssem(line);
        encode();

        if (state != COMPLETE_ENCODE) {
            fprintf(log, "FAIL: %s\n - Encoding failed. Error code: %d\n\n", line, state);
            continue;
        }

        uint32_t original_bin = BIN32;

        // Decode
        parseBin(NULL); // Reset internal state
        BIN32 = original_bin;
        decode();

        if (state != COMPLETE_DECODE) {
            fprintf(log, "FAIL: %s\n - Decoding failed. Error code: %d\n\n", line, state);
            continue;
        }

        // Capture disassembled string
        char disassembled[MAX_LINE];
        getDisassembled(disassembled);

        // Compare original to disassembled (case-insensitive)
        if (strcasecmp(line, disassembled) == 0) {
            fprintf(log, "PASS: %s\n", line);
            passed++;
        } else {
            fprintf(log, "FAIL: %s\n - Got: %s\n\n", line, disassembled);
        }
    }

    fprintf(log, "\nTest Summary: %d passed / %d total\n", passed, total);
    printf("Testing complete! Results written to %s\n", LOG_FILE);

    fclose(file);
    fclose(log);
    return 0;
}