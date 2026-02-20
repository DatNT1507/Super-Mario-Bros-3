https://github.com/user-attachments/assets/02a12c74-3be4-4e8c-b1b0-c706a9d3ec89

# Super Mario Bros 3 - Reimagined

A technical recreation of the classic NES platformer, *Super Mario Bros 3*, focusing on precise physics, state-based character logic, and modular enemy AI.



## 🚀 Overview
This project aims to replicate the "feel" of the original 1988 masterpiece while implementing modern coding standards. It features a custom kinematic character controller, a robust power-up system, and a grid-based interactive environment.

## 🛠️ Technical Features

### 1. Kinematic Character Controller
Unlike standard physics engines, this project uses a custom movement handler to achieve the iconic "slippery" Mario movement.
* **Variable Jump Height:** Jump velocity is scaled based on input duration.
* **P-Meter Logic:** Sprinting builds momentum, eventually unlocking flight/high-jump states.
* **Coyote Time & Input Buffering:** Forgiving mechanics to improve player feel.

### 2. State Machine Architecture
Mario’s forms (Small, Big, Raccoon, Fire) are managed via a hierarchical state machine:
* **Power-up Transitions:** Smoothly handles animation and hitbox changes.
* **Damage System:** Automatically downgrades states or triggers the "Die" sequence.

### 3. Entity & AI System
* **Stomp Logic:** Multi-layered collision detection to distinguish between "taking damage" and "defeating an enemy."
* **Shell Physics:** Koopa shells act as both enemies and projectiles with bouncing logic.
* **Object Pooling:** (Optional) Efficiently manages coins and particle effects to maintain high performance.
