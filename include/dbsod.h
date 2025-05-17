#ifndef DBSOD_H
#define DBSOD_H

#include <string>

extern "C" {
    void dbsod(
        double* data,
        int rows,
        int cols,
        const char* metric,
        float* epsSpacePtr,
        int numEpsValues
    );
}

#endif // DBSOD_H
