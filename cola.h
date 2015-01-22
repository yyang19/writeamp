#ifndef _HEADER_COLA_
#define _HEADER_COLA_
#include <math.h>
#include <stdlib.h>

typedef struct run{
    int level;
    int len;
    int curr;
    int count_wr;
    int *array;
    struct run *next;
}run_t;

typedef struct{
    int height;
    int r; //fanout factor between adjacent levels
    run_t *runs; 
}cola_t;  //cache-oblivious lookahead array (COLA)


int colaInit( cola_t *, int, int );
void colaDestroy( cola_t * );
int colaGrow( cola_t * );
int colaInsert( cola_t *, int );
void colaDump( cola_t * );
#endif
