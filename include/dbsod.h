#ifndef DBSOD_H
#define DBSOD_H

#include <string>

extern "C" {
    void dbsod(double* data, int rows, int cols, std::string metric);
}

#endif // DBSOD_H
