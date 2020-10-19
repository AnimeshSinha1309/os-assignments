# Question 3 | Semaphores

* Musicians: Implemented on 2 threads, one of them get's discarded.
* Singers: Implemented on 1 thread, looks in stage file to get a slot.
* Stage: Array of musicians and singers, holds 3 semaphores and several mutexes.
* Coordinators: Just a single semaphore
