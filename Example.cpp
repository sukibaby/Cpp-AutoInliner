// Example.cpp

#include <iostream>

#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define GREETING "Hello, World!"

int main() {
    double radius = 5.0;
    double area;

    if (radius > 0) {
        area = PI * SQUARE(radius);
    } else {
        area = 0;
    }

    std::string message;
    if (radius > 0) {
        message = GREETING;
    } else {
        message = "Invalid radius";
    }

    return 0;
}
