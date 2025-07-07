#include "pbar.h"
#include <iostream>
#include <chrono>
#include <string>

void pbar(
    int current,
    int total,
    int width = 50
) {
    static auto last_update = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count();

    // only update every 50 milliseconds
    if (elapsed >= 50 || current == total) {
        float progress = static_cast<float>(current) / total;
        int pos = width * progress;

        std::cout << "[";
        for (int i = 0; i < width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();

        last_update = now;
    }
}
