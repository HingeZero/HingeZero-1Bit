# Installation Guide

## System Requirements

- **Python**: 3.8 or higher
- **OS**: Linux, macOS, or Windows
- **RAM**: Minimum 2 GB (more for working with large memory systems)

## Dependencies

- `numpy` - For numerical operations
- `jupyter` - (Optional) For running the demo notebook

## Installation Methods

### Method 1: Clone from GitHub (Recommended)

```bash
# Clone the repository
git clone https://github.com/HingeZero/HingeZero-1Bit.git

# Navigate to the directory
cd HingeZero-1Bit

# Install dependencies
pip install -r requirements.txt
```

### Method 2: Direct Download

1. Download the repository as ZIP from [GitHub](https://github.com/HingeZero/HingeZero-1Bit)
2. Extract the archive
3. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

## Verify Installation

Test that the installation was successful:

```python
from core import PackedMemory, HammingTopK, HingeZeroRefinement

print("✓ HingeZero-1Bit successfully installed!")
```

## Optional: Jupyter Notebook Setup

To run the demo notebook, install Jupyter:

```bash
pip install jupyter
jupyter notebook HingeZero_1Bit_Demo.ipynb
```

## Troubleshooting

### ImportError: No module named 'numpy'

```bash
pip install numpy
```

### ImportError: No module named 'core'

Make sure you're running Python from the project root directory where `core.py` is located.

### Issues on Windows

If you encounter issues on Windows:

```bash
# Try using python instead of python3
python -m pip install -r requirements.txt
```

## Development Setup

For contributing to the project:

```bash
# Clone the repository
git clone https://github.com/HingeZero/HingeZero-1Bit.git
cd HingeZero-1Bit

# Install in development mode
pip install -e .

# Install development dependencies
pip install -r requirements-dev.txt
```

## Getting Help

- Check the [README](README.md) for general information
- See [EXAMPLES.md](EXAMPLES.md) for code examples
- Open an [issue](https://github.com/HingeZero/HingeZero-1Bit/issues) for problems
- Start a [discussion](https://github.com/HingeZero/HingeZero-1Bit/discussions) for questions
