# A03 - LERP: Documentation #
***
This is the documentation file accompanying A03 - LERP assignment.
I've edited the AppClass.h and AppClass.cpp files.

Rubric:
- Can dynamically initialize orbits from *.ini file.
- Routes are generated on-the-fly by formula, at runtime.
- Comments are generous.

Note:
- The spheres aren't ever static, but, when the lerping mechanism switches the current and target positions (edge-connection maps hello!) there is a brief frame in which it lags behind because of where I placed the timer.

Extra:
- It might not be enough to count for extra credit I guess, but, I gave each orbit row a different depth value, using a glm::translate on the z-axis.
- It gives it a nice effect. (Be sure to rotate the camera around the space to check it out?)
- The points were generated with this effect explicitly in mind!
- It looks really nice from the orthographic views - try it out!

***
Changelist:
+------------------+--------------------------------------------+
| Line #	   | Description	 			|
+------------------+--------------------------------------------+
+------------------+--------------------------------------------+
| AppClass.h	   | 						|
+------------------+--------------------------------------------+
| 23 		   | Create vector of vectors (m_orbits).	|
+------------------+--------------------------------------------+
| 24 		   | Create vector of uint (m_cycleIDs). 	|
+------------------+--------------------------------------------+
+------------------+--------------------------------------------+
| AppClass.cpp	   | 						|
+------------------+--------------------------------------------+
| 42 		   | Generating route waypoints at runtime.	|
+------------------+--------------------------------------------+
| 111 		   | Render green waypoint spheres for context.	|
+------------------+--------------------------------------------+
| 125 		   | Generate the timer and delta time for lerp.|
+------------------+--------------------------------------------+
| 148 		   | Switch sections when a lerp reaches an end.|
+------------------+--------------------------------------------+
| 182 		   | Loop through all the orbits.		|
+------------------+--------------------------------------------+
| 205 		   | Depth of each orbit is calculated.		|
+------------------+--------------------------------------------+
| 235 		   | Calculation of the actual lerp function.	|
+------------------+--------------------------------------------+
| 240 		   | Draw the white orbiting spheres.		|
+------------------+--------------------------------------------+