## HingeZero-1Bit Demo

This repository now includes a runnable browser demo for the project concept.

### Open the demo

- Open `index.html` directly in a browser, or
- serve the folder from a local static server:

```bash
cd HingeZero-1Bit
python -m http.server 8000
```

Then open:

```text
http://localhost:8000
```

### What the demo shows

- packed 1-bit storage footprint
- robustness under bit corruption
- a simple comparison between a legacy dense retrieval workflow and HingeZero 1-bit search
- execution-time estimates for large associative memory workloads

### Notes

This is a web demo intended to visualize the design and performance story. It is intentionally lightweight and browser-friendly so it can run without a complex backend or GPU setup.

---

The original algorithm and benchmark logic remain in [`HingeZero 1-Bit.py`](HingeZero%201-Bit.py).
