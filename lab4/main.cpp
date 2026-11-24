#include "cli/ConsoleMenu.h"

#include <clocale>
#include <iostream>

int main() {
    std::setlocale(LC_ALL, "");

    try {
        cli::ConsoleMenu menu;
        menu.run();
    } catch (const std::exception& ex) {
        std::cerr << "Необработанное исключение: " << ex.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Неизвестная критическая ошибка.\n";
        return 1;
    }

    return 0;
}
