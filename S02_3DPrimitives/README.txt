Ian Effendi || IGME 309.03
Prof. A. Bobadilla || A02 - 3D Primitives
+-----------------------------------------------+

I began with the provided material for the 3D Primitives assignment and implemented my own versions of the Cone, Cylinder, Tube, and Sphere. I did not get around to doing the Torus.

+-----------------------------------------------+

Note:
It takes a while for the sphere to compile when subdivided to a level of 5, because the icosahedron method requires a split of the triangles by 4 for each time.
In addition to this, when generating the vertices for the icosahedron, initially, by normalizing all the vertices after being subdivided, they're smoothed out even further, at least, for this approximation. Compared to the UVsphere implementation (in which it's subdivided by longitude and latitude) and compared to the smoothed cube implementation (in which a cube is continually subdivided) this was my favorite to implement just for how well it turned out.

+-----------------------------------------------+

Here's what was implemented:
- Cone primitive.
- Cylinder primitive.
- Tube primitive.
- Sphere primitive, using the icosahedron algorithm approach.

+-----------------------------------------------+

Other work:
- Added a magnitude method and overload method to get the magnitude of a vector3 quickly.
- Added a midpoint index method to grab the index of a midpoint if it already exists, or, adds it to the map<string, int> of midpoint indices.

+-----------------------------------------------+

Extra Credit:
- I did not do the optional part of the assignment.

+-----------------------------------------------+

Missing Work:
- Nothing is missing from this submission.

+-----------------------------------------------+

Table of contents:
Below are line number references for pieces of the algorithm for each of the methods.

GenerateCone():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #282	|| Start of the my code.
Line #321	|| Creating the base of the cone.
Line #350	|| Creating the sides of the cone.

GenerateCylinder():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #405	|| Start of the my code.
Line #439	|| Creating the base of the cylinder.
Line #466	|| Creating the cap of the cylinder.
Line #495	|| Creating the sides of the cylinder.

GenerateTube():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #564	|| Start of the my code.
Line #600	|| Creating the base of the tube.
Line #633	|| Creating the cap of the tube.
Line #668	|| Creating the outside of the tube.
Line #703	|| Creating the inside of the tube.
	
GenerateSphere():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #821	|| Start of the my code.
Line #833	|| Addition of vertex indices, representing the initial 20 faces.
Line #998	|| Creation of the vertices being used by the indices.
Line #1024	|| Subdivision of the vertices on the shape.
Line #1080	|| Normalization of the vectors representing the vertices.
Line #1090	|| Creation of the surface triangles of the sphere.

Magnitude():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #1112	|| Magnitude(float, float, float)
Line #1119	|| Magnitude(vector3)

MidpointIndex():
+-----------------------------------------------+
| Line # 	|| Description: 		|
+-----------------------------------------------+
Line #1127	|| Start of the method.
Line #1133	|| Check if midpoint already exists.
Line #1141	|| Adding or getting midpoint index value if it already exists.

+-----------------------------------------------+


	
		