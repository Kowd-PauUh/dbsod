#ifndef DBSOD_H
#define DBSOD_H

#include <string>

extern "C" {
    float* dbsod(
        double* data,
        int rows,
        int cols,
        const char* metric,
        float* epsSpacePtr,
        int numEpsValues,
        int minPts
    );
    void free_array(float* arrayPtr);
}

#endif // DBSOD_H
