#if defined(_WIN32) || defined(_WIN64)

#include "Window_win32.cpp"

#elif defined(__linux__)
#include "Window_linux.cpp"
#else
#error "Unsupported platform to init zim::Window class"
#endif