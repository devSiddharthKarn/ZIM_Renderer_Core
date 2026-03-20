#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios orig;

void enable_raw() {
    tcgetattr(STDIN_FILENO, &orig);
    struct termios raw = orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

int main() {
    enable_raw();

    printf("\033[?1003h");  // mouse move
    printf("\033[?1006h");  // sgr mode
    fflush(stdout);

    char buf[64];

    while (1) {
        int n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n <= 0) continue;

        // CASE 1: Escape sequence (mouse OR special key)
        if (buf[0] == 27) {

            // ---- MOUSE ----
            int b, x, y;
            char action;

            if (sscanf(buf, "\033[<%d;%d;%d%c", &b, &x, &y, &action) == 4) {

                if (b & 32)
                    printf("Mouse MOVE (%d,%d)\n", x, y);
                else
                    printf("Mouse CLICK (%d,%d)\n", x, y);

                continue;
            }

            // ---- KEYBOARD (special / ALT) ----
            if (buf[1] == '[') {
                char key = buf[2];

                if (key == 'A') printf("Up Arrow\n");
                if (key == 'B') printf("Down Arrow\n");
                if (key == 'C') printf("Right Arrow\n");
                if (key == 'D') printf("Left Arrow\n");
            } else {
                printf("Alt + %c\n", buf[1]);
            }
        }

        // CASE 2: Ctrl keys
        else if (buf[0] < 32) {
            printf("Ctrl key: %d\n", buf[0]);
        }

        // CASE 3: Normal keys
        else {
            printf("Key: %c\n", buf[0]);
        }

        fflush(stdout);
    }
}