// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};
extern void initlock(struct spinlock *lk,char*name);
extern void acquire(struct spinlock *lk);
extern void release(struct spinlock *lk);
