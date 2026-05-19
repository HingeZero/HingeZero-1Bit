# HingeZero-1Bit

[![Python 3.8+](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green.svg)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/HingeZero/HingeZero-1Bit.svg?style=social)](https://github.com/HingeZero/HingeZero-1Bit)

Packed 1-bit memory retrieval with Hamming Top-K search and HingeZero refinement for efficient and edge-oriented associative memory systems.

## 🚀 Overview

HingeZero-1Bit explores packed 1-bit associative memory retrieval, combining extreme compression with efficient retrieval mechanisms suitable for edge devices and memory-constrained systems.

### Key Features

- **Packed 1-bit Memory Storage** - Extreme compression with minimal footprint (~61 MB for 1M memories at 512 dimensions)
- **Hamming Top-K Search** - Fast approximate nearest neighbor retrieval using Hamming distance
- **Local HingeZero Refinement** - High-precision refinement on candidate results
- **Edge-Oriented Design** - Low memory operation for deployment on resource-constrained devices
- **Robust to Bit Corruption** - Maintains 100% Top-1 accuracy even with 25% bit corruption

### Performance Example

```
Dimensions: 512
Memories: 1,000,000
Packed Storage: ≈61 MB
Bit Corruption: 25%
Top-1 Accuracy: 1.0
```

## 📋 Table of Contents

- [Installation](#installation)
- [Quick Start](#quick-start)
- [Architecture](#architecture)
- [Usage](#usage)
- [Demo](#demo)
- [Future Work](#future-work)
- [Contributing](#contributing)
- [License](#license)

## 🔧 Installation

### Requirements

- Python 3.8 or higher
- NumPy
- (Optional) Jupyter for running the demo notebook

### From Source

```bash
git clone https://github.com/HingeZero/HingeZero-1Bit.git
cd HingeZero-1Bit
pip install -r requirements.txt
```

## ⚡ Quick Start

```python
from core import PackedMemory, HammmingTopK, HingeZeroRefinement

# Create a packed memory system
memory = PackedMemory(dimension=512, max_memories=1_000_000)

# Add memories (1-bit vectors)
memory.add(query_vector)

# Retrieve with Hamming Top-K
top_k_indices = HammingTopK.search(query, k=10)

# Refine results with HingeZero
refined_results = HingeZeroRefinement.refine(top_k_indices)
```

For a complete working example, see [HingeZero_1Bit_Demo.ipynb](HingeZero_1Bit_Demo.ipynb).

## 🏗️ Architecture

The retrieval pipeline follows this flow:

```
Packed 1-bit Memory Storage
         ↓
    Hamming Top-K Search
         ↓
   Unpack Top-K Candidates
         ↓
  HingeZero Refinement
         ↓
      Recall Results
```

### How It Works

1. **Packed Storage**: Memories stored as 1-bit vectors, packed efficiently to minimize memory usage
2. **Fast Search**: Hamming distance calculation on packed bits for rapid Top-K identification
3. **Selective Unpacking**: Only top candidates unpacked to full precision
4. **Refinement**: HingeZero algorithm refines results for high accuracy
5. **Recall**: Final results optimized for recall performance

## 📖 Usage

### Basic Memory Operations

```python
from core import PackedMemory

# Initialize
memory = PackedMemory(dimension=512, max_memories=1_000_000)

# Store a memory
memory.add(vector_1bit)

# Check storage size
print(f"Storage: {memory.size_mb()} MB")
```

### Hamming Top-K Retrieval

```python
from core import HammingTopK

# Search for top-k similar vectors
query = generate_1bit_query()
top_indices, distances = HammingTopK.search(query, k=100)
```

### HingeZero Refinement

```python
from core import HingeZeroRefinement

# Refine top-k results for higher accuracy
refined = HingeZeroRefinement.refine(top_indices, memory)
```

### Robustness Testing

Test memory system under bit corruption:

```python
memory.corrupt_bits(corruption_rate=0.25)
results = memory.search(query, k=10)
# Maintains high accuracy despite corruption
```

## 🎓 Demo

Interactive Jupyter notebook demonstrating the full pipeline:

[**HingeZero_1Bit_Demo.ipynb**](HingeZero_1Bit_Demo.ipynb)

The notebook includes:
- Memory initialization and configuration
- Retrieval performance metrics
- Corruption robustness testing
- Comparative analysis
- Visualization of results

## 🔮 Future Work

- [ ] Larger dimensions (1024 / 2048 / 4096)
- [ ] Billion-scale streamed retrieval
- [ ] Edge deployment optimization (mobile, IoT)
- [ ] int8 / int4 / int2 quantization comparisons
- [ ] Hardware acceleration (GPU/TPU support)
- [ ] Streaming memory updates
- [ ] Distributed retrieval systems

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Ways to Contribute

- Report bugs and issues
- Suggest new features or optimizations
- Improve documentation
- Add tests and examples
- Performance optimizations
- Hardware acceleration implementations

## 📄 License

This project is licensed under the GNU General Public License v3.0 - see [LICENSE](LICENSE) file for details.

## ⭐ If You Find This Useful

Please consider starring the repository! Your support helps increase visibility and encourages further development.

## 📚 References

- Hamming distance in binary retrieval
- Associative memory systems
- 1-bit neural networks and quantization
- Edge computing optimization

---

**Questions or Ideas?** Open an [issue](https://github.com/HingeZero/HingeZero-1Bit/issues) or start a [discussion](https://github.com/HingeZero/HingeZero-1Bit/discussions)!
