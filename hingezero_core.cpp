// HingeZero 1-bit packed associative memory.
//
// This is a self-contained C++17 implementation of the reference Python
// algorithm: packed bipolar vectors, Hamming candidate selection, and the
// HingeZero refinement loop. The hot loops are OpenMP-parallelized.

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

struct HingeZeroResult {
    std::size_t index = 0;
    int match = 0;
    int margin_bits = 0;
    std::vector<std::size_t> topk_indices;
    std::vector<int> topk_matches;
    std::vector<float> recalled;
};

class HingeZero1BitMemory {
public:
    explicit HingeZero1BitMemory(std::size_t n_bits)
        : n_bits_(n_bits), row_bytes_((n_bits + 7) / 8) {
        if (n_bits_ == 0) {
            throw std::invalid_argument("n_bits must be greater than zero");
        }
    }

    void fit(const std::vector<std::vector<float>>& bipolar_memory) {
        if (bipolar_memory.empty()) {
            packed_.clear();
            return;
        }
        for (const auto& row : bipolar_memory) {
            if (row.size() != n_bits_) {
                throw std::invalid_argument("every memory vector must have n_bits elements");
            }
        }

        packed_.assign(bipolar_memory.size() * row_bytes_, 0);
        #pragma omp parallel for schedule(static)
        for (long long i = 0; i < static_cast<long long>(bipolar_memory.size()); ++i) {
            auto* destination = packed_.data() + static_cast<std::size_t>(i) * row_bytes_;
            for (std::size_t bit = 0; bit < n_bits_; ++bit) {
                if (bipolar_memory[static_cast<std::size_t>(i)][bit] >= 0.0f) {
                    destination[bit / 8] |= static_cast<std::uint8_t>(1u << (7u - bit % 8));
                }
            }
        }
    }

    std::size_t size() const { return packed_.size() / row_bytes_; }
    std::size_t n_bits() const { return n_bits_; }
    std::size_t packed_bytes() const { return packed_.size(); }

    HingeZeroResult recall(const std::vector<float>& query,
                           std::size_t top_k = 64,
                           int steps = 12,
                           float alpha = 0.25f,
                           float eps = 0.10f,
                           float lambda = 0.02f,
                           float beta = 8.0f) const {
        if (query.size() != n_bits_) {
            throw std::invalid_argument("query must have n_bits elements");
        }
        if (packed_.empty() || top_k == 0) {
            throw std::invalid_argument("memory must be non-empty and top_k must be greater than zero");
        }

        std::vector<std::uint8_t> query_packed(row_bytes_, 0);
        for (std::size_t bit = 0; bit < n_bits_; ++bit) {
            if (query[bit] >= 0.0f) {
                query_packed[bit / 8] |= static_cast<std::uint8_t>(1u << (7u - bit % 8));
            }
        }

        const std::size_t count = size();
        std::vector<int> matches(count, 0);
        #pragma omp parallel for schedule(static)
        for (long long i = 0; i < static_cast<long long>(count); ++i) {
            const auto* row = packed_.data() + static_cast<std::size_t>(i) * row_bytes_;
            int distance = 0;
            for (std::size_t byte = 0; byte < row_bytes_; ++byte) {
                distance += __builtin_popcount(static_cast<unsigned int>(row[byte] ^ query_packed[byte]));
            }
            matches[static_cast<std::size_t>(i)] = static_cast<int>(n_bits_) - distance;
        }

        const std::size_t k = std::min(top_k, count);
        std::vector<std::size_t> candidates(count);
        std::iota(candidates.begin(), candidates.end(), 0);
        std::partial_sort(candidates.begin(), candidates.begin() + k, candidates.end(),
                          [&](std::size_t a, std::size_t b) { return matches[a] > matches[b]; });
        candidates.resize(k);

        std::vector<std::vector<float>> decoded(k, std::vector<float>(n_bits_));
        for (std::size_t c = 0; c < k; ++c) {
            const auto* row = packed_.data() + candidates[c] * row_bytes_;
            for (std::size_t bit = 0; bit < n_bits_; ++bit) {
                decoded[c][bit] = (row[bit / 8] & (1u << (7u - bit % 8))) ? 1.0f : -1.0f;
            }
        }

        std::vector<float> norms(k, 0.0f);
        for (std::size_t c = 0; c < k; ++c) {
            for (float value : decoded[c]) norms[c] += value * value;
            norms[c] = std::sqrt(norms[c]) + 1e-9f;
        }

        std::vector<float> x = query;
        normalize(x);
        for (int step = 0; step < steps; ++step) {
            std::vector<float> q = x;
            normalize(q);
            std::vector<float> weights(k);
            float max_similarity = -std::numeric_limits<float>::infinity();
            for (std::size_t c = 0; c < k; ++c) {
                float similarity = 0.0f;
                for (std::size_t bit = 0; bit < n_bits_; ++bit) similarity += decoded[c][bit] * q[bit] / norms[c];
                weights[c] = similarity;
                max_similarity = std::max(max_similarity, similarity);
            }
            float weight_sum = 0.0f;
            for (float& weight : weights) { weight = std::exp(beta * (weight - max_similarity)); weight_sum += weight; }
            std::vector<float> h(n_bits_, 0.0f);
            for (std::size_t c = 0; c < k; ++c) for (std::size_t bit = 0; bit < n_bits_; ++bit) h[bit] += weights[c] * decoded[c][bit] / weight_sum;
            for (std::size_t bit = 0; bit < n_bits_; ++bit) x[bit] = (1.0f - lambda) * x[bit] + eps * (std::tanh(h[bit]) + alpha * std::tanh(2.0f * h[bit]));
            normalize(x);
        }

        std::vector<std::pair<int, std::size_t>> final_order;
        final_order.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            int match = 0;
            const auto* row = packed_.data() + i * row_bytes_;
            for (std::size_t bit = 0; bit < n_bits_; ++bit) {
                const bool stored = (row[bit / 8] & (1u << (7u - bit % 8))) != 0;
                if (stored == (x[bit] >= 0.0f)) ++match;
            }
            final_order.emplace_back(match, i);
        }
        std::partial_sort(final_order.begin(), final_order.begin() + std::min<std::size_t>(2, count), final_order.end(),
                          [](const auto& a, const auto& b) { return a.first > b.first; });

        HingeZeroResult result;
        result.index = final_order[0].second;
        result.match = final_order[0].first;
        result.margin_bits = count > 1 ? final_order[0].first - final_order[1].first : final_order[0].first;
        result.recalled = std::move(x);
        result.topk_indices = candidates;
        result.topk_matches.reserve(k);
        for (auto index : candidates) result.topk_matches.push_back(matches[index]);
        return result;
    }

private:
    static void normalize(std::vector<float>& values) {
        float norm = 0.0f;
        for (float value : values) norm += value * value;
        norm = std::sqrt(norm) + 1e-9f;
        for (float& value : values) value /= norm;
    }

    std::size_t n_bits_;
    std::size_t row_bytes_;
    std::vector<std::uint8_t> packed_;
};
