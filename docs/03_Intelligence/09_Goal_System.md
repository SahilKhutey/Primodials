# 09_Goal_System

This document specifies the goal system — how entities stack long-term intentions and handle short-term behavioral preemptions.

---

## 1. Intention Stack Architecture
Entities maintain a goal stack inside [AIComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/AIComponent.hpp) to preserve behavioral continuity:

```
Goal Stack
 ┌───────────────────────┐
 │ Sub-Task: Path to (X) │  ← Active (Short-term)
 ├───────────────────────┤
 │ Task: Gather Resource │  ← Suspended
 ├───────────────────────┤
 │ Goal: Survive Winter  │  ← Base (Long-term)
 └───────────────────────┘
```

---

## 2. Goal Preemption (Interrupts)
High-priority events (e.g. threat detection or critical starvation) push override tasks onto the top of the stack:
* **Interrupt Check:** If local `fear` exceeds `courage` + $50.0$, the current sub-task is suspended and `Escape` is pushed to the top of the stack.
* **Resumption:** When the preemption criteria are cleared, the override task is popped, and the entity resumes its suspended gather/craft mission.

---

## 3. Persistent Goal Fields
Long-term goals are modulated by Sentinel policies:
* **Harvest Policy:** Increases the base priority of resource gathering for all colony shapes.
* **Defense Policy:** Directs shapes to return to garrison coordinates when threat fields rise.
* **Research Policy:** Shifts career interests toward knowledge gathering.
