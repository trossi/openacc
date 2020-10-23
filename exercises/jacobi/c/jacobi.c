#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifdef _OPENACC
#include <openacc.h>
#endif

// Utility routine for allocating a two dimensional array
float **malloc_2d(int nx, int ny)
{
    float **array;
    int i;

    array = (float **) malloc(nx * sizeof(float *));
    array[0] = (float *) malloc(nx * ny * sizeof(float));

    for (i = 1; i < nx; i++) {
        array[i] = array[0] + i * ny;
    }

    return array;
}


// Utility routine for deallocating a two dimensional array
void free_2d(float **array)
{
    free(array[0]);
    free(array);
}


void init(float **arr, int nx, int ny)
{
    int i, j;

    /* Implement data initialization with OpenACC on device */
    #pragma acc parallel loop collapse(2) present(arr)
    for (i = 0; i < nx + 2; i++) {
        for (j = 0; j < ny + 2; j++) {
            arr[i][j] = 0e0;
        }
    }

    /* Implement data initialization with OpenACC on device */
    #pragma acc parallel loop present(arr)
    for (i = 0; i < nx + 2; i++) {
        arr[i][ny + 1] = 1e0;
    }

    /* Implement data initialization with OpenACC on device */
    #pragma acc parallel loop present(arr)
    for (i = 0; i < ny + 2; i++) {
        arr[nx + 1][i] = 1e0;
    }
}

void update(float restrict **newarr, float restrict **oldarr, float *norm, int nx, int ny)
{
    int i, j;
    const float factor = 0.25;
    float lnorm;

    if (norm != NULL) {
        lnorm = 0;
        /* Implement computation with OpenACC on device */
        #pragma acc parallel loop collapse(2) reduction(max:lnorm) present(newarr, oldarr)
        for (i = 1; i < nx + 1; i++) {
            for (j = 1; j < ny + 1; j++) {
                newarr[i][j] = factor * (oldarr[i - 1][j] + oldarr[i + 1][j] +
                                         oldarr[i][j - 1] + oldarr[i][j + 1]);
                lnorm = fmaxf(lnorm, fabsf(newarr[i][j] - oldarr[i][j]));
            }
        }
        *norm = lnorm;
    } else {
        /* Implement computation with OpenACC on device */
        #pragma acc parallel loop collapse(2) present(newarr, oldarr)
        for (i = 1; i < nx + 1; i++) {
            for (j = 1; j < ny + 1; j++) {
                newarr[i][j] = factor * (oldarr[i - 1][j] + oldarr[i + 1][j] +
                                         oldarr[i][j - 1] + oldarr[i][j + 1]);
            }
        }
    }
}

void usage(pname)
{
    printf("Usage: %s [nx] [ny]", pname);
}


int main(int argc, char **argv)
{
    struct timespec time0, time1;
    float eps;

    float **u, ** unew;
    float norm = 0e0, mlups = 0e0;
    int maxiter, nx, ny, iter, ndef = 2400;
    clock_t t_start, t_end;
    float dt;

    eps = 0.5e-3;
    maxiter = (int)(1e0 / eps);

    switch (argc) {
    case 1:
        nx = ndef;
        ny = nx;
        break;
    case 2:
        nx = atoi(argv[1]);
        ny = nx;
        break;
    case 3:
        nx = atoi(argv[1]);
        ny = atoi(argv[2]);
        break;
    default:
        usage(argv[0]);
        return -1;
    }

    printf("Stencil: nx,ny,maxiter,eps=%d %d %d %18.16f\n", nx, ny, maxiter, eps);

    u = malloc_2d(nx + 2, ny + 2);
    unew = malloc_2d(nx + 2, ny + 2);

    clock_gettime(CLOCK_REALTIME, &time0);
    /* Initialize data region on device */
    #pragma acc data create(u[:(nx+2)][:(ny+2)], unew[:(nx+2)][:(ny+2)])
    {
    init(u, nx, ny);
    init(unew, nx, ny);

    t_start = clock();

    norm = eps + 1;
    iter = 0;
    while (iter <= maxiter && norm >= eps) {
        update(unew, u, NULL, nx, ny);
        update(u, unew, &norm, nx, ny);
        iter = iter + 2;
        if (iter % 100 == 0 || norm < eps) {
            printf(": norm, eps= %18.16f %18.16f\n", norm, eps);
        }
    }
    }
    clock_gettime(CLOCK_REALTIME, &time1);

    double time = (time1.tv_sec - time0.tv_sec)
                  + (time1.tv_nsec - time0.tv_nsec)*1e-9;

    printf("Time: %.6f s\n", time);

    free_2d(u);
    free_2d(unew);

    mlups = 1.0e-6 * iter * nx * ny;
    t_end = clock();
    dt = ((float)(t_end - t_start)) / CLOCKS_PER_SEC;
    printf("'Stencil: norm =%18.16f with iter = %d\n", norm, iter);
    printf("'Stencil: Time =%18.16f sec, MLups/s=%18.16f\n", dt, (float) mlups / dt);

    return 0;
}
