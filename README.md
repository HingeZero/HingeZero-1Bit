# HingeZero-1Bit

HingeZero-1Bit explores packed 1-bit associative memory retrieval for efficient and edge-oriented memory systems.

The implementation combines:

- Packed 1-bit memory storage
- Hamming Top-K retrieval
- Local HingeZero refinement
- Low-memory operation for edge systems

Pipeline:

Packed 1-bit memory  
↓  
Hamming Top-K search  
↓  
Unpack Top-K candidates only  
↓  
HingeZero refinement  
↓  
Recall

Example result:

N = 512  
P = 1,000,000 memories  
Packed storage ≈ 61 MB  
Bit corruption = 25%  
Top1 accuracy = 1.0

This repository explores stable remembering, associative retrieval and efficient low-bit memory systems.

Future work:

- Larger dimensions (1024 / 2048 / 4096)
- Billion-scale streamed retrieval
- Edge deployment
- int8 / int4 / int2 comparisons
- Hardware acceleration

Please leave a star if you find the work useful.
