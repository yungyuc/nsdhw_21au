Job shop scheduling
===================

Basic information (including the GitHub repository)
---------------------------------------------------

Job-shop scheduling or the job-shop problem (JSP) is an optimization
problem in computer science and operations research.

**My repo link**: https://github.com/BoWeii/job-shop-scheduling ##
Problem to solve

In a general job scheduling problem, we are given n jobs **J1, J2, ...,
Jn** of varying processing times,each job consists of a set of
operations **O1, O2, ..., On** which need to be processed in a specific
order (known as precedence constraints). Each operation has a specific
machine that it needs to be processed on and only one operation in a job
can be processed at a given time. We need to schedule the jobs on **m**
machines with varying processing power, while trying to minimize the
makespan. |image0|

Perspective users
-----------------

Some automated processing factories or people with similar needs will
need this algorithm

System architecture
-------------------

**Python**: \* Initialize the informantion and parameters 1. population
size 2. crossover rate v 3. mutation rate v 4. processing table for
Jobs/Operations 5. machine sequence table for Jobs/Operations \* Render
the result as Gantt Chart

**C++**: \* Responsibility for all of Genetic-Algorithm working

**Workflow** : |image1|

API description
---------------

-  Python

   1. **init**\ () : init the params and table data
   2. **Gen\_gantt**\ () : generate the Grantt Chart by plotly

-  C++

   1. **Gen\_init\_group**\ () : random the Initializaion group.
   2. **Evaluate**\ () : evaluation function rating solutions in terms
      of their fitness
   3. **Keep\_best**\ (): all individuals are compared with the best
      one, if an individual which is better than the current best
      individual, the individual is kept as the new best one.
   4. **Crossover**\ () : propose a partial schedule of exchanging
      crossover.
   5. **Mutation**\ () : generate randomly two positions in the list and
      exchange their genes, and if the two genes are same one, retry to
      select new positions.
   6. **Elitist\_select**\ (): select the best pop-size individuals
      conforming their fitness for next generation

Engineering infrastructure
--------------------------

**Build System:** Makefile

**Testing Framework:** Python:pytest C++:GoogleTest

**Version Control** : git ## Schedule \* week1: create the structure and
environment in python and c++ \* week2: implement the function of
genetic algorithm \* week3: implement the function of genetic algorithm
\* week4: implement the function of genetic algorithm \* week5:
implement the function of genetic algorithm \* week6: implement the
function of genetic algorithm \* week7: output the result as
Grantt-Chart \* week8: prepare presentaion

References
----------

https://en.wikipedia.org/wiki/Job-shop\_scheduling
https://ieeexplore.ieee.org/document/400072
https://plotly.com/python/

.. |image0| image:: https://i.imgur.com/XMlXh4Z.png
.. |image1| image:: https://i.imgur.com/CFasA9G.jpg
