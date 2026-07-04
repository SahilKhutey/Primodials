# 05_Emotion_System

This document specifies the emotion system — how variables like Fear, Rage, and Trust fluctuate and influence behavior.

---

## 1. Emotional State Variables
Intelligent entities track four core emotional values inside [MindComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/MindComponent.hpp):
* **Fear ($F$):** Stimulated by local threat influence emitters or receiving collision damage.
* **Rage ($R$):** Stimulated by attacks from hostiles or competition over depleted resources.
* **Trust ($T$):** Built by proximity to friendly shapes, sharing resources, or Sentinel broadcasts.
* **Wonder ($W$):** Stimulated by entering newly discovered spatial coordinates.

---

## 2. Emotional Decay Invariant
Emotions are transient responses. When stimulations cease, variables decay back to baseline levels ($0.0$):

$$\text{Emotion}_{t+1} = \max(0.0, \text{Emotion}_t - C_{\text{decay}} \cdot \Delta t)$$

* **Fear Decay Rate ($C_{\text{fear\_decay}}$):** 10.0 units/sec.
* **Rage Decay Rate ($C_{\text{rage\_decay}}$):** 5.0 units/sec.

---

## 3. Cognitive Override Values
High emotional values override base genetic utility weights:
* **Fear > 80.0:** Forces decision utility score for Escape to jump to maximum ($U_{\text{Escape}} = F \times 1.2$), overriding hunger and exploration.
* **Rage > 75.0:** Boosts combat action priorities while suppressing self-preservation steering checks.
