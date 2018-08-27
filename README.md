# cache-level

Some toy programs for playing with Intel intrinsics [1] at cache level and controling HW prefetcher described at [2].



# msr-tools

This is a tool to control Model Specific Refister on Intel processors. Download it [3] and run `make` to compile that. `rdmsr` and `wrmsr` are used to read/write the MSR. The address of prefetch register is 0x1A4.

```
rdmsr -a 0x1a4     # read prefetch register of all processors
rdmsr -p0 0x1a4    # read prefetch register of processor 0
```

The 4 least significant bits of 0x1A4 is dedicated to 4 prefetch methods in Intel processors.
```
Bit0   : L2 hardware prefetcher. Fetches additional lines of code or data into the L2 cache.
Bit1   : L2 adjacent cache line prefetcher. Fetches the cache line that comprises a cache line pair (128 bytes)
Bit2   : DCU prefetcher. Fetches the next cache line into L1-D cache
Bit3   : DCU IP prefetcher. Uses sequential load history (based on Instruction Pointer of previous loads) to determine whether to prefetch additional lines
```
For each bit, 0 means enable and 1 means disable. Some examples
```
wrmsr -p0 0x1a4 0     # enables all prefetchers on processor 0
wrmsr -p0 0x1a4 15    # disables all prefetchers on processor 0
wrmsr -a 0x1a4 15    # disables all prefetchers on all processors
```


[1] https://software.intel.com/sites/landingpage/IntrinsicsGuide/

[2] https://software.intel.com/en-us/articles/disclosure-of-hw-prefetcher-control-on-some-intel-processors

[3] https://01.org/msr-tools/downloads/msr-tools-source-code
