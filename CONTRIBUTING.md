# Contributing to HingeZero-1Bit

Thank you for your interest in contributing to HingeZero-1Bit! We welcome all kinds of contributions, from bug reports to feature implementations.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Ways to Contribute](#ways-to-contribute)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Features](#suggesting-features)
- [Questions or Need Help?](#questions-or-need-help)

## Code of Conduct

Be respectful, inclusive, and constructive in all interactions. We're building a community dedicated to advancing memory systems research and development.

## Ways to Contribute

### 🐛 Report Bugs
Found a bug? Please open an issue with:
- Clear description of the problem
- Steps to reproduce
- Expected vs. actual behavior
- Python version and OS
- Relevant code snippets

### ✨ Suggest Features
Have an idea? Open a discussion or issue with:
- Clear description of the feature
- Use case and benefits
- Potential implementation approach
- Any relevant references or papers

### 📖 Improve Documentation
- Fix typos or unclear sections
- Add examples or diagrams
- Improve README or guides
- Add code comments
- Create tutorials

### 🔍 Code Review
Review pull requests and provide feedback on:
- Code quality and style
- Performance implications
- Documentation completeness

### ⚡ Optimize & Improve
- Performance optimizations
- Memory efficiency improvements
- Code refactoring
- Test coverage improvements

### 🚀 Implement Features
Pick from the [Future Work](README.md#-future-work) section:
- [ ] Larger dimensions support (1024/2048/4096)
- [ ] Billion-scale streaming
- [ ] Edge deployment
- [ ] Quantization comparisons
- [ ] Hardware acceleration

## Getting Started

### 1. Fork the Repository

```bash
# Go to https://github.com/HingeZero/HingeZero-1Bit
# Click "Fork" button
```

### 2. Clone Your Fork

```bash
git clone https://github.com/YOUR_USERNAME/HingeZero-1Bit.git
cd HingeZero-1Bit
```

### 3. Add Upstream Remote

```bash
git remote add upstream https://github.com/HingeZero/HingeZero-1Bit.git
```

### 4. Set Up Development Environment

```bash
# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt
pip install -r requirements-dev.txt  # Development tools
```

## Development Workflow

### Create a Branch

Use descriptive branch names:

```bash
# For bug fixes
git checkout -b fix/issue-description

# For features
git checkout -b feature/feature-name

# For documentation
git checkout -b docs/what-you-updated
```

### Make Changes

- Keep commits atomic and focused
- Write clear commit messages
- Add tests for new features
- Update documentation as needed

### Run Tests

```bash
# Run all tests
python -m pytest

# Run specific test
python -m pytest tests/test_core.py

# Run with coverage
python -m pytest --cov=core
```

### Verify Code Quality

```bash
# Check style
pylint core.py

# Format code
black core.py

# Type checking
mypy core.py
```

## Commit Guidelines

Write clear, descriptive commit messages:

```
[type]: Brief description (50 chars max)

Longer explanation if needed (wrap at 72 chars)

- Bullet points for multiple changes
- Reference issues: Fixes #123
- Reference PRs: Related to #456
```

### Commit Types

- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation
- `test:` Tests
- `refactor:` Code refactoring
- `perf:` Performance improvement
- `ci:` CI/CD changes
- `chore:` Maintenance

### Examples

```
feat: Add GPU acceleration for Hamming distance

Implements CUDA kernels for faster Top-K search on NVIDIA GPUs.
Provides 10x speedup on large-scale queries.

Fixes #42
```

```
fix: Handle empty memory edge case

Previously caused IndexError when searching empty memory.
Now returns empty results gracefully.

Fixes #89
```

## Pull Request Process

### 1. Push to Your Fork

```bash
git push origin your-branch-name
```

### 2. Create Pull Request

- Go to https://github.com/HingeZero/HingeZero-1Bit
- Click "New Pull Request"
- Select your branch

### 3. PR Description Template

```markdown
## Description
Brief explanation of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation
- [ ] Performance improvement
- [ ] Other

## Related Issues
Fixes #(issue number)

## Changes
- Change 1
- Change 2
- Change 3

## Testing
- [ ] Added tests
- [ ] Updated tests
- [ ] All tests pass

## Documentation
- [ ] Updated README
- [ ] Updated comments
- [ ] New docs added

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] No new warnings generated
- [ ] Tests pass locally
```

### 4. Response to Feedback

- Respond constructively to all feedback
- Make requested changes
- Push updates to the same branch
- Don't force-push (let history show the discussion)

## Reporting Bugs

### Issue Template

```markdown
## Description
Clear description of the bug

## Steps to Reproduce
1. Do this
2. Then this
3. Finally this

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- Python version: 3.x.x
- OS: (Windows/macOS/Linux)
- NumPy version: x.x.x

## Code Example
```python
# Minimal example that reproduces the bug
```

## Additional Context
Any other relevant information
```

## Suggesting Features

### Feature Request Template

```markdown
## Description
Clear description of the feature

## Motivation
Why this feature is needed

## Proposed Solution
How you'd implement it

## Alternative Approaches
Other possible solutions

## Additional Context
References, papers, related work

## Priority
- [ ] High (Core functionality)
- [ ] Medium (Important enhancement)
- [ ] Low (Nice to have)
```

## Questions or Need Help?

- **Usage Questions**: Open a [Discussion](https://github.com/HingeZero/HingeZero-1Bit/discussions)
- **Bugs**: Open an [Issue](https://github.com/HingeZero/HingeZero-1Bit/issues)
- **General Questions**: Start a [Discussion](https://github.com/HingeZero/HingeZero-1Bit/discussions)

## Recognition

Contributors are recognized in:
- README.md contributors section
- Release notes for significant contributions
- GitHub contributor graph

## Priority Areas for Contribution

### 🔴 High Priority
- [ ] GPU acceleration (CUDA/OpenCL)
- [ ] Billion-scale performance
- [ ] Edge deployment (mobile/IoT)
- [ ] Critical bug fixes

### 🟡 Medium Priority
- [ ] Additional quantization methods
- [ ] Performance benchmarking
- [ ] Documentation improvements
- [ ] Example notebooks

### 🟢 Low Priority
- [ ] Code style improvements
- [ ] UI/visualization enhancements
- [ ] Minor optimizations

---

**Thank you for contributing to HingeZero-1Bit!** 🙏

Your work helps advance memory systems research and makes this project better for everyone.
