# Boot ABI

The Bolun boot ABI is a C ABI shared by assembly entry code, C boot code, the HAL, and the kernel. It is intentionally independent of Rust and is suitable for ARM and x86 bring-up.

## Required invariants

- `magic` equals `BOLUN_BOOT_MAGIC`.
- CPU architecture is one of AArch32, AArch64, x86, or x86_64.
- SoC family is one of the supported Qualcomm Lumia-class families: Snapdragon S4, 200, 400, 800, 808, or 810.
- Kernel physical range is half-open and non-empty.
- Memory regions are sorted by physical address.
- Memory regions do not overlap.
- Memory region starts and ends are page-aligned.
- The early kernel accepts at most `BOLUN_MAX_BOOT_MEMORY_REGIONS` regions to keep validation allocation-free.

## Device metadata

`BolunBootInfo` carries optional device tree, ACPI, and framebuffer physical addresses. ARM Lumia-class ports are expected to rely primarily on device tree or board profiles, while x86/x64 ports may use ACPI during emulator and desktop-hosted testing.

## Responsibility split

- Assembly performs only the minimum CPU-mode and calling-convention setup.
- Boot C code builds `BolunBootInfo`.
- HAL validates CPU/SoC compatibility and exposes quirks.
- Kernel consumes validated memory, framebuffer, and platform metadata.
