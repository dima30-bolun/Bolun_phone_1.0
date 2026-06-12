//! Capability identifiers used by kernel-mediated services and drivers.

/// Kernel object namespaces. User space never receives raw kernel pointers.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum ObjectKind {
    /// Thread control block.
    Thread,
    /// Address space descriptor.
    AddressSpace,
    /// IPC endpoint.
    Endpoint,
    /// Driver-owned device object.
    Device,
}

/// Compact capability descriptor suitable for IPC messages.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Capability {
    object: u32,
    kind: ObjectKind,
    rights: Rights,
}

impl Capability {
    /// Construct a capability for a kernel object id.
    #[must_use]
    pub const fn new(object: u32, kind: ObjectKind, rights: Rights) -> Self {
        Self {
            object,
            kind,
            rights,
        }
    }

    /// Return the object id.
    #[must_use]
    pub const fn object(self) -> u32 {
        self.object
    }

    /// Return the object namespace.
    #[must_use]
    pub const fn kind(self) -> ObjectKind {
        self.kind
    }

    /// Test whether all requested rights are present.
    #[must_use]
    pub const fn permits(self, requested: Rights) -> bool {
        self.rights.contains(requested)
    }
}

/// Bitset of rights carried by a capability.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Rights(u16);

impl Rights {
    /// No operation is allowed.
    pub const NONE: Self = Self(0);
    /// Read from object.
    pub const READ: Self = Self(1 << 0);
    /// Write to object.
    pub const WRITE: Self = Self(1 << 1);
    /// Transfer this capability over IPC.
    pub const TRANSFER: Self = Self(1 << 2);
    /// Map object into an address space.
    pub const MAP: Self = Self(1 << 3);

    /// Combine two rights sets.
    #[must_use]
    pub const fn union(self, other: Self) -> Self {
        Self(self.0 | other.0)
    }

    /// Whether this set contains all bits of another set.
    #[must_use]
    pub const fn contains(self, other: Self) -> bool {
        (self.0 & other.0) == other.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn capability_checks_requested_rights() {
        let cap = Capability::new(
            7,
            ObjectKind::Endpoint,
            Rights::READ.union(Rights::TRANSFER),
        );

        assert!(cap.permits(Rights::READ));
        assert!(!cap.permits(Rights::WRITE));
    }
}
