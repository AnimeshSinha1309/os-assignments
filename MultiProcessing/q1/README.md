# Question 1 | Parallel Implementation of MergeSort

The details of the run are given here 

| Size of Input | Time: Normal | Time: Processes | Time: Threads |
| ------------- | ------------ | --------------- | ------------- |
| 10            | 0.000001     | 0.000567        | 0.000349      |
| 100           | 0.000010     | 0.003100        | 0.001894      |
| 1000          | 0.000121     | 0.022446        | 0.020815      |
| 10000         | 0.005124     | 1.060021        | 0.440991      |

The threads and processes do not seem to be helping at all.
The reason seems to be that there is not enough to parallelize, and 
we are taking too much of an overhead making those threads.
