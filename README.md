# MapGenerator
This is my attempt of a C++ and OpenGL implementation of Amit Patel's Map Gen 4. I Intend to try to use the gain in speed from creating a standalone application to pursue more features and possible additional generation attributes.

MapGen4 : https://www.redblobgames.com/maps/mapgen4/

Here is an explanation for the starting point of the map generation / the basic data structure responsible for the polygonal regions:
https://mapbox.github.io/delaunator/




![mmexport1650679257413](https://user-images.githubusercontent.com/6415123/168144723-3bc0466c-2f56-4e9e-82c6-276859f281de.jpg)


If possible I am going to try to to use a method described in this article www.geometry.caltech.edu/pubs/MMdGD11.pdf, specifically Hodge-Optimized Triangulations to create nicer dual meshes for working with.

Progress Updates:

I am testing my understanding of every step as while the basic concepts are very manageable, implementing them efficiently and with OpenGl is a new challenge I want to understand fully,

Generating Poisson Points and Rendering:

![unknown-11-1](https://user-images.githubusercontent.com/6415123/168144964-81afd968-6a37-4ec6-9bc4-6bae63076d66.png)

Creating and rendering triangles from those points:

![unknown-25](https://user-images.githubusercontent.com/6415123/168145136-24fa533e-5746-479c-9b83-186d192a940b.png)

Implementing a custome shader to distinguish the triangle better in non-wireframe mode:

![-5121115581670599172_121](https://user-images.githubusercontent.com/6415123/168145300-4a447e6b-6a89-4b4e-b180-10a580872da7.jpg)

Solving the issue of the triangles only generating in a circle:

![unknown-23](https://user-images.githubusercontent.com/6415123/168145405-09c57bad-f148-4ad6-a7c4-92c3963d1d35.png)

Testing at scale :

![unknown-17](https://user-images.githubusercontent.com/6415123/168145476-9c0891cc-b605-4bf8-b449-bf960864bcb8.png)

Checking the efficiency of my code thus far:

![image](https://user-images.githubusercontent.com/6415123/168146632-eba7a0d3-5fe9-4913-b7e0-7e3d7cbaf3ea.png)
![image](https://user-images.githubusercontent.com/6415123/168146672-09975dde-1a82-4c01-8232-8f9b179b449b.png)

This seems to be an acceptable speed for the initial generation and the majority of the bottleneck is from libraries that are already as fast as possible, thankfully it seems like I havent made any major errors yet.
