#include "sort.h"

void 
quicksort( int *x, int first,int last )
{

    int pivot,j,temp,i;
    
    if(first<last){
        pivot=first;
        i=first;
        j=last;
        
        while(i<j){
            while(x[i]<=x[pivot]&&i<last)
                i++;
            while(x[j]>x[pivot])
                j--;
            if(i<j){
                temp=x[i];
                x[i]=x[j];
                x[j]=temp;
            }
        }
        
        temp=x[pivot];
        x[pivot]=x[j];
        x[j]=temp;
        quicksort(x,first,j-1);
        quicksort(x,j+1,last);
        
    }
}

void
mergeSortedArrays( int *src1, int count1, int *src2, int count2, int *dest )
{
    int i,j,k;

    i=0;
    j=0;
    k=0;

    while( i<count1 && j<count2 ){
        if( src1[i]<= src2[j] ){
            dest[k++] = src1[i++];
            continue;
        }
        else{
            dest[k++] = src2[j++];
            continue;
        }
    }

    while( i<count1 ) 
        dest[k++] = src1[i++];
    
    while( j<count2 ) 
        dest[k++] = src2[j++];

    return;
}
