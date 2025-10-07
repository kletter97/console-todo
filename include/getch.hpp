#include <iostream>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    int _unix_getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);               // disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);        // set new terminal attributes
        ch = getchar();                                 // read a character
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);        // restore old terminal attributes
        return ch;
    }
#endif

int getch() {
#ifdef _WIN32
    return _getch();            // from <conio.h> for Windows
#else
    return _unix_getch();       // custom implementation for Unix-based systems
#endif
}