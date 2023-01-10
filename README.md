# BLAS libraries are stupid fast
- Yong DA Li
- Saturday, December 31, 2022

## copy-paste from slack \#random channel
> I've been playing around with C/C++ computational libraries, cuz that's what my thesis research field is. You're always supposed to use the provided subroutines for math operations. They optimize the hell out of pipelining and SIMD (single instruction multiple data) stuff that I'm too small brain to understand. I never really knew how fast they were until I tried it out today. I used a generic BLAS (basic linear algebra subprogram) library from Netlib that isn't vendor specific (about 10x faster). Then I downloaded the AMD Ryzen specific one (about 200x faster).
> - https://netlib.org/blas/
> - https://developer.amd.com/amd-aocl/ 
> 
> I'm doing a matrix * matrix multiplication. The inputs are 100x100 random matrices and I'm doing the computation 1000 times.
> - my naive implementation - ~7s (varies up to 0.5s)
> - netlib BLAS - 0.6s
> - BLAS provided by AMD - 0.044s
> 
> I should also note that I wrote my matrix library using single-precision floats (32-bits). The BLAS libraries are operating on doubles (64-bit double precision), so they're doing 2x the amount of work. And they're still so fast.

## usage
Uncomment/comment these lines in the makefile to use the AMD BLAS or the regular BLAS. The AMD BLAS library is downloaded to my Ubuntu downloads folder. The library is statically linked (I couldn't get the dynamic .so linking to work) and file path is hard coded.

The `LP` and `ILP` is just how big your arrays are. The `LP` type uses 32-bit integers for array indices. If your arrays are even larger than that, then the `ILP` uses 64-bit integers for the array indices. I know the link is the Intel documentation, but I'm pretty sure this is an industry standard thing and AMD follows suit. As such, I'm using the `LP` version since my arrays are not that big.
- https://www.intel.com/content/www/us/en/develop/documentation/onemkl-linux-developer-guide/top/linking-your-application-with-onemkl/linking-in-detail/linking-with-interface-libraries/using-the-ilp64-interface-vs-lp64-interface.html

```
# use regular Netlib BLAS
CFLAGS = -Wall -I/usr/local/include -Iutil -I -g -c	# compiler flags
LIBS = -lblas -lm

# use AMD Ryzen BLAS, statically link
# CFLAGS = -Wall -I/usr/local/include -I/home/liyongda/Downloads/amd-blis/include/LP64 -Iutil -I -g -c	# compiler flags
# LIBS = /home/liyongda/Downloads/amd-blis/lib/LP64/libblis.a  -lm -lpthread
```

## Resources
AMD user guide
- https://developer.amd.com/wp-content/resources/57404_AOCL_v4.0_GA_UG.pdf

Similar vibes to George Hotz multiplying matrices faster than NumPy.
- https://www.youtube.com/watch?v=VgSQ1GOC86s


## Python comparision
For fun, I also tried the default `np.matmul(A,B)` matrix-matrix multiplication that NumPy provides. It's resonably fast, on the same order of speed as the generic Netlib BLAS. It took 0.62 seconds.

Python is compiled to Python byte code. Then that byte code is interpreted by the Python interpreter. With C or C++, the equivalent of the byte code is direct assembly code that can be directly executed by the CPU. So the overhead of the interpreter easily adds like 100x more clock cycles per meaningful user code instruction.
- https://youtu.be/vVUnCXKuNOg?t=322

Explicitly using BLAS via `scipy.linalg.blas.sgemm(alpha, matA, matB)` took 0.433 seconds.
- https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.blas.sgemm.html
- https://www.benjaminjohnston.com.au/matmul

Ways to speed up Python
1. use BLAS
2. use multi-core

  