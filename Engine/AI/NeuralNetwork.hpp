// Engine/AI/NeuralNetwork.hpp
#pragma once

#include "Core/Platform.hpp"
#include "Simulation/DeterministicRng.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

namespace Shape::AI {

enum class ActivationFunction {
    Linear,
    ReLU,
    Tanh,
    Sigmoid
};

class NeuralNetwork {
public:
    NeuralNetwork() = default;

    /// Construct network with layer topology (e.g. {8, 16, 4})
    explicit NeuralNetwork(const std::vector<usize>& layer_sizes,
                           ActivationFunction activation = ActivationFunction::Tanh)
        : m_layer_sizes(layer_sizes), m_activation(activation) {
        initialize_weights();
    }

    void initialize(const std::vector<usize>& layer_sizes,
                    ActivationFunction activation = ActivationFunction::Tanh) {
        m_layer_sizes = layer_sizes;
        m_activation = activation;
        initialize_weights();
    }

    /// Forward pass inference: input_vector -> output_vector
    std::vector<f32> Evaluate(const std::vector<f32>& inputs) const {
        if (m_layer_sizes.size() < 2 || inputs.size() != m_layer_sizes[0]) {
            return {};
        }

        std::vector<f32> current_activations = inputs;

        for (usize layer = 0; layer < m_weights.size(); ++layer) {
            const usize next_size = m_layer_sizes[layer + 1];
            std::vector<f32> next_activations(next_size, 0.0f);

            for (usize j = 0; j < next_size; ++j) {
                f32 sum = m_biases[layer][j];
                for (usize i = 0; i < current_activations.size(); ++i) {
                    sum += current_activations[i] * m_weights[layer][j][i];
                }
                // Apply activation (except linear for output if preferred)
                next_activations[j] = activate(sum, (layer == m_weights.size() - 1));
            }
            current_activations = std::move(next_activations);
        }

        return current_activations;
    }

    /// Mutate weights and biases deterministically using DeterministicRng
    void Mutate(Simulation::DeterministicRng& rng, f32 mutation_rate = 0.1f, f32 mutation_strength = 0.2f) {
        for (auto& layer_weights : m_weights) {
            for (auto& neuron_weights : layer_weights) {
                for (auto& w : neuron_weights) {
                    if (rng.NextBool(mutation_rate)) {
                        f32 delta = rng.NextRangeF32(-mutation_strength, mutation_strength);
                        w += delta;
                    }
                }
            }
        }

        for (auto& layer_biases : m_biases) {
            for (auto& b : layer_biases) {
                if (rng.NextBool(mutation_rate)) {
                    f32 delta = rng.NextRangeF32(-mutation_strength, mutation_strength);
                    b += delta;
                }
            }
        }
    }

    /// Two-point crossover between this network and parent_b
    static NeuralNetwork Crossover(const NeuralNetwork& parent_a, const NeuralNetwork& parent_b,
                                   Simulation::DeterministicRng& rng) {
        if (parent_a.m_layer_sizes != parent_b.m_layer_sizes) {
            return parent_a;
        }

        NeuralNetwork child = parent_a;
        for (usize layer = 0; layer < child.m_weights.size(); ++layer) {
            for (usize j = 0; j < child.m_weights[layer].size(); ++j) {
                for (usize i = 0; i < child.m_weights[layer][j].size(); ++i) {
                    if (rng.NextBool(0.5f)) {
                        child.m_weights[layer][j][i] = parent_b.m_weights[layer][j][i];
                    }
                }
                if (rng.NextBool(0.5f)) {
                    child.m_biases[layer][j] = parent_b.m_biases[layer][j];
                }
            }
        }
        return child;
    }

    const std::vector<usize>& layer_sizes() const noexcept { return m_layer_sizes; }
    ActivationFunction activation() const noexcept { return m_activation; }

private:
    void initialize_weights() {
        m_weights.clear();
        m_biases.clear();

        if (m_layer_sizes.size() < 2) return;

        m_weights.resize(m_layer_sizes.size() - 1);
        m_biases.resize(m_layer_sizes.size() - 1);

        // Simple Xavier/Glorot-like deterministic initialization using fixed seed RNG
        Simulation::DeterministicRng init_rng(1337ULL + m_layer_sizes.size());

        for (usize layer = 0; layer < m_layer_sizes.size() - 1; ++layer) {
            const usize in_size = m_layer_sizes[layer];
            const usize out_size = m_layer_sizes[layer + 1];

            f32 scale = std::sqrt(2.0f / static_cast<f32>(in_size + out_size));

            m_weights[layer].resize(out_size, std::vector<f32>(in_size, 0.0f));
            m_biases[layer].resize(out_size, 0.0f);

            for (usize j = 0; j < out_size; ++j) {
                m_biases[layer][j] = init_rng.NextRangeF32(-0.1f, 0.1f);
                for (usize i = 0; i < in_size; ++i) {
                    m_weights[layer][j][i] = init_rng.NextRangeF32(-scale, scale);
                }
            }
        }
    }

    f32 activate(f32 x, bool is_output_layer) const {
        if (is_output_layer && m_activation != ActivationFunction::Linear) {
            // Tanh for bounded output [-1, 1]
            return std::tanh(x);
        }

        switch (m_activation) {
            case ActivationFunction::Linear:  return x;
            case ActivationFunction::ReLU:    return std::max(0.0f, x);
            case ActivationFunction::Tanh:    return std::tanh(x);
            case ActivationFunction::Sigmoid: return 1.0f / (1.0f + std::exp(-x));
        }
        return x;
    }

    std::vector<usize> m_layer_sizes;
    ActivationFunction m_activation = ActivationFunction::Tanh;
    std::vector<std::vector<std::vector<f32>>> m_weights; // [layer][neuron][input]
    std::vector<std::vector<f32>> m_biases;              // [layer][neuron]
};

} // namespace Shape::AI
