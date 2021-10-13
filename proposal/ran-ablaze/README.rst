===================================================================
Simple Polyhedral Structure and Optimization
===================================================================

The GitHub repository: 

Basic Information
=================

The polyhedral model is a powerful mathematical framework that reasons about loops in programs. Each lattice point inside a polyhedral corresponds with one instance of statement within nested loops.

Problem to Solve
================

The interest of using polyhedral representations is that they can be manipulated or optimized with algorithms whose complexity depends on their structure. Furthermore, generic and compact solutions can be designed that depend on program parameters. 

The Simple Polyhedral Structure tries to use C++ to construct a simple API to express the model and how the model should be optimized.

The structure has a scheduling language featuring novel commands for targeting multiple high performance architectures. It is designed to enable easy integration of code generators for new architectures.

Prospective Users
=================

Almost all loops in a majority of High Performance Computing programs and deep learning.

System Architecture
===================

Simple Polyhedral Structure can use simple instructions to declare loops and generate specified optimized programs.

The `iter` class represents the iterator variables. The range defines the range of the loop around the computation.

The `expr` class represents the expressions.

The `comput` class represents the computations that share schedules up to a level. A computation has an expression and iteration domain defined using an iterator variable. It contains many optimization functions.

Workflow:

1. Declare iterator variables
1. Declare expressions
1. Specify optimizations
1. Generate code

API Description
===============

The `iter` class and `comput` class should be constructed with variable name, range of loop and the statement in the loop.

Function in `comput` class

- `parallelize()`: parallelize the for loop with specified layer.
- `vectorize()`: vectorize the for loop with specified layer and vector size.
- `tiling()`: tiling the loop.
- `codegen()`: generate the loop code.
- more to be continued

Engineering Infrastructure
==========================

I plan to use google test as my testing framework.


Schedule
========

Week 1:
  Implement the iter class.

Week 2:
  Implement the expr class.

Week 3:
  Implement the comput class constructors.

Week 4:
  Implement the codegen() function and corresponding unit tests.

Week 5:
  Implement one optimization finctions in comput class and corresponding unit tests.

Week 6:
  Implement one optimization finctions in comput class and corresponding unit tests.

Week 7:
  Implement one optimization finctions in comput class and corresponding unit tests.

Week 8:
  Overflown work.

References
==========

* Polyhedral compilation community :
  https://polyhedral.info/
