# Laborator 3

## SPECS

- My machine has an 8 core(16 threads) processor

## TIME

1. OETS:

- main thread  
    `$ time ./oets 90999 0`  
    real 0m24,688s  
    user 0m24,459s  
    sys 0m0,036s

- 2 threads  
    `$ time ./oets 90999 2`  
    real 0m15,575s  
    user 0m26,673s  
    sys 0m0,390s  
- 4 threads  
    `$ time ./oets 90999 4`  
    real 0m12,050s  
    user 0m32,879s  
    sys 0m1,323s  
- 8 threads  
    `$ time ./oets 90999 8`  
    real 0m10,276s  
    user 0m43,496s  
    sys 0m3,606s  

- 16 threads  
    `$ time ./oets 90999 16`  
    real 0m15,153s  
    user 1m6,170s  
    sys 0m10,742s  

2. SHEAR:  

- main thread  
    `$ time ./shear 5555 0`  
    real 0m32,846s  
    user 0m32,733s  
    sys 0m0,040s  

- 2 threads  
    `$ time ./shear 5555 2`  
    real 0m17,307s  
    user 0m33,870s  
    sys 0m0,036s  

- 4 threads  
    `$ time ./shear 5555 4`  
    real 0m9,236s  
    user 0m34,649s  
    sys 0m0,044s  

- 8 threads  
    `$ time ./shear 5555 8`  
    real 0m5,724s  
    user 0m38,407s  
    sys 0m0,072s  

- 16 threads  
    `$ time ./shear 5555 16`  
    real 0m4,621s  
    user 0m55,099s  
    sys 0m0,136s  
