#include "../include/aegis.h"
#include <getopt.h>

void print_banner(void) {
    printf("\n");
    printf("     █████╗ ███████╗ ██████╗ ██╗███████╗\n");
    printf("    ██╔══██╗██╔════╝██╔════╝ ██║██╔════╝\n");
    printf("    ███████║█████╗  ██║  ███╗██║███████╗\n");
    printf("    ██╔══██║██╔══╝  ██║   ██║██║╚════██║\n");
    printf("    ██║  ██║███████╗╚██████╔╝██║███████║\n");
    printf("    ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝╚══════╝\n");
    printf("   ────────────────────────────────────────\n");
    printf("   Basic Malware Detection System By Tanush\n");
    printf("   ────────────────────────────────────────\n\n");
}

void print_help(void) {
    printf("\nUsage: aegis [options]\n\n");
    printf("Options:\n");
    printf("  -h      Print this help menu\n");
    printf("  -f      Scan all files in a folder\n");
    printf("          Usage: -f [full path to folder]\n");
    printf("  -d      Background mode and monitors Downloads directory\n");
    printf("  -i      Install Aegis\n");
}

int main(int argc, char *argv[]) {
    int opt;
    char *folder_path = NULL;

    while ((opt = getopt(argc, argv, "hf:di")) != -1) {
        switch (opt) {
            case 'h':
                print_banner();
                print_help();
                break;
            case 'f':
                folder_path = optarg;
                print_banner();
                scan_directory(folder_path);
                break;
            case 'd':
                print_banner();
                printf("Aegis is now running in daemon mode\n");
                monitor_downloads();
                break;
            case 'i':
                if (install_aegis() == 0) {
                    printf("Installation successful\n");
                } else {
                    printf("Installation failed\n");
                }
                break;
            default:
                print_help();
                return 1;
        }
    }

    return 0;
} 