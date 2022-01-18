The ClosestPointQuery project is build on top of my personal rendering engine PotatoEngine. The engine is in the very early stage but have basic 3D rendering. I thought it would be good to visualize the mesh, query point and the cloest point result, so I use my own engine to do the job.

# Assumption
* The mesh data is taken from an obj model file.
* Assuming all mesh faces is triangle

# Solution Motivation
For fast query closest point on mesh, first I have to solve how to query the closest point on a single triangle. The method I use is in the book "Real-time Collision Detection" by Erison Christer.

But there is still a lot of triangles to query, so I use BVH to do the acceleartion. Compare to Kd-tree, I think BVH doesn't have to worry about if a triangle is lie on the split plane. I can simply wrap it to a child box because BVH allow children to be overlapped. The key acceleration is that, if the bounding box distance is further than the temporory result has been found, it can skip the every triangles inside the bounding box.

# Build:
For now the graphics support is only available in Visual Studio 2019 build. For Linux build, please switch to branch `CloestPointQueryNoGraphics`.
1. Init and update submodule
   ```
   git submodule init
   git submodule update
   ```
2. Open `PotatoEngine.sln`.
3. Right click project `ClosestPointQuery`, select "Set as Start Up Project".
4. Desire obj model path need to be fill in "Command Arguments". In resources/objects, there are some obj you can try. For example, you can enter `../resources/objects/dragon/dragon.obj`.
5. Select desire build. Build platform need to be x64.
6. Press F5 to build and run the application.

## Build test
DebugTest/ReleaseTest mode will run the simple unit test.
The test will load relevant test data base on which model you load. If the test data doesn't exist yet, you need to generate it first. To do so, uncomment line 20 in `main.cpp` then run the test:
```
test.GenerateTestPointsAndResults();
```
If you change the parameter in `ClosestPiontUnitTest` constructor, you will have to regenerate the test data too.

# Usage: 
```
ClosestPointQuery [modelPath]
```
The query point is red and the result closest point is green.

# Operations:
* Press TAB key to switch to console for query point and max search distance input. You need to manually switch the focus window to console
* Right click mouse to enable camera movement (W A S D)
* Press I/K to increase/decrease the desired depth to be drew
