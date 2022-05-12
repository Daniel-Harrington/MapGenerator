# MapGenerator
This is my attempt of a C++ and OpenGL implementation of Amit Patel's Map Gen 4. I Intend to try to use the gain in speed from creating a standalone application to pursue more features and possible additional generation attributes.

https://www.redblobgames.com/maps/mapgen4/

Here is an explanation for the starting point of the map generation / the basic data structure responsible for the polygonal regions:
https://mapbox.github.io/delaunator/

If possible I am going to try to to use a method described in this article www.geometry.caltech.edu/pubs/MMdGD11.pdf, specifically Hodge-Optimized Triangulations to create nicer dual meshes for working with.
