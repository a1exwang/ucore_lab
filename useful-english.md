- cprintf("phi_test_condvar: state_condvar[%d] will eating\n",i);

- the sem semaphore  is used to down the waiting proc, and the signaling proc should up the waiting proc

- // the next semaphore is used to down the signaling proc itself, and the other OR wakeuped waiting proc should wake up the sleeped signaling proc

- I ate over

