#include <iostream>

using namespace std;

void ClearScreen();

int main() {
    ClearScreen();
}

void ClearScreen() {
    std::cout << "\033[2J\033[1;1H";
}
