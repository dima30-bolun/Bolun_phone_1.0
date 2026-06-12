# Bolun Phone 1.0 Beta Architecture

Bolun Phone is designed as a classical mobile operating system with a native boot chain, hardware abstraction layer, C/C++ kernel, device-family-aware drivers, user-mode services, graphics shell, application layer, recovery environment, and update OS.

## Non-negotiable language policy

Rust is not the primary OS implementation language. Production kernel, HAL, boot, and driver work is based on C, C++, and Assembly. C# is reserved for higher-level services, shell components, apps, and SDK tooling where managed runtime support is appropriate. Rust may remain in `tools/experimental/rust/` for experiments and development tools only.

## Top-level architecture

| Layer | Responsibility | Primary languages |
| --- | --- | --- |
| Boot | Firmware handoff, image validation, early page tables, boot ABI | Assembly, C |
| HAL | Interrupt controllers, timers, SoC discovery, board quirks, low-level CPU/platform glue | C, Assembly |
| Kernel | Memory manager, scheduler, IPC, object handles, process/thread model, security primitives | C, C++, Assembly |
| Drivers | Display, touch, sensors, storage, network, power, battery, USB | C, C++ |
| MainOS | OS image policy, package composition, service launch ordering | C++, C# |
| Services | Telephony, networking, update orchestration, settings, app model | C++, C# |
| Graphics | Display pipeline, compositor, rendering bridge, shell presentation primitives | C++, C# |
| Shell | Mobile UX, launcher, lock screen, notifications, input UX | C++, C# |
| Applications | Built-in apps and samples | C#, C++ |
| Recovery | Minimal repair/update environment | C, C++ |
| UpdateOS | A/B or staged update verification and installation | C, C++ |
| SDK | Developer APIs, build tools, packaging, emulators/device deployment | C#, C++ |

## Device Compatibility Layer

The HAL owns the Device Compatibility Layer. It is designed from the beginning for real Lumia-class Qualcomm devices, not only QEMU. The initial supported SoC families are:

- Qualcomm Snapdragon S4;
- Qualcomm Snapdragon 200;
- Qualcomm Snapdragon 400;
- Qualcomm Snapdragon 800;
- Qualcomm Snapdragon 808;
- Qualcomm Snapdragon 810.

The DCL records CPU generation, expected core count, and hardware quirks so boot, kernel, and drivers can select correct policies without scattering board checks through the codebase. The Lumia Device Layer adds board profiles and compatibility checks for Lumia 520, 525, 530, 532, 535, 540, 550, 620, 625, 630, 635, 640, 640 XL, 650, 720, 730, 735, 820, 920, 925, 930, 1020, 1320, 1520, 950, and 950 XL.

## Driver Abstraction Layer

The Driver Abstraction Layer gives the kernel and user-mode driver hosts a stable way to match drivers to SoC families and device classes. The first mandatory classes are display, touch, audio, camera, Bluetooth, Wi-Fi, GPS, sensors, NFC, USB, cellular, storage, network, and power because Lumia adaptation depends on display, touch, sensor, radio, and storage bring-up early in the roadmap.

## Development priority

1. Boot loader.
2. HAL.
3. Kernel.
4. Memory management.
5. Scheduler.
6. Display drivers.
7. Sensor drivers.
8. Graphics shell.
9. System services.
10. SDK.

## Boot sequence roadmap

1. Firmware or existing device boot chain loads a Bolun boot image.
2. Boot assembly switches CPU mode, establishes minimal stack/page tables, and calls C boot code.
3. C boot code normalizes firmware/device data into `BolunBootInfo`.
4. HAL validates CPU architecture and SoC family through the Device Compatibility Layer.
5. Kernel validates physical memory, initializes memory management, configures security capabilities, creates handle tables, validates IPC endpoints, and starts the early scheduler.
6. Driver Abstraction Layer brings up display, touch, sensors, storage, power, radios, USB, camera, and audio in dependency order.
7. Graphics initializes the framebuffer, compositor, window manager, tile engine, font engine, and animation engine.
8. MainOS launches package, update, notification, power, storage, and account services.
9. Shell launches start screen, lock screen, notification center, settings, and application launcher.
10. Recovery and UpdateOS remain independently bootable for repair and staged updates.
