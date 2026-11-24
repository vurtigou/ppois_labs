#pragma once

namespace cli {

class ConsoleMenu {
public:
    ConsoleMenu() = default;
    ~ConsoleMenu() = default;

    void run();

private:
    static void printMainMenu();
    static void printSortingMenu();
    static void printGraphMenu();

    void handleSortingTask();
    void handleGraphTask();

    static int readInt(const char* prompt, int minVal, int maxVal);
    static double readDouble(const char* prompt, double minVal, double maxVal);
    static void clearInputStream();
};

} // namespace cli
