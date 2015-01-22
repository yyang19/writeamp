#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lsa.h"
#include "sort.h"

static void _insert( lsa_t *, lsa_run_t *, int *, int );

int
lsaInit( lsa_t *c, int r, int h ){

    int i;

    c->r = r;
    c->height = 0;
    c->runs = NULL;
    
    for( i=0; i<h; i++ )
        lsaGrow(c);

    return 0;
}

void
lsaDestroy( lsa_t *c ){
    
    lsa_run_t *header = c->runs;
    lsa_run_t *r;

    while( header ){
        r = header;
        header = header->next;
        free(r->array);        
        free(r);
    }
}

int
lsaGrow( lsa_t *c ){

    int i;
    int j;
    lsa_run_t *r = c->runs;
    lsa_run_t *new_run;

    i = c->height;
    
    new_run = (lsa_run_t *) malloc (sizeof(lsa_run_t)); 

    if( !new_run )
        return -1;
    
    new_run->level      = i;
    new_run->len        = (int) pow(c->r,i);
    new_run->curr       = 0 ;
    new_run->count_wr   = 0;
    new_run->next       = NULL;

    new_run->array  = malloc(sizeof(int) * new_run->len);

    if( !new_run->array ){
        free( new_run );
        return -1;
    }

    for( j=0; j<new_run->len; j++ )
        new_run->array[j] = ~0;

    if( !c->runs )
        c->runs = new_run;
    else{
        while( r->next )
            r = r->next;
        r->next = new_run;
    }

    ++ c->height;

    return 0;
}

int 
lsaInsert( lsa_t *c, int key ){

    _insert( c, c->runs, &key, 1 );

    return 0;
}

void
lsaDump( lsa_t *c ){
        
    lsa_run_t *run = c->runs;
   
    printf("-----LSA dump-----\n");
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
    
    run = c->runs;
#endif

    while( run ){
        printf( "The run on level %d is written %d times\n", run->level, run->count_wr );
        run = run->next;
    };
    

    return ;
}

static void
_writePageNode( lsa_run_t *r, int key ){
    
    assert( r->curr<r->len );

#ifdef VERBOSE
    printf( "Write key %d in node #%d on level %d\n", key, r->curr, r->level );
#endif
    r->array[r->curr] = key;
    r->count_wr ++;
    r->curr ++;
    
}

static void 
_insert( lsa_t *c, lsa_run_t *r, int *array, int count ){
   
    int i,j;
    int *merged_array;

    if( r->curr == r->len ){

        if( ! r->next )
            lsaGrow(c);
    
        quicksort( r->array, 0, r->curr-1 );

        _insert( c, r->next, r->array, r->curr );

        r->curr = 0; //reset frontier in current run
        
        for( i=0; i<count; i++ )
            _writePageNode( r, array[i] );
    }
    else{
        if( r->next ){ //not the lowest level
            for( i=0; i<count; i++ )
                _writePageNode( r, array[i] );
        }
        else{

            j = r->curr;
            assert( count+j <= r->len );

            merged_array = (int *) malloc (sizeof(int)*(count + j));
            mergeSortedArrays( array, 
                               count, 
                               r->array,
                               j,
                               merged_array );

            r->curr = 0;

            for( i=0; i<count+j; i++ )
                _writePageNode( r, merged_array[i] );

            free( merged_array );
        }
    }    

    return;
}

