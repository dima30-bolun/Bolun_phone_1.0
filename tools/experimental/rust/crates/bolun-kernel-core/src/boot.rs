//! Boot-time ABI shared by firmware loaders and the Bolun kernel.

use core::fmt;

use crate::memory::{MemoryMap, MemoryRegion};

/// Magic value placed in [`BootInfo::magic`] by trusted Bolun boot loaders.
pub const BOOT_MAGIC: u64 = 0x424f_4c55_4e31_3042;

/// Maximum number of memory regions accepted from a first-stage boot loader.
pub const MAX_BOOT_MEMORY_REGIONS: usize = 64;

/// CPU architecture reported by the boot loader.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum BootArchitecture {
    /// 64-bit Arm profile, expected first for phones and Arm emulators.
    AArch64,
    /// 64-bit x86 profile, useful for desktop emulation and CI.
    X86_64,
}

/// Mandatory boot metadata consumed by the architecture-specific kernel entry.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct BootInfo<'a> {
    /// Must equal [`BOOT_MAGIC`].
    pub magic: u64,
    /// Architecture of the CPU that entered the kernel.
    pub architecture: BootArchitecture,
    /// Physical address at which the kernel image starts.
    pub kernel_phys_start: u64,
    /// Exclusive physical end address of the kernel image.
    pub kernel_phys_end: u64,
    /// Firmware-provided memory map.
    pub memory_map: &'a [MemoryRegion],
    /// Optional flattened device tree physical address for Arm platforms.
    pub device_tree_phys: Option<u64>,
}

/// Boot validation error.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum BootError {
    /// Boot loader did not provide the Bolun magic value.
    BadMagic {
        /// Value observed in the boot metadata.
        actual: u64,
    },
    /// Kernel image range is empty or reversed.
    InvalidKernelRange {
        /// Inclusive physical start address reported for the kernel image.
        start: u64,
        /// Exclusive physical end address reported for the kernel image.
        end: u64,
    },
    /// Boot loader supplied no usable memory information.
    EmptyMemoryMap,
    /// Boot loader supplied more regions than the bounded early kernel path accepts.
    TooManyMemoryRegions {
        /// Number of regions supplied by the boot loader.
        actual: usize,
        /// Maximum accepted region count.
        max: usize,
    },
    /// Memory map itself is malformed.
    InvalidMemoryMap,
}

impl fmt::Display for BootError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::BadMagic { actual } => write!(f, "bad boot magic: {actual:#x}"),
            Self::InvalidKernelRange { start, end } => {
                write!(f, "invalid kernel range: {start:#x}..{end:#x}")
            }
            Self::EmptyMemoryMap => f.write_str("empty boot memory map"),
            Self::TooManyMemoryRegions { actual, max } => {
                write!(f, "too many memory regions: {actual} > {max}")
            }
            Self::InvalidMemoryMap => f.write_str("invalid boot memory map"),
        }
    }
}

impl core::error::Error for BootError {}

impl BootInfo<'_> {
    /// Validate the boot contract before architecture code enables allocators,
    /// scheduler state, or device discovery.
    ///
    /// # Errors
    ///
    /// Returns [`BootError`] when the boot metadata is unauthenticated, malformed,
    /// unsorted, overlapping, empty, or too large for allocation-free early boot.
    pub fn validate(&self) -> Result<(), BootError> {
        if self.magic != BOOT_MAGIC {
            return Err(BootError::BadMagic { actual: self.magic });
        }

        if self.kernel_phys_start >= self.kernel_phys_end {
            return Err(BootError::InvalidKernelRange {
                start: self.kernel_phys_start,
                end: self.kernel_phys_end,
            });
        }

        if self.memory_map.is_empty() {
            return Err(BootError::EmptyMemoryMap);
        }

        if self.memory_map.len() > MAX_BOOT_MEMORY_REGIONS {
            return Err(BootError::TooManyMemoryRegions {
                actual: self.memory_map.len(),
                max: MAX_BOOT_MEMORY_REGIONS,
            });
        }

        MemoryMap::new(self.memory_map)
            .is_valid()
            .then_some(())
            .ok_or(BootError::InvalidMemoryMap)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::memory::{MemoryKind, MemoryRegion};

    const REGION: MemoryRegion = MemoryRegion::new(0x1000, 0x9000, MemoryKind::Usable);

    #[test]
    fn validates_minimal_boot_info() {
        let info = BootInfo {
            magic: BOOT_MAGIC,
            architecture: BootArchitecture::AArch64,
            kernel_phys_start: 0x2000,
            kernel_phys_end: 0x4000,
            memory_map: &[REGION],
            device_tree_phys: Some(0x8000),
        };

        assert_eq!(info.validate(), Ok(()));
    }

    #[test]
    fn rejects_bad_magic() {
        let info = BootInfo {
            magic: 0,
            architecture: BootArchitecture::X86_64,
            kernel_phys_start: 0x2000,
            kernel_phys_end: 0x4000,
            memory_map: &[REGION],
            device_tree_phys: None,
        };

        assert_eq!(info.validate(), Err(BootError::BadMagic { actual: 0 }));
    }
}
