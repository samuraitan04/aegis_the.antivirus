#ifndef AEGIS_H
#define AEGIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

// Function declarations
int scan_file(const char *filepath);
int scan_directory(const char *dirpath);
int check_hash(const char *hash);
void print_banner(void);
void print_help(void);
void monitor_downloads(void);
int install_aegis(void);

// Constants
#define MAX_PATH_LENGTH 4096
#define HASH_BUFFER_SIZE 8192
#define CSV_BUFFER_SIZE 1024

#endif // AEGIS_H 