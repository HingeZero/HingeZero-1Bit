# Usage Examples

This guide provides practical examples for using HingeZero-1Bit in your projects.

## Table of Contents

- [Basic Setup](#basic-setup)
- [Memory Operations](#memory-operations)
- [Retrieval Operations](#retrieval-operations)
- [HingeZero Refinement](#hingezero-refinement)
- [Robustness Testing](#robustness-testing)
- [Performance Benchmarking](#performance-benchmarking)
- [Complete End-to-End Example](#complete-end-to-end-example)

## Basic Setup

### Import the Required Classes

```python
from core import PackedMemory, HammingTopK, HingeZeroRefinement
import numpy as np
```

### Create a 1-Bit Query

```python
# Generate a random 1-bit vector (dimension = 512)
query = np.random.randint(0, 2, size=512, dtype=np.uint8)

# Or create from a continuous vector by thresholding
continuous_vector = np.random.randn(512)
query_1bit = (continuous_vector > 0).astype(np.uint8)

print(f"Query shape: {query.shape}")
print(f"Query dtype: {query.dtype}")
```

## Memory Operations

### Initialize Packed Memory

```python
# Create a packed memory system
# dimension: vector dimensionality
# max_memories: maximum number of vectors to store
memory = PackedMemory(dimension=512, max_memories=1_000_000)

print(f"Initialized memory with capacity: {memory.capacity}")
print(f"Current size: {memory.size()}")
```

### Add Memories

```python
# Add a single memory
single_vector = np.random.randint(0, 2, size=512, dtype=np.uint8)
memory.add(single_vector)

# Add multiple memories
batch_vectors = np.random.randint(0, 2, size=(100, 512), dtype=np.uint8)
for vec in batch_vectors:
    memory.add(vec)

# Check memory statistics
print(f"Total memories stored: {memory.size()}")
print(f"Storage size: {memory.size_mb():.2f} MB")
```

### Check Memory Status

```python
# Get current memory statistics
stats = memory.get_statistics()
print(f"Dimension: {stats['dimension']}")
print(f"Stored vectors: {stats['count']}")
print(f"Storage (MB): {stats['size_mb']}")
print(f"Capacity utilization: {stats['utilization']:.2%}")
```

## Retrieval Operations

### Basic Hamming Top-K Search

```python
# Search for top-10 similar vectors
query = np.random.randint(0, 2, size=512, dtype=np.uint8)
k = 10

top_indices, distances = HammingTopK.search(memory, query, k=k)

print(f"Top-{k} indices: {top_indices}")
print(f"Hamming distances: {distances}")
```

### Batch Retrieval

```python
# Retrieve for multiple queries at once
num_queries = 5
queries = np.random.randint(0, 2, size=(num_queries, 512), dtype=np.uint8)

results = []
for i, query in enumerate(queries):
    top_indices, distances = HammingTopK.search(memory, query, k=10)
    results.append({
        'query_idx': i,
        'top_indices': top_indices,
        'distances': distances
    })

print(f"Retrieved {len(results)} result sets")
```

### Top-1 Retrieval (Exact Match)

```python
# Find the single closest match
query = np.random.randint(0, 2, size=512, dtype=np.uint8)
top_index, distance = HammingTopK.search(memory, query, k=1)

print(f"Closest match index: {top_index[0]}")
print(f"Hamming distance: {distance[0]}")
```

## HingeZero Refinement

### Basic Refinement

```python
# Get top-k candidates and refine them
query = np.random.randint(0, 2, size=512, dtype=np.uint8)

# First get approximate results
top_indices, approx_distances = HammingTopK.search(memory, query, k=100)

# Refine the results
refined_results = HingeZeroRefinement.refine(
    memory=memory,
    query=query,
    candidate_indices=top_indices,
    top_k=10
)

print(f"Refined top-10: {refined_results}")
```

### Two-Stage Retrieval Pipeline

```python
def retrieve_with_refinement(memory, query, num_candidates=100, top_k=10):
    """
    Two-stage retrieval: fast approximate search + refinement
    """
    # Stage 1: Fast approximate search
    candidate_indices, _ = HammingTopK.search(
        memory, query, k=num_candidates
    )
    
    # Stage 2: Refine candidates
    refined = HingeZeroRefinement.refine(
        memory=memory,
        query=query,
        candidate_indices=candidate_indices,
        top_k=top_k
    )
    
    return refined

# Use the pipeline
query = np.random.randint(0, 2, size=512, dtype=np.uint8)
final_results = retrieve_with_refinement(memory, query)
print(f"Final refined results: {final_results}")
```

## Robustness Testing

### Test Bit Corruption Tolerance

```python
# Add some test vectors
test_vectors = np.random.randint(0, 2, size=(10, 512), dtype=np.uint8)
for vec in test_vectors:
    memory.add(vec)

# Corrupt bits in stored memory
corruption_rate = 0.25  # 25% bit error
memory.corrupt_bits(corruption_rate=corruption_rate)

# Try to retrieve
query = test_vectors[0].copy()
top_indices, distances = HammingTopK.search(memory, query, k=1)

print(f"Top-1 index after {corruption_rate*100}% corruption: {top_indices[0]}")
print(f"Distance: {distances[0]}")
```

### Measure Robustness

```python
def test_robustness(memory, test_query, corruption_rates):
    """Test retrieval accuracy under different corruption levels"""
    results = {}
    
    for corruption in corruption_rates:
        # Corrupt memory
        memory_copy = memory.copy()
        memory_copy.corrupt_bits(corruption_rate=corruption)
        
        # Try retrieval
        top_indices, distances = HammingTopK.search(
            memory_copy, test_query, k=1
        )
        
        results[corruption] = {
            'top_index': top_indices[0],
            'distance': distances[0]
        }
    
    return results

# Test
query = np.random.randint(0, 2, size=512, dtype=np.uint8)
corruption_rates = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5]
robustness = test_robustness(memory, query, corruption_rates)

for rate, result in robustness.items():
    print(f"Corruption {rate*100:5.1f}%: distance={result['distance']:3d}")
```

## Performance Benchmarking

### Measure Search Speed

```python
import time

# Create memory with many vectors
num_vectors = 100_000
memory = PackedMemory(dimension=512, max_memories=1_000_000)

print("Adding vectors...")
for i in range(num_vectors):
    vec = np.random.randint(0, 2, size=512, dtype=np.uint8)
    memory.add(vec)

print(f"Storage size: {memory.size_mb():.2f} MB")

# Benchmark search
num_searches = 100
queries = np.random.randint(0, 2, size=(num_searches, 512), dtype=np.uint8)

start_time = time.time()
for query in queries:
    HammingTopK.search(memory, query, k=10)
elapsed = time.time() - start_time

throughput = num_searches / elapsed
print(f"\nSearch Benchmark:")
print(f"Total queries: {num_searches}")
print(f"Time: {elapsed:.3f} seconds")
print(f"Throughput: {throughput:.1f} queries/second")
print(f"Per-query time: {elapsed/num_searches*1000:.2f} ms")
```

### Compare Packed vs Unpacked Storage

```python
def compare_storage_efficiency():
    """Compare memory usage: packed 1-bit vs float32"""
    dimensions = [256, 512, 1024, 2048]
    num_vectors = 1_000_000
    
    print("Storage Efficiency Comparison")
    print(f"Number of vectors: {num_vectors:,}")
    print(f"{'Dimension':<12} {'1-bit (MB)':<15} {'Float32 (MB)':<15} {'Ratio':<10}")
    print("-" * 52)
    
    for dim in dimensions:
        # 1-bit storage (rounded up to nearest byte)
        bytes_per_vector = (dim + 7) // 8
        packed_size = (bytes_per_vector * num_vectors) / (1024 * 1024)
        
        # Float32 storage
        float_size = (dim * 4 * num_vectors) / (1024 * 1024)
        
        ratio = float_size / packed_size
        print(f"{dim:<12} {packed_size:<15.2f} {float_size:<15.2f} {ratio:.1f}x")

compare_storage_efficiency()
```

## Complete End-to-End Example

```python
from core import PackedMemory, HammingTopK, HingeZeroRefinement
import numpy as np
import time

# Configuration
DIMENSION = 512
NUM_MEMORIES = 100_000
K_APPROXIMATE = 1000
K_FINAL = 10

print("=" * 60)
print("HingeZero-1Bit Complete Example")
print("=" * 60)

# Step 1: Initialize Memory
print("\n1. Initializing memory...")
memory = PackedMemory(dimension=DIMENSION, max_memories=NUM_MEMORIES * 2)

# Step 2: Populate with Random Vectors
print(f"2. Adding {NUM_MEMORIES:,} random vectors...")
start = time.time()
for i in range(NUM_MEMORIES):
    vec = np.random.randint(0, 2, size=DIMENSION, dtype=np.uint8)
    memory.add(vec)
    if (i + 1) % 20_000 == 0:
        print(f"   Added {i+1:,} vectors")

elapsed = time.time() - start
print(f"   Completed in {elapsed:.2f} seconds")
print(f"   Storage: {memory.size_mb():.2f} MB")

# Step 3: Create Query
print("\n3. Creating query vector...")
query = np.random.randint(0, 2, size=DIMENSION, dtype=np.uint8)

# Step 4: Two-Stage Retrieval
print(f"\n4. Performing retrieval...")
print(f"   Stage 1: Finding top-{K_APPROXIMATE} candidates...")
start = time.time()
candidates, approx_distances = HammingTopK.search(
    memory, query, k=K_APPROXIMATE
)
stage1_time = time.time() - start
print(f"   Stage 1 time: {stage1_time*1000:.2f} ms")

print(f"   Stage 2: Refining to top-{K_FINAL} results...")
start = time.time()
refined = HingeZeroRefinement.refine(
    memory=memory,
    query=query,
    candidate_indices=candidates,
    top_k=K_FINAL
)
stage2_time = time.time() - start
print(f"   Stage 2 time: {stage2_time*1000:.2f} ms")

# Step 5: Results
print(f"\n5. Results:")
print(f"   Top-{K_FINAL} indices: {refined[:K_FINAL]}")
print(f"   Total retrieval time: {(stage1_time + stage2_time)*1000:.2f} ms")

# Step 6: Robustness Test
print(f"\n6. Testing robustness to bit corruption...")
corruption_rates = [0.0, 0.1, 0.25, 0.5]
for rate in corruption_rates:
    memory_test = memory.copy()
    memory_test.corrupt_bits(corruption_rate=rate)
    candidates, _ = HammingTopK.search(memory_test, query, k=K_APPROXIMATE)
    print(f"   Corruption {rate*100:5.1f}%: {len(candidates)} candidates found")

print("\n" + "=" * 60)
print("Example Complete!")
print("=" * 60)
```

## Tips and Best Practices

1. **Batch Operations**: Process multiple queries in batches for better performance
2. **Two-Stage Retrieval**: Use approximate search + refinement for best accuracy/speed tradeoff
3. **Corruption Testing**: Always test with realistic corruption rates for your use case
4. **Memory Management**: Monitor `memory.size_mb()` to stay within device limits
5. **Query Preparation**: Ensure queries are proper 1-bit vectors (values 0 or 1)

For more information, see the [README](README.md) and [installation guide](INSTALL.md).
