/*
 * Copyright 2025 Ivan Danylenko
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pbar.h"
#include <iostream>
#include <chrono>
#include <string>

void pbar(
    int current,
    int total,
    int width = 20,
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
        std::cout << "(" << speed << " it/s)           \r";
        std::cout.flush();

        last_update = now;
        last_count = current;
    }
    if (current == total) {
        std::cout << std::endl;
        last_update = std::chrono::steady_clock::now();
        last_count = 0;
        speed = 0.0;
    }
}
