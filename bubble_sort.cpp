/*
 *  compile command:    g++ -O3 -Wall -o bubble_sort bubble_sort.cpp
 *      run command:    taskset -c 0 ./bubble_sort
 */
 
 /*  get msr-tools 
  *  ./rdmsr -a 0x1a4       read all processors prefetch register
  *  ./wrmsr -a 0x1a4 0     write 0 to prefetch register for all processors => enable 4 prefetch methods
  *  ./wrmsr -a 0x1a4 15    write 0xF to prefetch register for all processors => disable 4 prefetch methods
  */
  
#include <iostream>
#include <time.h>
#include <x86intrin.h>
#include <stdint.h>
int main()
{
  srand( time(0) );
  int N = 100000;

  /* create two similar arrays */
  int array[ N ], backup[ N ];
  for (int i = 0; i < N; i++) {
    array[ i ] = rand() % N;
    backup[ i ] = array[ i ];
  }

  /* flush arrays from all cache levels */
  for (int i = 0; i < N; i++) {
    _mm_clflush( &array[ i ] );
    _mm_clflush( &backup[ i ] );
  }


  /* measuring time with clock_gettime */
  struct timespec start, end;
  int tmp;
  clock_gettime( CLOCK_MONOTONIC, &start );
  for (int i = 0; i < N; i++ ) {
    for (int j = i+1; j < N; j++ ) {
      if (array[ i ] > array[ j ]) {
        tmp = array[ i ];
        array[ i ] = array[ j ];
        array[ j ] = tmp;
      }
    }
  }
  clock_gettime( CLOCK_MONOTONIC, &end );
  uint64_t diff = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;  // time is reported in ns

  /* measuring time with TSC */
  uint64_t ts, te, td;
  _mm_lfence(); // fence to be sure that order of instructions are not changed with -O3
  ts = __rdtsc();
  _mm_lfence();
  for (int i = 0; i < N; i++ ) {
    for (int j = i+1; j < N; j++ ) {
      if (backup[ i ] > backup[ j ]) {
        tmp = backup[ i ];
        backup[ i ] = backup[ j ];
        backup[ j ] = tmp;
      }
    }
  }
  _mm_lfence();
  te = __rdtsc();
  _mm_lfence();
  td = te - ts;

  std::cout << "\nSize = " << N << "\n";
  std::cout << "Time = " << (float)diff/1000000 << "ms\n";
  std::cout << "TSC = " << td << "\n";

  return 0;
}
