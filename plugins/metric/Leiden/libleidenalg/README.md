# Introduction

This package implements the Leiden algorithm in `C++`.  It relies on
`igraph` for it to function. Besides the relative flexibility of the
implementation, it also scales well, and can be run on graphs of millions of
nodes (as long as they can fit in memory). The core class is
`Optimiser` which finds the optimal partition using the Leiden algorithm[^1], which is an extension of the Louvain algorithm[^2] for a number of
different methods. The methods currently implemented are (1) modularity[^3],
(2) Reichardt and Bornholdt's model using the configuration null model and the
Erdös-Rényi null model[^4], (3) the Constant Potts model (CPM) [^5], (4)
Significance [^6], and finally (5) Surprise [^7]. In addition, it supports
multiplex partition optimisation allowing community detection on for example
negative links [^8] or multiple time slices [^9]. There is the possibility of
only partially optimising a partition, so that some community assignments remain
fixed [^10]. It also provides some support for community detection on bipartite
graphs.

This package contains the `C++` code only. Most people will find it easier to work with the Python interface at https://github.com/vtraag/leidenalg.

# Installation

The build system uses CMake and follows the prototypical CMake build steps:

1. Get the source code.

   You can download the source code from the latest release at https://github.com/vtraag/libleidenalg/releases. Alternatively, you can clone the repository using `git`.

2. Create a build directory

   You can create a build directory anywhere. A common location is to create a subdirectory in the source code as

   ```
   mkdir build
   ```

3. Configure the build system

   Assuming you created the build directory as a subdirectory, you can run the following

   ```
   cmake ..
   ```

   Note that the build directory should be your current working directory.

4. Build the library

   ```
   cmake --build .
   ```

5. Install the library

   ```
   cmake --build . --target install
   ```

You can change the installation location of `libleidenalg` using [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html) as usual.

You can change whether a static or dynamic library should be built using [`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html) as usual.

This library depends on `igraph`, which you should install before. See https://igraph.org/c/doc/igraph-Installation.html for more details.

If you have installed `igraph` in a non-standard location, CMake might not be able to find it automatically. If you use `CMAKE_INSTALL_PATH=<dir>` to install `igraph`, you can specify the []`CMAKE_PREFIX_PATH=<dir>`](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) when configuring `libleidenalg` to find `igraph`.

# Usage

The `Optimiser` class is responsible for optimising a `MutableVertexPartition` (possibly multiple in the case of a multiplex approach). The `MutableVertexPartition` is just a base class, and should be implemented to provide explicit quality function:
- `CPMVertexPartition`
- `ModularityVertexPartition`
- `RBConfigurationVertexPartition`
- `RBERVertexPartition`
- `SignificanceVertexPartition`
- `SurpriseVertexPartition`

Some of these classes depend on intermediate derived classes. The implementation of a quality function in a derived class, essentially comes to down implementing the `diff_move` and `quality`, see also [`CONTRIBUTING.md`](CONTRIBUTING.md) in this repository.

An `igraph_t` object from `igraph` is used to construct a separate `Graph` object, which can be used to construct a `MutableVertexPartition`. For example, to find a partition using CPM, you could do the following

```C
    igraph_t g;
    igraph_famous(&g, "Zachary");

    Graph graph(&g);

    CPMVertexPartition part(&graph,
                            0.05 /* resolution */ );

    Optimiser o;

    o.optimise_partition(&part);
```

In the `example` directory, we added a complete example, including the CMake build files (please note that this directory is not included in the release source package, and only available on the GitHub repository). In order to compile it, first make sure you have properly installed `igraph` and `libleidenalg`. In the `example` directory, you can then follow the standard CMake routine to build it
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
and run `./example`.

Licence
-------

Copyright (C) 2020 V.A. Traag, (C) 2022 Andrew Robbins

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.

# References

Please cite the references appropriately in case they are used.

[^1]:  Traag, V.A., Waltman. L., Van Eck, N.-J. (2018). From Louvain to
       Leiden: guaranteeing well-connected communities. Scientific reports, 9(1), 5233.
       [10.1038/s41598-019-41695-z](http://dx.doi.org/10.1038/s41598-019-41695-z)

[^2]:  Blondel, V. D., Guillaume, J.-L., Lambiotte, R., & Lefebvre, E. (2008).
       Fast unfolding of communities in large networks. Journal of Statistical
       Mechanics: Theory and Experiment, 10008(10), 6.
       [10.1088/1742-5468/2008/10/P10008](http://doi.org/10.1088/1742-5468/2008/10/P10008)

[^3]:  Newman, M. E. J., & Girvan, M. (2004). Finding and evaluating community
       structure in networks. Physical Review E, 69(2), 026113.
       [10.1103/PhysRevE.69.026113](http://doi.org/10.1103/PhysRevE.69.026113)

[^4]:  Reichardt, J., & Bornholdt, S. (2006). Statistical mechanics of
       community detection. Physical Review E, 74(1), 016110.
       [10.1103/PhysRevE.74.016110](http://doi.org/10.1103/PhysRevE.74.016110)

[^5]:  Traag, V. A., Van Dooren, P., & Nesterov, Y. (2011). Narrow scope for
       resolution-limit-free community detection. Physical Review E, 84(1),
       1. [10.1103/PhysRevE.84.016114](http://doi.org/10.1103/PhysRevE.84.16114)

[^6]:  Traag, V. A., Krings, G., & Van Dooren, P. (2013). Significant scales in
       community structure. Scientific Reports, 3, 2930. [10.1038/srep02930](http://doi.org/10.1038/srep02930)

[^7]:  Traag, V. A., Aldecoa, R., & Delvenne, J.-C. (2015). Detecting
       communities using asymptotical surprise. Physical Review E, 92(2),
       1. [10.1103/PhysRevE.92.022816](http://doi.org/10.1103/PhysRevE.92.022816)

[^8]:  Traag, V. A., & Bruggeman, J. (2009). Community detection in networks
       with positive and negative links. Physical Review E, 80(3), 036115.
       [10.1103/PhysRevE.80.036115](http://doi.org/10.1103/PhysRevE.80.036115)

[^9]:  Mucha, P. J., Richardson, T., Macon, K., Porter, M. A., & Onnela, J.-P.
       (2010). Community structure in time-dependent, multiscale, and multiplex
       networks. Science, 328(5980), 876–8. [10.1126/science.1184819](http://doi.org/10.1126/science.1184819)

[^10]:  Zanini, F., Berghuis, B. A., Jones, R. C., Robilant, B. N. di,
        Nong, R. Y., Norton, J., Clarke, Michael F., Quake, S. R. (2019).
        northstar: leveraging cell atlases to identify healthy and neoplastic
        cells in transcriptomes from human tumors. BioRxiv, 820928.
        [10.1101/820928](https://doi.org/10.1101/820928)
