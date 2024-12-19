#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_usage(const char *program_name) {
    printf("Usage: %s <size> <unit> <filename>\n", program_name);
    printf("\n");
    printf("  size     - The size of the file to generate\n");
    printf("  unit     - The unit of size: B (bytes), KB (kilobytes), MB (megabytes), GB (gigabytes)\n");
    printf("  filename - The name of the file to create (with extension)\n");
    printf("\nExample: %s 10 MB output.txt\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    long long size = atoll(argv[1]);
    if (size <= 0) {
        fprintf(stderr, "Error: Size must be a positive number.\n");
        return EXIT_FAILURE;
    }

    char unit = toupper(argv[2][0]);
    switch (unit) {
        case 'B':
            break;
        case 'K':
            size *= 1024;
            break;
        case 'M':
            size *= 1024 * 1024;
            break;
        case 'G':
            size *= 1024 * 1024 * 1024;
            break;
        default:
            fprintf(stderr, "Error: Invalid unit. Use B, KB, MB, or GB.\n");
            return EXIT_FAILURE;
    }

    const char *filename = argv[3];

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    const size_t chunk_size = 1024 * 1024;
    char *buffer = (char *)malloc(chunk_size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    memset(buffer, 0, chunk_size);

    while (size > 0) {
        size_t to_write = (size > chunk_size) ? chunk_size : size;
        if (fwrite(buffer, 1, to_write, file) != to_write) {
            fprintf(stderr, "Error: Write operation failed.\n");
            free(buffer);
            fclose(file);
            return EXIT_FAILURE;
        }
        size -= to_write;
    }

    free(buffer);
    fclose(file);

    printf("File '%s' created successfully.\n", filename);
    return EXIT_SUCCESS;
}
