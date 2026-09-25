# Meshmerize Robot Software Architecture

## Purpose

Autonomous line-following maze robot software for the Meshmerize competition.

## Architecture Layers

1. Hardware
2. Hardware Abstraction
3. Perception
4. Control
5. Robot
6. World Model
7. Exploration
8. Planning
9. Execution
10. Application

## Hardware-Independent Core

The following components must remain independent of ESP32 hardware:

- Maze Graph
- Exploration
- Path Planning
- Path Optimization
- Route Execution
- Robot State
- State Machine

## Hardware-Dependent Components

The following components are isolated behind hardware interfaces:

- IR sensor reading
- Motor PWM
- Encoder reading
- Start button
- LED
- Buzzer

## Simulation

The project contains a simulation backend that uses the same core interfaces as the eventual ESP32 implementation.