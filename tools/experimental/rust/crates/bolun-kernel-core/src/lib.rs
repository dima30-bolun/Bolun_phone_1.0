//! Architecture-neutral kernel core for Bolun Phone 1.0 Beta.
//!
//! This crate intentionally contains no MMIO, assembly, allocator, or platform
//! assumptions. It defines the contracts that boot code, CPU ports, drivers,
//! and system services must share before architecture-specific crates are added.

#![cfg_attr(not(test), no_std)]

pub mod boot;
pub mod capability;
pub mod memory;
pub mod scheduler;
pub mod time;
