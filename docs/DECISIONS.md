# Architecture Decisions

## Decision 001 — Language

Date: 2026-09-25

Decision:
Use C++ for the robot software.

Reason:
The final target is an ESP32-based autonomous robot requiring real-time sensor processing, motor control, encoder feedback, and graph/path algorithms.

Status:
ACCEPTED


## Decision 002 — Hardware Independence

Date: 2026-09-25

Decision:
Keep the maze, exploration, planning, path optimization, and core robot logic independent of ESP32 hardware.

Reason:
The software will first be developed and tested using a simulator and later connected to the physical ESP32.

Status:
ACCEPTED


## Decision 003 — Simulation First

Date: 2026-09-25

Decision:
Develop and test the software using a simulation backend before integrating hardware.

Reason:
This allows the algorithms to be developed independently of physical hardware.

Status:
ACCEPTED


## Decision 004 — Build System

Date: 2026-09-25

Decision:
Use CMake for the desktop development environment.

Reason:
The project will contain multiple modules and tests.

Status:
ACCEPTED


## Decision 005 — C++ Standard

Date: 2026-09-25

Decision:
Use C++17.

Reason:
C++17 provides the language features required by the project while maintaining a conservative compatibility target.

Status:
ACCEPTED