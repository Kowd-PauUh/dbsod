#ifndef DBSOD_H
#define DBSOD_H

#include <string>

extern "C" {
    double* dbsod(
        double* data,
        int rows,
        int cols,
        const char* metric,
        float* epsSpacePtr,
        int numEpsValues,
        int minPts
    );
    void free_array(double* arrayPtr);
}

#endif // DBSOD_H
