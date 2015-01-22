#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "random.h"

#include "cola.h"
#include "lsa.h"

#define MAX (1<<10)

static void
_help( void ){
    printf( "WA package options:\n" );
    printf("\t args: [1]FAN OUT [2] # of insertions/updates \n");
}

int main( int argc, char *argv[] ){
    
    int ki;
    int n;
    int result = 0;

    int *key_set;

    cola_t cola;
    lsa_t lsa;

    if( argc<3 )
        goto help;

    result = colaInit( &cola, atoi(argv[1]), 1 );
    if( result != 0 )
        goto out;

    result = lsaInit( &lsa, atoi(argv[1]), 1 );
    if( result != 0 ){
        colaDestroy( &cola );
        goto out;
    }

    n = atoi( argv[2] );

    key_set = (int *) malloc ( sizeof(int) * MAX );

    create_random_array( key_set, MAX, MAX );

    for( ki=1; ki<=n; ki++ ){
        colaInsert( &cola, key_set[ki] );
        lsaInsert( &lsa, key_set[ki] );
    }

    colaDump( &cola );
    lsaDump( &lsa );

    colaDestroy( &cola );
    lsaDestroy( &lsa );

    free(key_set);
    
    goto out;

help:
    _help();
    result = -1;

out:
    return result;
}
