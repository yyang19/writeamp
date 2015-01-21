#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "sort.h"
#include "random.h"

#define FAN_OUT (3)
#define LEVEL_TOTAL (4)
//#define VERBOSE (1)

typedef struct run{
    int level;
    int len;
    int curr;
    int count_wr;
    int *array;
    struct run *next;
}run_t;

static void
_writePageNode( run_t *r, int key ){
    
    assert( r->curr<r->len );

#ifdef VERBOSE
    printf( "Write key %d in node #%d on level %d\n", key, r->curr, r->level );
#endif
    r->array[r->curr] = key;
    r->count_wr ++;
    r->curr ++;
    
}

static void 
_inject( run_t *r, int key ){
   
    int i;

    if( r->curr == r->len ){

        quicksort( r->array, 0, r->len-1 );

        if( r->next->curr < r->next->len )
            r->next->count_wr += r->next->curr;

        for( i=0; i<r->len; i++ ){
            //printf("Move key %d from level %d to level %d\n", key, r->level, r->level+1);
            _inject( r->next, r->array[i] );
            r->array[i] = ~0 ;
        }

        r->curr = 0;
    }

    _writePageNode( r, key );

    return;
}

void
_print_stat( run_t *r ){
    
    int i;
    run_t *run = r;
    
    while( run ){
        //printf( "The run on level %d :", run->level );
        for( i=0; i<run->curr; i++ )
            if( run->array[i] != ~0 )
                printf( "\t%d", run->array[i] );
        printf( "\n" );

        run = run->next;
    };
    
    run = r;

    while( run ){
        printf( "The run on level %d is written %d times\n", run->level, run->count_wr );
        run = run->next;
    };
    

    return ;
}

int main( int argc, char *argv[] ){
    
    int i,j;
    int level = LEVEL_TOTAL;
    int ki;
    int universe;

    int *key_set;
    int key_set_size;

    run_t  *runs;

    runs = (run_t *) malloc (level * sizeof(run_t));

    for( i = 0; i < level ; i++ ){
        
        runs[i].level = i;
        runs[i].len = (int) pow(FAN_OUT,i);//1<<i;
        runs[i].curr = 0 ;
        runs[i].count_wr = 0;
        runs[i].array = malloc ( sizeof(int) * runs[i].len );    
        if( i>0 )
            runs[i-1].next = &runs[i];
        else if( i==LEVEL_TOTAL-1 )
            runs[i].next = NULL;

        if( !runs[i].array )
            goto out;
            
        for( j=0; j<runs[i].len; j++ )
            runs[i].array[j] = ~0;
    }

    _print_stat( runs );

    universe = atoi( argv[1] );

    key_set_size = (int)pow(FAN_OUT,LEVEL_TOTAL) * 2;
    key_set = (int *) malloc ( sizeof(int) * key_set_size );

    create_random_array( key_set, key_set_size, key_set_size );

    for( ki=1; ki<=universe; ki++ ){
        _inject( runs, key_set[ki] );
        //_print_stat( runs );
    }

out:
    _print_stat( runs );

    for( j = 0; j < i ; j++ )
        free(runs[j].array);
    
    free( runs );

    free(key_set);

    return 0;
}
