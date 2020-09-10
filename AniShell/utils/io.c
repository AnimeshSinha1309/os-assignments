#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
char get_key() {
    struct termios state;
    tcgetattr(0, &state);
    state.c_lflag &= ~(ICANON | ECHO);  // Disable echo as well
    tcsetattr(0, TCSANOW, &state);
    int buffer_count;
    ioctl(0, FIONREAD, &buffer_count);
    tcgetattr(0, &state);
    state.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &state);
    return (buffer_count == 0) ? '\0' : getchar();
}
#pragma clang diagnostic pop
