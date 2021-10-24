Basic Information
-------------------
Github Repositroy: TBD

Problem to Solve
-----------------
Harmony Search algorithm is a heuristic optimization algorithm.
It is inspired from the method jazz musicians employ when playing
a piece, in order to find the best harmoics. The algorithm stores
a series of 'harmonies', and at each iteration generates a new one.
This new harmony is compared to the worst harmony in memory, and if
it is judged superior, the harmony memory is updated.

The algorithm has seen much use since its initial publication in 2001;
it has also been under harsh criticism. 

In any case, there seem to be few options for those wanting to use the
algorithm in python. 

Prospective Users
------------------
Anyone with an interest in optimizations.

System Architecture
--------------------
Harmony Search begins by initializing a Harmony Memory (HM). HM can be saved
as a matrix of size (N, HMMS), where N is the number of parameters, and HMMS
is the HM size. 

The User provides an objective function, which is stored as an array of the coefficients. 
The HM is initialized and entries are fit to the function. Keeping tabs on the worst and
best fit is important.

On every iteration of the algorithm, a new harmony is created with a probability of 1-HMCR 
(Harmony Memory Considering Rate). If an existing entry is chosen instead (p=HCMR), then it
is altered according to a Pitch Rate.

The new harmony is then fit against the worst fitting entry in HM. IF the new harmony is a better
fit, then HM is overwritten; otherwise, the new harmony is simply discarded.

Harmony Search can be written as a class containing it required elements HM, HMCR, PR, and the methods
required to produce and fit the harmonies to the objective function.
The class should also store the best and worst fits in HM.

API Description
----------------
The harmony search algorithm class will be written in C++.
This will be wrapped in python for python use.

Engineering Infrastructure
---------------------------
Testing will be done with python unittest.

The Harmony Search algorithm can be tested against other ER optimization algorithms (since they
have been compared before as well).

Schedule
--------

* Week 1: Research Harmony Search Algorithm
* Week 2: Write Harmony Search Algorithm Class
* Week 3: Write Objective Function Class
* Week 4: Integrate both and catch up on any leftover work
* Week 5: Start testing Harmony Search Algorithm
* Week 6: Consider Harmony Search Algorithm Variations 
* Week 7: Further tests and leftover work