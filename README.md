# Bolun Phone 1.0 Beta

Bolun Phone 1.0 Beta is a clean-room mobile operating system project targeting a classical mobile OS architecture rather than a Rust-only hobby-OS layout.

## Primary implementation languages

Rust is **not** the primary operating-system implementation language for Bolun Phone. Rust may be used only for experimental components, prototypes, verification utilities, and development tools unless a subsystem design document explicitly justifies production Rust usage.

The primary language policy is:

| Subsystem | Primary languages |
| --- | --- |
| Boot loader | Assembly, C |
| HAL | C, Assembly |
| Kernel | C, C++, Assembly |
| Drivers | C, C++ |
| Graphics shell | C++, C# |
| System services | C++, C# |
| SDK | C#, C++ |

The previously created Rust kernel-core prototype has been moved to `tools/experimental/rust/` to make its experimental status explicit.

## Repository layout

- `boot/` — boot ABI, boot loader contracts, and future ARM/x86 assembly entry code.
- `hal/` — hardware abstraction layer and Device Compatibility Layer for real device families.
- `kernel/` — C/C++ kernel core, memory management, scheduling, IPC, and architecture ports.
- `drivers/` — Driver Abstraction Layer and concrete display, touch, sensor, storage, network, and power drivers.
- `shell/` — mobile shell and interaction layer.
- `services/` — user-mode system services.
- `graphics/` — compositor, rendering backends, and display pipeline.
- `runtime/` — common runtime status/contracts for system components.
- `sdk/` — C#/C++ developer SDK tooling and APIs.
- `apps/` — built-in applications.
- `mainos/` — main OS launch plan and image policy.
- `recovery/` — recovery environment.
- `updateos/` — update OS and device update pipeline.
- `data/` — device, service, and application compatibility data.
- `tests/` — host contract tests for C and C++ components.
- `tools/experimental/rust/` — experimental Rust prototypes and developer tooling only.
- `build/cmake/` — checked-in build-system support files; generated build outputs are ignored.

## Current status

This repository now contains a corrected `M0` foundation in C with host contract tests:

- boot metadata validation for ARM/x86 and Lumia-class Qualcomm targets;
- Device Compatibility Layer profiles for Snapdragon S4, 200, 400, 800, 808, and 810;
- Driver Abstraction Layer matching rules for device-family-aware drivers;
- early memory helpers and allocation-free scheduler policy in C;
- explicit subsystem directories matching the intended mobile OS architecture;
- Lumia Board Profiles and Lumia Compatibility checks for the required device list;
- boot manager/configuration selection for MainOS, Recovery, UpdateOS, and Diagnostics;
- kernel security capabilities, handle manager, and IPC message validation;
- C++ tile layout engine, shell route model, MainOS launch plan, and service registry.

## Development

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Optional Rust prototype checks remain available from `tools/experimental/rust/`:

```sh
cargo test --manifest-path tools/experimental/rust/Cargo.toml
```

## Design documents

- [Architecture](docs/architecture.md)
- [Boot ABI](docs/boot-abi.md)
