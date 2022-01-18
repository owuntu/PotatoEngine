The ClosestPointQuery project is build on top of my personal rendering engine PotatoEngine. The engine is in the very early stage but have basic 3D rendering. I thought it would be good to visualize the mesh, query point and the cloest point result, so I use my own engine to do the job.

# Assumption
* The mesh data is taken from an obj model file.
* Assuming all mesh faces is triangle

# Solution Motivation
For fast query closest point on mesh, first I have to solve how to query the closest point on a single triangle. The method I use is in the book "Real-time Collision Detection" by Erison Christer.

But there is still a lot of triangles to query, so I use BVH to do the acceleartion. Compare to Kd-tree, I think BVH doesn't have to worry about if a triangle is lie on the split plane. I can simply wrap it to a child box because BVH allow children to be overlapped. The key acceleration is that, if the bounding box distance is further than the temporory result has been found, it can skip the every triangles inside the bounding box.

# Build & Run:
This branch doesn't support model visualization. To enable it, switch to branch `ClosetPointQuery` and use Visual Studio 2019 to build.
1. Init and update submodule
   ```
   $ git submodule init
   $ git submodule update
   ```
2. To build the application, run the script:
   ```
   $ build.sh
   ```
3. The application now is located in `ClosestPointQuery/ClosestPointQuery`. To run, jump into the directory and load an obj model file:
   ```
   $ cd ClosestPointQuery
   $ ./ClosestPointQuery ../resources/objects/dragon/dragon.obj
   ```
4. Type in the desire query point and max search distance into command line:
   ```
   Building BVH ...

   Please input the queary point and max search distance: x y z distance
   1 1 1 5
   ```
   and hit enter to see the result:
   ```
   Input point and max distance: (1, 1, 1), 5
   Closest point is: (0.093636, 0.148634, -0.004483)
   ```

## Build test
Run the `build.sh` script with `test` argutment
```
$ build.sh test
```
Then the `ClosestPointQuery` application is a test now. When run it, it will load the pre-generate test data and run to compare it with the BVH query result and brute force method's.
```
$ cd ClosestPointQuery
$ ./ClosestPointQuery ../resources/objects/dragon/dragon.objRunning all tests
Building BVH ...
All BVH search took 326 milliseconds
Building BVH ...
All brute force search took 29168 milliseconds
```
 The test will load relevant test data base on which model you load. If the test data doesn't exist yet, you need to generate it first. To do so, uncomment line 20 in `main.cpp` then run the test:
```cpp
test.GenerateTestPointsAndResults();
```
If you change the parameter in `ClosestPiontUnitTest` constructor, you will have to regenerate the test data too.
