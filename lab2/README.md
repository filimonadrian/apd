# TIME

My machine has an 8 core(16 threads) processor.

- sequential program: `time ./multiply_seq 1000` - `7.204s`
- exterior for on threads: `time ./multiply 1000 16` - `1.25s`
- second for on threads: `time ./multiply 1000 16` - `0.679s`
- third for on threads:
    - `time ./multiply 1000 16` - 51s
	- `time ./multiply 1000 8` - 55s
- strassen algorithm: `time ./strassen 1000` - `1.368s`

