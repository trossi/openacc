#include <stdio.h>
#include <time.h>

#ifdef _OPENACC
#include <openacc.h>
#endif

#define NX 1<<25

int main(void)
{
    struct timespec time0, time1;
    double vecA[NX], vecB[NX], vecC[NX];
    double sum;
    int i;

    printf("Array size: %d.\n", NX);

    /* Initialization of the vectors */
    for (i = 0; i < NX; i++) {
        vecA[i] = 1.0 / ((double) (NX - i));
        vecB[i] = vecA[i] * vecA[i];
    }

    /*
     * Vector addition on device with OpenACC
     * vecC = vecA + vecB
     */
    clock_gettime(CLOCK_REALTIME, &time0);
    #pragma acc parallel loop
    for (i = 0; i < NX; i++) {
        vecC[i] = vecA[i] + vecB[i];
    }
    clock_gettime(CLOCK_REALTIME, &time1);

    double time = (time1.tv_sec - time0.tv_sec)
                  + (time1.tv_nsec - time0.tv_nsec)*1e-9;

    printf("Time: %.6f s\n", time);

    sum = 0.0;
    /* Compute the check value */
    for (i = 0; i < NX; i++) {
        sum += vecC[i];
    }
    printf("Reduction sum: %18.16f\n", sum);

    return 0;
}
