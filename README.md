Graphs
======
 
Code and files for graph generation. Developed by [Katie Hyatt](http://github.com/kshyatt) and [Roger Melko](http://github.com/rgmelko). 

This code aims to generate all isomorphically and/or topologically distinct graphs on a variety of 2D lattices used in condensed matter simulations. We define two graphs as isomorphically distinct iff there is no transformation in the lattice's [dihedral group](http://en.wikipedia.org/wiki/Dihedral_group) that can take one graph to the other. We say they are topologically distinct if the Hamiltonians for the two graphs will be different.

The main documentation for the Graphs repo can be found [here](http://kshyatt.github.com/Graphs/)

Installation
============

Since graphs is ISO C++, it should work on any machine which has a C++ compiler installed. You can use the included makefile to compile graphs.cpp. g++ and icpc have been tested. 

Running graphs
==============

Graphs is intended to generate a variety of types of graphs needed for numerical condensed matter simulations. You can create site- and bond-based graphs. You can find detailed descriptions of these on the Graphs [wiki](http://github.com/rgmelko/Graphs/wiki). Since NLC is run on sets of self-consistent graphs, you can generate all site- or bond-based graphs, or only sets like "all rectangles". You will need to create a vector to store the graphs, and then call the appropriate function. graphs also contains utility functions to read/write the graphs to/from file.

TODO
====

- Add lattices besides square and their dihedral groups
- Improve performance of graph generation
- Add MPI functionality to speed up tasks such as subgraph finding 
