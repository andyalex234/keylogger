# keylogger
The basic functionality of keylogger is to record the user’s keystrokes when ever the user is pressing on some keys. Once a keylogger is installed on a target machine, it starts operating in the background and capture every stroke on the target machine. But for the sake of simplicity the keylogger that we have designed doesn’t run in the background.

# Instructions
$gcc -c main.c keylogger.c find_event_file.c
$gcc -o keylogger main.o keylogger.o find_event_file.o
$sudo ./keylogger -f [filename.txt]
