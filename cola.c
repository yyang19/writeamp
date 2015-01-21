#include <assert.h>
#include "cola.h"
#include "sort.h"

static void _insert( cola_t *, run_t *, int *, int );

int
colaInit( cola_t *c, int r, int h ){

    int i;

    c->r = r;
    c->height = 0;
    c->runs = NULL;
    
    for( i=0; i<h; i++ )
        colaGrow(c);

    return 0;
}

void
colaDestroy( cola_t *c ){
    
    run_t *header = c->runs;
    run_t *r;

    while( header ){
        r = header;
        header = header->next;
        free(r->array);        
        free(r);
    }
}

int
colaGrow( cola_t *c ){

    int i;
    int j;
    run_t *r = c->runs;
    run_t *new_run;

    i = c->height;
    
    new_run = (run_t *) malloc (sizeof(run_t)); 

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
colaInsert( cola_t *c, int key ){

    _insert( c, c->runs, &key, 1 );

    return 0;
}

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
_insert( cola_t *c, run_t *r, int *array, int count ){
   
    int i, j;
    int *merged_array;

    if( r->curr == r->len ){

        if( ! r->next )
            colaGrow(c);

        _insert( c, r->next, r->array, r->curr );

        r->curr = 0; //reset frontier in current run
        
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

    return;
}

