import scipy.linalg
import numpy as np
import time

nrows = 100
N = 1000
A = np.random.rand(nrows, nrows)
B = np.random.rand(nrows, nrows)

tic = time.perf_counter()   # time things with performance counter (not time.time())

# run
print("starting test")
for i in range(N):
    # print progress every 10%
    if i % int(N*0.10) == 0:
        print(f"{i}/{N}")
    C = np.matmul(A,B)
    # C = scipy.linalg.blas.sgemm(1, A,B)     # C = alpha * matA * matB

toc = time.perf_counter()
duration = toc-tic

print(f"Multipling {nrows}x{nrows} matrices {N} times took {duration} seconds")