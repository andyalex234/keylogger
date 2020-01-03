#include<stdlib.h>
#include<stdio.h>

#include "find_event_file.h"


#define INPUT_DIR "/dev/input/"

static int is_char_device(const struct dirent *file){
    struct stat filestat;
    char filename[512];
    int err;

    snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, file->d_name);

    err = stat(filename, &filestat);
    if(err){
        return 0;
    }

    return S_ISCHR(filestat.st_mode);
}

char *get_keyboard_event_file(void){
    char *keyboard_file = NULL;
    int num, i;
    struct dirent **event_files;
    char filename[512];

    num = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);
    if(num < 0){
        return NULL;
    }
    else {
        for(i = 0; i < num; ++i){
            int32_t event_bitmap = 0;
            int fd;

            // for testing if the keyboard supports keys A,B,C,Z
            int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z; 

            // writes the event files one by one to a special char array --filename
            snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, event_files[i]->d_name);
            fd = open(filename, O_RDONLY); // open the files in readonly mode

            if(fd == -1){
                // if there is an error happening during reading the file
                // throw an error
                perror("open");
                continue;
            }
            ioctl(fd, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
            if((EV_KEY & event_bitmap) == EV_KEY){
                // The device acts like a keyboard
                ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
                // checks the intersection of our keyboard bitmap and event bitmap
                // if the event bitmap contains A,B,C,D keys -- green light
                if((kbd_bitmap & event_bitmap) == kbd_bitmap){
                    // The device supports A, B, C, Z keys, so it probably is a keyboard
                    keyboard_file = strdup(filename);
                    close(fd);
                    break;
                }

            }
            close(fd);
        }
    }
    // Cleanup scandir
    for(i = 0; i < num; ++i){
        free(event_files[i]);
    }
    free(event_files);
    return keyboard_file;
}

