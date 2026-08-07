# 29. Neural Network & Machine Learning Systems

## Overview

The machine learning layer of the Primodilas engine provides data-driven entity intelligence as a complement to rule-based decision making. Two cooperating systems implement this: a general-purpose feedforward `NeuralNetwork` and a reinforcement learning `QLearningSystem`.

---

## NeuralNetwork (`Engine/AI/NeuralNetwork.hpp`)

A lightweight, header-only multi-layer perceptron (MLP) written in C++23.

### Architecture

```
Input Layer → [Hidden Layer(s)] → Output Layer
```

- Layers are defined as `std::vector<std::vector<float>>` weight matrices.
- Each layer has a bias vector.
- Activation functions per layer: `Sigmoid`, `Tanh`, `ReLU`.

### Key API

| Method | Description |
|---|---|
| `NeuralNetwork(topology, activation)` | Constructs network with given layer sizes |
| `Forward(inputs)` | Runs forward pass, returns output activations |
| `Train(inputs, targets, lr)` | Backpropagation gradient descent step |
| `SetWeights(layer, weights)` | Direct weight injection for saved models |

### Forward Pass

```cpp
NeuralNetwork net({4, 8, 4, 2}, Activation::ReLU);
auto output = net.Forward({hunger, fear, energy, proximity});
// output[0] = move probability, output[1] = eat probability
```

### Training

Uses mean squared error (MSE) loss with gradient descent. Suitable for supervised behaviour cloning from a reference policy.

---

## NeuralControlSystem (`Engine/Simulation/Systems/NeuralControlSystem.h`)

An `ISimulationSystem` that drives entity decisions using neural inference.

### Behaviour

Each tick:
1. Reads `MindComponent` sensor fields (hunger, fear, energy, perceived neighbours).
2. Runs `NeuralNetwork::Forward()` with sensor data as inputs.
3. Maps output activations to discrete entity actions (move, eat, flee, rest).
4. Writes chosen action back to `MindComponent::current_action`.

### Integration

```cpp
// Register as a simulation system
scheduler.Register<NeuralControlSystem>(&world, &net);
```

Priority: `80` (after perception, before physics application).

---

## QLearningSystem (`Engine/Simulation/Systems/QLearningSystem.h`)

A tabular reinforcement learning system using Q-learning (temporal difference).

### QBrainComponent

Each learning entity carries a `QBrainComponent`:

```cpp
struct QBrainComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::QBrainComponent);
    std::vector<float> q_table;   // [state_count × action_count]
    uint32_t state_count  = 8;
    uint32_t action_count = 4;
    float epsilon = 0.2f;          // Exploration rate
    float alpha   = 0.1f;          // Learning rate
    float gamma   = 0.9f;          // Discount factor
    uint32_t last_state  = 0;
    uint32_t last_action = 0;
    float last_reward    = 0.0f;
};
```

### Q-Update Rule

$$Q(s, a) \leftarrow Q(s, a) + \alpha \left[ r + \gamma \max_{a'} Q(s', a') - Q(s, a) \right]$$

### Epsilon-Greedy Policy

- With probability `epsilon`: choose a random action (exploration).
- Otherwise: choose `argmax Q(s, a)` (exploitation).
- `epsilon` decays over time toward a minimum floor.

### Bounds Safety

`get_q(state, action)` and `set_q(state, action, value)` clamp both indices to `[0, state_count-1]` and `[0, action_count-1]` preventing out-of-bounds Q-table access.

---

## Test Coverage

| Test File | Cases |
|---|---|
| `EngineTests/AI/test_neural_network.cpp` | Forward pass shape, ReLU output range, gradient descent convergence |
| `EngineTests/AI/test_qlearning.cpp` | Q-value update correctness, epsilon-greedy exploration ratio, bounds-safety |
