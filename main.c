#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "random.h"

#include "cola.h"

//#define VERBOSE (1)
#define MAX (1<<10)

void
_print_stat( run_t *r ){
    
    run_t *run = r;
    
#ifdef VERBOSE
    int i;
    while( run ){
        printf( "The run on level %d :", run->level );
        for( i=0; i<run->curr; i++ )
            if( run->array[i] != ~0 )
                printf( "\t%d", run->array[i] );
        printf( "\n" );

        run = run->next;
    };
    
    run = r;

#endif

    while( run ){
        printf( "The run on level %d is written %d times\n", run->level, run->count_wr );
        run = run->next;
    };
    

    return ;
}

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

    if( argc<3 )
        goto help;

    result = colaInit( &cola, atoi(argv[1]), 1 );
    if( result != 0 )
        goto out;

    n = atoi( argv[2] );

    key_set = (int *) malloc ( sizeof(int) * MAX );

    create_random_array( key_set, MAX, MAX );

    for( ki=1; ki<=n; ki++ )
        colaInsert( &cola, key_set[ki] );

help:
    _help();
    result = -1;

out:
    _print_stat( cola.runs );

    colaDestroy( &cola );

    free(key_set);

    return result;
}
