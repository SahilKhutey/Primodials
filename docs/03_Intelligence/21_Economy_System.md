# 21_Economy_System

This document specifies the market economics system — how supply-demand ratios define resource prices and guide trade exchanges.

---

## 1. Supply-Demand Pricing Curves
Every storage node maintains a localized **Resource Price Table** calculated from current stock levels relative to capacity:

$$P_{\text{resource}} = P_{\text{base}} \times \left( \frac{\text{Capacity}}{\text{Stock} + 1.0} \right)^k$$

* **High Scarcity (Stock → 0):** Price spikes exponentially, increasing the economy interest for carrier shapes to import that resource from neighboring depots.
* **Over-supply (Stock → Capacity):** Price drops to near zero, prompting shapes to export the resource to deficit zones.

---

## 2. Agent-to-Depot Bartering
Economy shapes (Pentagons) act as merchants:
1. They purchase low-price resources from surplus depots.
2. They transport cargo along the logistics trade highway (see [10_Logistics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/10_Logistics_System.md)).
3. They sell cargo to high-price deficit depots.

---

## 3. Profit Maximization Drive
Merchants calculate profit margins before dispatching trade paths:

$$\text{Margin} = (P_{\text{sell}} - P_{\text{buy}}) \times \text{CargoCapacity} - \text{TransportEnergyCost}$$

If Margin is negative, the merchant stays idle or switches tasks, preventing wasteful energy-cost trades.
