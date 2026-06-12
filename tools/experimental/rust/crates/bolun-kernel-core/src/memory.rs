//! Physical memory map policy for early kernel initialization.

/// Granularity used by the first production targets.
pub const PAGE_SIZE: u64 = 4096;

/// Firmware memory classification normalized by the boot loader.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum MemoryKind {
    /// RAM that can be used by the physical page allocator after reservations.
    Usable,
    /// Kernel image, boot services, framebuffers, device trees, or other reservations.
    Reserved,
    /// MMIO window that must never be handed to the RAM allocator.
    Device,
    /// Persistent crash log or update metadata region.
    Persistent,
}

/// Half-open physical memory interval `[start, end)`.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct MemoryRegion {
    /// Inclusive physical start address.
    pub start: u64,
    /// Exclusive physical end address.
    pub end: u64,
    /// Semantic region type.
    pub kind: MemoryKind,
}

impl MemoryRegion {
    /// Construct a physical memory region.
    #[must_use]
    pub const fn new(start: u64, end: u64, kind: MemoryKind) -> Self {
        Self { start, end, kind }
    }

    /// Region length in bytes.
    #[must_use]
    pub const fn len(self) -> u64 {
        self.end.saturating_sub(self.start)
    }

    /// Whether the region has no addressable bytes.
    #[must_use]
    pub const fn is_empty(self) -> bool {
        self.start >= self.end
    }

    /// Whether both ends are aligned to [`PAGE_SIZE`].
    #[must_use]
    pub const fn is_page_aligned(self) -> bool {
        self.start.is_multiple_of(PAGE_SIZE) && self.end.is_multiple_of(PAGE_SIZE)
    }
}

/// Borrowed memory map with validation helpers.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct MemoryMap<'a> {
    regions: &'a [MemoryRegion],
}

impl<'a> MemoryMap<'a> {
    /// Wrap a boot-provided region slice.
    #[must_use]
    pub const fn new(regions: &'a [MemoryRegion]) -> Self {
        Self { regions }
    }

    /// Return the underlying ordered region list.
    #[must_use]
    pub const fn regions(self) -> &'a [MemoryRegion] {
        self.regions
    }

    /// Validate monotonic ordering, non-overlap, non-empty regions, and page alignment.
    #[must_use]
    pub fn is_valid(self) -> bool {
        let mut previous_end = 0;
        let mut index = 0;

        while index < self.regions.len() {
            let region = self.regions[index];
            if region.is_empty() || !region.is_page_aligned() || region.start < previous_end {
                return false;
            }
            previous_end = region.end;
            index += 1;
        }

        true
    }

    /// Count usable pages available before later kernel reservations are applied.
    #[must_use]
    pub fn usable_pages(self) -> u64 {
        self.regions
            .iter()
            .filter(|region| region.kind == MemoryKind::Usable)
            .map(|region| region.len() / PAGE_SIZE)
            .sum()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn counts_only_usable_pages() {
        let regions = [
            MemoryRegion::new(0x0000, 0x1000, MemoryKind::Reserved),
            MemoryRegion::new(0x1000, 0x9000, MemoryKind::Usable),
            MemoryRegion::new(0x9000, 0xa000, MemoryKind::Device),
        ];

        assert!(MemoryMap::new(&regions).is_valid());
        assert_eq!(MemoryMap::new(&regions).usable_pages(), 8);
    }

    #[test]
    fn rejects_overlapping_regions() {
        let regions = [
            MemoryRegion::new(0x1000, 0x4000, MemoryKind::Usable),
            MemoryRegion::new(0x3000, 0x8000, MemoryKind::Usable),
        ];

        assert!(!MemoryMap::new(&regions).is_valid());
    }
}
