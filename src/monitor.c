#include "../include/aegis.h"
#include <sys/inotify.h>
#include <limits.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

void *monitor_thread(void *arg) {
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return NULL;
    }

    const char *downloads_path = getenv("HOME");
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s/Downloads", downloads_path);

    int wd = inotify_add_watch(fd, full_path, IN_CREATE | IN_MOVED_TO);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        return NULL;
    }

    char buffer[BUF_LEN];
    while (1) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO) {
                    char file_path[PATH_MAX];
                    snprintf(file_path, sizeof(file_path), "%s/%s", full_path, event->name);
                    
                    printf("New file detected: %s\n", file_path);
                    int result = scan_file(file_path);
                    if (result == 1) {
                        printf("WARNING: %s is not safe!\n", file_path);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    return NULL;
}

void monitor_downloads(void) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, monitor_thread, NULL) != 0) {
        perror("pthread_create");
        return;
    }
    pthread_detach(thread);
} 