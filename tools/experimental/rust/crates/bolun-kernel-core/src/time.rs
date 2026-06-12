//! Monotonic kernel time primitives.

/// Monotonic timestamp in nanoseconds since kernel timebase initialization.
#[derive(Clone, Copy, Debug, Default, Eq, Ord, PartialEq, PartialOrd)]
pub struct InstantNanos(u64);

impl InstantNanos {
    /// Create a timestamp from nanoseconds.
    #[must_use]
    pub const fn from_nanos(nanos: u64) -> Self {
        Self(nanos)
    }

    /// Return raw nanoseconds.
    #[must_use]
    pub const fn as_nanos(self) -> u64 {
        self.0
    }

    /// Checked duration between two instants.
    #[must_use]
    pub const fn checked_duration_since(self, earlier: Self) -> Option<u64> {
        self.0.checked_sub(earlier.0)
    }
}
