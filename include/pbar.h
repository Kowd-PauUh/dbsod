#ifndef PBAR_H
#define PBAR_H

#include "pbar.h"
#include <string>

void pbar(
    int current,
    int total,
    int width,
    const std::string& desc
);

#endif // PBAR_H
