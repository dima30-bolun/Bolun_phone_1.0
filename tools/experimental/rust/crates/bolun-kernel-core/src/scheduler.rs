//! Deterministic early scheduler policy.
//!
//! The production scheduler will grow CPU-local run queues and real preemption,
//! but this core policy is intentionally small and testable: higher priority
//! runnable threads are selected first, and equal priority is FIFO.

use crate::time::InstantNanos;

/// Maximum number of runnable threads tracked by the early boot scheduler.
pub const EARLY_RUN_QUEUE_CAPACITY: usize = 64;

/// Stable thread identifier allocated by architecture-independent kernel code.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct ThreadId(u32);

impl ThreadId {
    /// Construct a thread id from a non-zero integer.
    #[must_use]
    pub const fn new(raw: u32) -> Option<Self> {
        if raw == 0 { None } else { Some(Self(raw)) }
    }

    /// Return the raw id.
    #[must_use]
    pub const fn raw(self) -> u32 {
        self.0
    }
}

/// Scheduler priority. Higher numeric values run first.
#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct Priority(u8);

impl Priority {
    /// Lowest user priority.
    pub const LOW: Self = Self(32);
    /// Default service priority.
    pub const NORMAL: Self = Self(128);
    /// Kernel worker priority.
    pub const KERNEL: Self = Self(224);

    /// Create a priority from a raw value.
    #[must_use]
    pub const fn new(raw: u8) -> Self {
        Self(raw)
    }

    /// Return the raw priority.
    #[must_use]
    pub const fn raw(self) -> u8 {
        self.0
    }
}

/// Runnable thread metadata stored in a run queue.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct RunnableThread {
    /// Thread id.
    pub id: ThreadId,
    /// Scheduling priority.
    pub priority: Priority,
    /// Time at which the thread became runnable.
    pub enqueued_at: InstantNanos,
}

/// Errors produced by bounded early scheduler operations.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum SchedulerError {
    /// Queue has reached [`EARLY_RUN_QUEUE_CAPACITY`].
    QueueFull,
}

/// Fixed-capacity scheduler used until the dynamic allocator is online.
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct EarlyRunQueue {
    entries: [Option<RunnableThread>; EARLY_RUN_QUEUE_CAPACITY],
    len: usize,
}

impl Default for EarlyRunQueue {
    fn default() -> Self {
        Self::new()
    }
}

impl EarlyRunQueue {
    /// Create an empty queue.
    #[must_use]
    pub const fn new() -> Self {
        Self {
            entries: [None; EARLY_RUN_QUEUE_CAPACITY],
            len: 0,
        }
    }

    /// Number of runnable threads.
    #[must_use]
    pub const fn len(&self) -> usize {
        self.len
    }

    /// Whether the queue is empty.
    #[must_use]
    pub const fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// Enqueue a runnable thread.
    ///
    /// # Errors
    ///
    /// Returns [`SchedulerError::QueueFull`] when the bounded early boot queue has
    /// no free slot.
    pub fn push(&mut self, thread: RunnableThread) -> Result<(), SchedulerError> {
        if self.len == EARLY_RUN_QUEUE_CAPACITY {
            return Err(SchedulerError::QueueFull);
        }

        self.entries[self.len] = Some(thread);
        self.len += 1;
        Ok(())
    }

    /// Select and remove the next runnable thread.
    pub fn pop_next(&mut self) -> Option<RunnableThread> {
        let selected_index = self.best_index()?;
        let selected = self.entries[selected_index].take();

        let mut index = selected_index;
        while index + 1 < self.len {
            self.entries[index] = self.entries[index + 1];
            index += 1;
        }
        self.len -= 1;
        self.entries[self.len] = None;

        selected
    }

    fn best_index(&self) -> Option<usize> {
        let mut best: Option<usize> = None;
        let mut index = 0;

        while index < self.len {
            if let Some(candidate) = self.entries[index] {
                let replace = match best.and_then(|best_index| self.entries[best_index]) {
                    None => true,
                    Some(current) => {
                        candidate.priority > current.priority
                            || (candidate.priority == current.priority
                                && candidate.enqueued_at < current.enqueued_at)
                    }
                };

                if replace {
                    best = Some(index);
                }
            }
            index += 1;
        }

        best
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn thread(raw: u32, priority: Priority, nanos: u64) -> RunnableThread {
        RunnableThread {
            id: ThreadId::new(raw).unwrap(),
            priority,
            enqueued_at: InstantNanos::from_nanos(nanos),
        }
    }

    #[test]
    fn picks_highest_priority_first() {
        let mut queue = EarlyRunQueue::new();
        queue.push(thread(1, Priority::LOW, 10)).unwrap();
        queue.push(thread(2, Priority::KERNEL, 20)).unwrap();
        queue.push(thread(3, Priority::NORMAL, 5)).unwrap();

        assert_eq!(queue.pop_next().unwrap().id.raw(), 2);
        assert_eq!(queue.pop_next().unwrap().id.raw(), 3);
        assert_eq!(queue.pop_next().unwrap().id.raw(), 1);
    }

    #[test]
    fn preserves_fifo_for_equal_priority() {
        let mut queue = EarlyRunQueue::new();
        queue.push(thread(1, Priority::NORMAL, 20)).unwrap();
        queue.push(thread(2, Priority::NORMAL, 10)).unwrap();

        assert_eq!(queue.pop_next().unwrap().id.raw(), 2);
        assert_eq!(queue.pop_next().unwrap().id.raw(), 1);
    }
}
