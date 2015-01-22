#ifndef _HEADER_lsa_
#define _HEADER_lsa_
#include <math.h>
#include <stdlib.h>

typedef struct lsa_run{
    int level;
    int len;
    int curr;
    int count_wr;
    int *array;
    struct lsa_run *next;
}lsa_run_t;

typedef struct{
    int height;
    int r; //fanout factor between adjacent levels
    lsa_run_t *runs; 
}lsa_t;  //cache-oblivious lookahead array (lsa)


int lsaInit( lsa_t *, int, int );
void lsaDestroy( lsa_t * );
int lsaGrow( lsa_t * );
int lsaInsert( lsa_t *, int );
void lsaDump( lsa_t * );
#endif
