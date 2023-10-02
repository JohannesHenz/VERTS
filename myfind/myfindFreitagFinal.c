#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <ctype.h>
#include <getopt.h>

// Function to search for a file in a directory and its subdirectories
void search_file(const char *search_path, const char *filename, bool recursive, bool case_insensitive) {
    DIR *dir;
    struct dirent *direntry; 
    struct stat file_stat;

    if ((dir = opendir(search_path)) == NULL) { //double duty: do opendir/write it into dir and check whether it was successful
                                                // note opendir is capable of opening both relative and absolute paths
        perror("opendir");
        exit(EXIT_FAILURE);
    }
 
    while ((direntry = readdir(dir)) != NULL) {
        char entry_path[1024]; //neues string anlegen fuer den pfad
        snprintf(entry_path, sizeof(entry_path), "%s/%s", search_path, direntry->d_name); //writes searchpath followed by the new directory into entry_path

        if (stat(entry_path, &file_stat) < 0) { //lstat weil entry_path ein link is.
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode) && recursive && strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) { //S_ISDIR reads the int returned by file_stat-st_mode and tells us whether its a directory or not. also we gotta prevent acting on the current directory (.) and parent directory (..)
            // Recursively search subdirectories
            search_file(entry_path, filename, recursive, case_insensitive); //recursion: same function again, but with searchpath as searchpath + "/directoryname"

        } else if (S_ISREG(file_stat.st_mode)) { //since its a regular file, we stop here even in case of a recursion. Plus this check also eliminates the possibility of parent/currect directory
            // Check if the filename matches
            if(case_insensitive){
                if(strcasecmp(direntry->d_name, filename)==0)
                printf("%d: %s: %s\n", getpid(), filename, entry_path);
            }
            else{
                if(strcmp(direntry->d_name, filename)==0)
                printf("%d: %s: %s\n", getpid(), filename, entry_path);
            }
        }
    }
    closedir(dir);
}


int main(int argc, char *argv[]) {
    int c;
    //char *dateiname;
    char *programm_name;
    int case_insensitive = 0;
    int recursive = 0;
    char *search_path;

    programm_name = argv[0];

    while ((c = getopt(argc, argv, "iRk:")) != EOF)
    {
        switch (c){
        case 'i':
            case_insensitive++;
            break;
        case 'R':
            recursive++;
            break;
        case 'k': //for some reason it would never register the last option specified in the third argument of getopt: fixed by adding an unused letter that redirects to the same error as default does
            fprintf(stderr, "%s error: Unknown option.\n", programm_name);
            fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n", argv[0]);
            exit(1);
        default:
            fprintf(stderr, "%s error: Unknown option.\n", programm_name);
            fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n", argv[0]);
            exit(1);
        }
    }
    if ((recursive > 1) || (case_insensitive > 1))
    {
        fprintf(stderr, "%s Fehler: Optionen wurden mehrfach verwendet.\n", programm_name);
        exit(1);
    }

    // Check if we have at least 2 argument that arent option (search_path and filename)
    if (optind + 2 > argc) {
        fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n", argv[0]);
        exit(1);
    }
    //check if the first argument looks like a path, checking whether the first 2 chars are "./"
    if (strncmp(argv[optind], "./", 2) != 0) {
        fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n", argv[0]);
        exit(1);
    }
    search_path = argv[optind];
    ++optind;
    while (optind < argc) {
        char *filename = argv[optind]; 
        ++optind;
        pid_t child_pid = fork(); //create a fork for the argument
        if (child_pid == -1) { //case: error
            perror("fork");
            exit(1);
        }
        if (child_pid == 0) { //case: we're successfully within a child process
            search_file(search_path, filename, recursive, case_insensitive);
            exit(EXIT_SUCCESS);
        }
        //since we have no case for the pid_t to be != 0 or 1 this while actually doesn't do anything with the parent process
    }

    // Wait for all child processes to finish
    int status;
    for (int j = 0; j < argc - optind - 1; j++) { // -1 weil der pfad selbst keinen child process verursacht hat
        wait(&status); //prevents zombies
    }

    return 0;
}
