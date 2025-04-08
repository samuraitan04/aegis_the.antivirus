#include "../include/aegis.h"
#include <sys/stat.h>

int install_aegis(void) {
    // Create installation directory
    if (mkdir("/opt/aegis", 0755) != 0 && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }

    // Copy executable to /opt/aegis
    char current_path[PATH_MAX];
    if (getcwd(current_path, sizeof(current_path)) == NULL) {
        perror("getcwd");
        return -1;
    }

    char src_path[PATH_MAX];
    char dest_path[PATH_MAX];
    snprintf(src_path, sizeof(src_path), "%s/bin/aegis", current_path);
    snprintf(dest_path, sizeof(dest_path), "/opt/aegis/aegis");

    // Copy the executable
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        perror("fopen src");
        return -1;
    }

    FILE *dest = fopen(dest_path, "wb");
    if (!dest) {
        perror("fopen dest");
        fclose(src);
        return -1;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) != bytes) {
            perror("fwrite");
            fclose(src);
            fclose(dest);
            return -1;
        }
    }

    fclose(src);
    fclose(dest);

    // Set executable permissions
    if (chmod(dest_path, 0755) != 0) {
        perror("chmod");
        return -1;
    }

    // Add to PATH
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "HOME environment variable not set\n");
        return -1;
    }

    char shell_rc[PATH_MAX];
    snprintf(shell_rc, sizeof(shell_rc), "%s/.zshrc", home);

    FILE *rc_file = fopen(shell_rc, "a");
    if (!rc_file) {
        perror("fopen .zshrc");
        return -1;
    }

    fprintf(rc_file, "\nexport PATH=\"/opt/aegis:$PATH\"\n");
    fclose(rc_file);

    return 0;
} 