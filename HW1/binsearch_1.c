/* Simple binary search
 * downloaded from https://shaeod.tistory.com/313 
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>

int RecursiveBinSearch( int A[], int begin, int end, int target ) 
{
  int nMid = 0;

  if( begin > end ) return -1;	// escape or search failure

  nMid = (begin+end) / 2;
  if( A[nMid] == target ) return nMid;	// found target
  else if( target < A[nMid] )
    return RecursiveBinSearch( A, begin, nMid-1, target );
  else
    return RecursiveBinSearch( A, nMid+1, end, target );
}

int main( )
{
  int A[] = { 1, 3, 5, 7, 9, 11, 13 };
  int res;
  int i;

  res = RecursiveBinSearch( A, 0, sizeof(A)/sizeof(int)-1, 7 );
  if(res == -1)
    printf( "Search failed.\n" );
  else {
    for( i=0 ; i<(int)(sizeof(A)/sizeof(int)) ; ++i )
      printf( "%d ", A[i] );
    printf( "\nThe number is at %d element of the array.\n", res );
  }

  return 0;
}
