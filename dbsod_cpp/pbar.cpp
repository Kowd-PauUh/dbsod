#include "pbar.h"
#include <iostream>
#include <chrono>
#include <string>

void pbar(
    int current,
    int total,
    int width = 50,
    const std::string& desc = ""
) {
    static auto last_update = std::chrono::steady_clock::now();
    static int last_count = 0;
    static double speed = 0.0;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count();

    // only update every 100 milliseconds
    if (elapsed >= 100 || current == total) {
        // progress [%]
        float progress = static_cast<float>(current) / total;
        int pos = width * progress;

        // speed [it/s]
        if (elapsed > 0) {
            speed = (current - last_count) / (elapsed / 1000.0);
        }

        std::cout << desc << " [";
        for (int i = 0; i < width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << "% ";
        std::cout << "(" << speed << " it/s)\r";
        std::cout.flush();

        last_update = now;
        last_count = current;
    }
}
