===================
Graph Visualization
===================


Basic Information
=================

- GitHub repository: https://github.com/Luke2336/graph-visualization

Problem to Solve
================

Visualizing a graph can help people study graph theory. In this project, I would like to use force-directed method (Fruchterman-Reingold Algorithm) to layout a simple undirected graph to let the number of edge intersections as small as possible.


Prospective Users
=================

The prospective users are poeple who want to visualize a graph.

System Architecture
===================

- Input format
	The first line contains two non-negative integers n and m.
	For 2 ~ m + 1 lines, there are two integer a and b, which means there is an edge between vertex a and b.
- Flow chart
.. image:: Flow1.png
   :height: 50
   :width: 100
   :scale: 25
   :alt: Flow Chart

API Description
===============

``drawGraph('input.txt', 'output.png')``

Engineering Infrastructure
==========================

- Build System: Make
- Version Control: Git

Schedule
========

- Week 1: [Python] Transfer the coordinate into PNG
- Week 2: [C++] Implement forced-directed method
- Week 3: [C++] Implement forced-directed method & [Makefile]
- Week 4: [C++] Implement forced-directed method
- Week 5: Integrate the c++ and python code
- Week 6: [Python] Test the result of algorithm
- Week 7: Prepare presentation
- Week 8: Prepare presentation

References
==========