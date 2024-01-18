# Contribute

## Issues

Bug reports and other questions are always welcome at the issue tracker of this
repository: https://github.com/vtraag/libleidenalg/issues. Please make sure you
provide a [minimal reproducible
example](https://stackoverflow.com/help/minimal-reproducible-example) to help
solve the issue.

## Pull Requests

If you want to fix a bug, Pull Requests are also always welcome. New features
are of course also welcome as Pull Request. Before embarking, please first open
an issue with the proposed feature. If you want to implement a new community
detection quality function, while using the Leiden algorithm to optimise it,
please read the next section carefully.

## Implement new community detection methods

All methods derive from `MutableVertexPartition`, which implements almost all
necessary details, such as moving actual nodes while maintaining the internal
administration. Similarly, it provides all the necessary functionality for
initialising a partition. Additionally, there are two abstract classes that
derive from this base `ResolutionParameterVertexPartition` and
`LinearResolutionParameterVertexPartition` (which in turn derives from the
former class). If you want a method with a resolution parameter, you should
derive from one of these two classes, otherwise, simply from the base class
`MutableVertexPartition`.

There are two functions that you need to implement yourself: `diff_move` and
`quality`. Note that they should always be consistent, so that we can double
check the internal consistency. You should also ensure that the `diff_move`
function can be correctly used on any aggregate graph (i.e. moving a node in the
aggregate graph indeed corresponds to moving a set of nodes in the individual
graph).