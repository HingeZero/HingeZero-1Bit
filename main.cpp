// OpenMP benchmark for HingeZero 1-bit retrieval.
// Build: g++ -O3 -std=c++17 -fopenmp main.cpp -o hingezero_benchmark
// The implementation is included so the requested two files form a complete
// standalone example. Do not compile hingezero_core.cpp as a second translation
// unit unless it is first split into a header and source file.
#include "hingezero_core.cpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

int main() {
    constexpr std::size_t dimensions = 512;
    constexpr std::size_t memories = 100000;
    constexpr std::size_t trials = 20;
    constexpr std::size_t top_k = 64;
    constexpr float noise_flip_probability = 0.25f;

    std::mt19937 rng(0);
    std::uniform_int_distribution<int> bit(0, 1);
    std::bernoulli_distribution flip(noise_flip_probability);
    std::vector<std::vector<float>> memory(memories, std::vector<float>(dimensions));
    for (auto& row : memory) for (float& value : row) value = bit(rng) ? 1.0f : -1.0f;

    HingeZero1BitMemory hz(dimensions);
    const auto fit_start = std::chrono::steady_clock::now();
    hz.fit(memory);
    const auto fit_end = std::chrono::steady_clock::now();

    std::cout << "=== HingeZero 1-bit OpenMP benchmark ===\n"
              << "N=" << dimensions << " | P=" << memories
              << " | trials=" << trials << " | top_k=" << top_k << "\n"
              << "packed storage: " << std::fixed << std::setprecision(2)
              << static_cast<double>(hz.packed_bytes()) / (1024.0 * 1024.0) << " MiB\n"
              << "fit time: " << std::chrono::duration<double, std::milli>(fit_end - fit_start).count() << " ms\n";

    std::size_t correct = 0;
    double total_ms = 0.0;
    for (std::size_t trial = 0; trial < trials; ++trial) {
        const std::size_t target = (trial * 7919) % memories;
        std::vector<float> query = memory[target];
        for (float& value : query) if (flip(rng)) value = -value;

        const auto start = std::chrono::steady_clock::now();
        const HingeZeroResult result = hz.recall(query, top_k);
        const auto end = std::chrono::steady_clock::now();
        const double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        total_ms += elapsed_ms;
        correct += result.index == target;

        if (trial < 5) {
            std::cout << "[" << trial + 1 << "] target=" << target << " predicted=" << result.index
                      << " hit=" << (result.index == target ? "true" : "false")
                      << " match=" << result.match << "/" << dimensions
                      << " margin=" << result.margin_bits << " time=" << elapsed_ms << " ms\n";
        }
    }

    std::cout << "top-1 accuracy: " << static_cast<double>(correct) / trials << "\n"
              << "average recall: " << total_ms / trials << " ms\n"
              << "OpenMP max threads: " << omp_get_max_threads() << "\n";
    return 0;
}
