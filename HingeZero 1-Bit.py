import numpy as np

# ============================================================
# HINGEZERO 1-BIT PACKED MEMORY BASELINE
# Packed storage + Hamming Top-K + HZ refinement on candidates
# ============================================================

POPCOUNT = np.array([bin(i).count("1") for i in range(256)], dtype=np.uint8)

def l2_normalize(x, eps=1e-9):
    x = np.asarray(x, dtype=np.float32)
    return x / (np.linalg.norm(x) + eps)

def hinge_phi(h, alpha=0.25):
    return np.tanh(h) + alpha * np.tanh(2.0 * h)

def bipolar_to_bits(x):
    return (np.asarray(x) >= 0).astype(np.uint8)

def bits_to_bipolar(bits):
    return bits.astype(np.float32) * 2.0 - 1.0

def pack_bits(bits):
    return np.packbits(bits.astype(np.uint8), axis=1)

def unpack_rows(packed_rows, n_bits):
    bits = np.unpackbits(packed_rows, axis=1)[:, :n_bits]
    return bits_to_bipolar(bits)

def hamming_matches(packed_memory, packed_query, n_bits):
    xor = np.bitwise_xor(packed_memory, packed_query)
    mismatches = POPCOUNT[xor].sum(axis=1)
    return n_bits - mismatches

class HingeZero1BitMemory:
    def __init__(self, n_bits):
        self.n_bits = int(n_bits)
        self.packed = None

    def fit(self, bipolar_memory):
        bits = bipolar_to_bits(bipolar_memory)
        self.packed = pack_bits(bits)
        return self

    def recall(self, query, top_k=64, steps=12, alpha=0.25, eps=0.10, lam=0.02, beta=8.0):
        q_bits = bipolar_to_bits(query.reshape(1, -1))
        q_pack = pack_bits(q_bits)[0]

        matches = hamming_matches(self.packed, q_pack, self.n_bits)

        k = min(int(top_k), len(matches))
        idx = np.argpartition(-matches, k - 1)[:k]
        idx = idx[np.argsort(-matches[idx])]

        candidates = unpack_rows(self.packed[idx], self.n_bits)
        cand_norm = candidates / (np.linalg.norm(candidates, axis=1, keepdims=True) + 1e-9)

        x = l2_normalize(np.asarray(query, dtype=np.float32))

        for _ in range(int(steps)):
            q = l2_normalize(x)
            sims = cand_norm @ q

            z = beta * (sims - np.max(sims))
            w = np.exp(z)
            w /= np.sum(w) + 1e-9

            h = w @ candidates
            x = (1.0 - lam) * x + eps * hinge_phi(h, alpha)
            x = l2_normalize(x)

        final_bits = bipolar_to_bits(x.reshape(1, -1))
        final_pack = pack_bits(final_bits)[0]
        final_matches = hamming_matches(self.packed, final_pack, self.n_bits)

        order = np.argsort(-final_matches)
        best = int(order[0])
        second = int(order[1])

        return {
            "index": best,
            "match": int(final_matches[best]),
            "margin_bits": int(final_matches[best] - final_matches[second]),
            "topk_indices": idx,
            "topk_matches": matches[idx],
            "recalled": x,
        }

# ============================================================
# TEST
# ============================================================

rng = np.random.default_rng(0)

N = 512
P = 1_000_000
TRIALS = 50
TOP_K = 64
NOISE_FLIP = 0.25

print("=== HingeZero 1-bit packed test ===")
print(f"N={N} | P={P:,} | trials={TRIALS} | top_k={TOP_K} | bit_flip={NOISE_FLIP}")

memory = rng.choice([-1.0, 1.0], size=(P, N)).astype(np.float32)

hz = HingeZero1BitMemory(n_bits=N)
hz.fit(memory)

print("packed_MB:", round(hz.packed.nbytes / 1024 / 1024, 2))

correct = 0

for t in range(TRIALS):
    target_id = int(rng.integers(0, P))
    target = memory[target_id].copy()

    bits = bipolar_to_bits(target.reshape(1, -1))[0]
    flip = rng.random(N) < NOISE_FLIP
    bits[flip] ^= 1

    query = bits_to_bipolar(bits)

    result = hz.recall(query, top_k=TOP_K)

    hit = result["index"] == target_id
    correct += int(hit)

    if t < 10:
        print(
            f"[{t+1:03d}] true={target_id} pred={result['index']} "
            f"hit={hit} match={result['match']}/{N} margin={result['margin_bits']}"
        )

print("\n=== RESULTS ===")
print("top1_accuracy:", round(correct / TRIALS, 4))
