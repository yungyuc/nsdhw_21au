Basic Information
-------------------
Github Repositroy: TBD

Problem to Solve
-----------------
R-Trees are data structures that help index and access
spatial data. If we have multi-dimensional data we want to
index and relate to other spatial data, an R-Tree can be very
useful. R-Trees are flexible and have found usage in all kinds
of applicaions and disciplines.

Prospective Users
------------------
Wide user-base. Anyone who is interested in indexing and
manipulating spatial data.

System Architecture
--------------------
The R-Tree is a structure that stores and indexes data.

Inputs for storage consist of polygons (in the form of lists of
points). These polygons are fed to the R-Tree, with a Minimum 
Bounding Box (the smallest box that encloses this polygon).
The boxes are indexed and stored in the structure.

R-Tree leaves are all level. In an ordered (m, M) tree, the nodes
have at most M entries, and at least m <= M/2 entries. There is a 
tradeoff between time and efficient storage of the data.

Queries accept a range (a box). All items that intersect
this box are returned to the user.

There are different types of queries with increasing complexity. As a 
start, the most basic range queries will be implemented.

API Description
----------------
The structures and access functions will be implemented in c++.
This will be wrapped in python for python use.

Engineering Infrastructure
---------------------------
Testing will be done with python unittest.

The R-Tree should be implemented in a simple way to use.
A documentation page will provide some examples on usage.

Schedule
--------

* Week 1: Read on Implementations of R-Tree (R+, R*)
* Week 2: Program Structure of R-Tree
* Week 3: Program access/delete/... functions
* Week 4: Continue with Work from Previous Week
* Week 5: Start binding to Python and begin tests
* Week 6: Continue tests and implement other access/storing algorithms
* Week 7: Further tests and fixing algorithms
