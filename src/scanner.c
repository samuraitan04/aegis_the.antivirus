#include "../include/aegis.h"
#include <errno.h>

int scan_file(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s: %s\n", filepath, strerror(errno));
        return -1;
    }

    unsigned char buffer[HASH_BUFFER_SIZE];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, HASH_BUFFER_SIZE, file)) != 0) {
        SHA256_Update(&sha256, buffer, bytes_read);
    }

    SHA256_Final(hash, &sha256);
    fclose(file);

    // Convert hash to hex string
    char hash_string[65];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_string[i * 2], "%02x", hash[i]);
    }
    hash_string[64] = '\0';

    return check_hash(hash_string);
}

int scan_directory(const char *dirpath) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        fprintf(stderr, "Error opening directory %s: %s\n", dirpath, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    int malware_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            char full_path[MAX_PATH_LENGTH];
            snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", dirpath, entry->d_name);

            int result = scan_file(full_path);
            if (result == 1) {
                printf("%s is not safe\n", full_path);
                malware_count++;
            } else if (result == 0) {
                printf("%s is safe\n", full_path);
            }
        } else if (entry->d_type == DT_DIR && 
                  strcmp(entry->d_name, ".") != 0 && 
                  strcmp(entry->d_name, "..") != 0) {
            char full_path[MAX_PATH_LENGTH];
            snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", dirpath, entry->d_name);
            scan_directory(full_path);
        }
    }

    closedir(dir);
    return malware_count;
}

int check_hash(const char *hash) {
    FILE *csv_file = fopen("full.csv", "r");
    if (!csv_file) {
        fprintf(stderr, "Error opening CSV file: %s\n", strerror(errno));
        return -1;
    }

    char line[CSV_BUFFER_SIZE];
    char search_string[70];
    snprintf(search_string, sizeof(search_string), " \"%s\"", hash);

    while (fgets(line, CSV_BUFFER_SIZE, csv_file)) {
        if (strstr(line, search_string)) {
            fclose(csv_file);
            return 1; // Malware found
        }
    }

    fclose(csv_file);
    return 0; // Safe
} 