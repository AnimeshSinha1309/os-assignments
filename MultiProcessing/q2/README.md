# Question 2 | Mutexes

* Zones: Central book-keeping agents, maintains mutexes
* Student: Interacts with zone, busy waits till gets a slot, tests, and polls again
* Company: Only interacts with a zone, busy waits and delivers
