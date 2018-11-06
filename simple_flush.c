/* Compile:   gcc -Wall -O3 -g -o simple_flush simple_flush.c
       Run:   taskset -c 0 ./simple_flush
*/

#include <stdio.h>
#include <x86intrin.h>
#include <stdint.h>
int main()
{
    /* create array */
    int array[ 100 ];
    int i;
    volatile uint64_t j, z;
    for ( i = 0; i < 100; i++ )
        array[ i ] = i;   // bring array to the cache

	// warmup the cpu frequency
    z = 0;
    for ( j = 0; j < 2000000000; j++ ) {
          ++z;
    }

    uint64_t t1, t2, ov, diff1, diff2, diff3, diff4;

    /* flush the first cache line */
    _mm_lfence();
    _mm_clflush( &array[ 30 ] );
    _mm_lfence();

    /* READ MISS */
    _mm_lfence();           // fence to keep load order
    t1 = __rdtsc();         // set start time
    _mm_lfence();
    (void) *((volatile int*)array + 30);   // read the first elemet => cache miss
    _mm_lfence();
    t2 = __rdtsc();         // set stop time
    _mm_lfence();
    diff1 = t2 - t1;        // two fence statements are overhead
	
    /* READ HIT*/
    _mm_lfence();           // fence to keep load order
    t1 = __rdtsc();         // set start time
    _mm_lfence();
    (void) *((volatile int*)array + 33);   // read the first elemet => cache hit
    _mm_lfence();
    t2 = __rdtsc();         // set stop time
    _mm_lfence();
    diff2 = t2 - t1;        // two fence statements are overhead
	
    /* measuring fence overhead */
    _mm_lfence();
    t1 = __rdtsc();
    _mm_lfence();
    _mm_lfence();
    t2 = __rdtsc();
    _mm_lfence();
    ov = t2 - t1;

    printf( "lfence overhead is %lu\n", ov );
    printf( "TSC for array[30] is %lu\n", diff1-ov );
    printf( "TSC for array[33] is %lu\n", diff2-ov );
    return 0;
}
 
