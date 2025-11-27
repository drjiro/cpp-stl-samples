#include <iostream>

void inifinite() {
    inifinite();
}

int main()
{
    std::cout << "Start!\n";
    inifinite();
    std::cout << "End!\n";
}
