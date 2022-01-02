The QueryCloestPoint project is build on top of my personal rendering engine PotatoEngine. The engine is in the very early stage but have basic 3D rendering. I thought it would be good to visualize the mesh, query point and the cloest point result, so I use my own engine to do the job.

# Build:
For now only Visual Studio 2019 build is supported.
1. Init and update submodule
   ```
   git submodule init
   git submodule update
   ```
2. Open `PotatoEngine.sln`.
3. Right click project `QueryClosestPoint`, select "Set as Start Up Project".
4. In `QueryClosestPoint` project properties, Configuration Properties->Debugging, change "Working Directory" to `$(SolutionDir)`.
5. Desire obj model path need to be fill in "Command Arguments". In resources/objects, there are some obj you can try. For example, you can enter `resources/objects/dragon/dragon.obj`.
6. Select desire build. Build platform need to be x64.
7. Press F5 to build and run the application.

## Build test
DebugTest/ReleaseTest mode will run the simple unit test.
The test will load relevant test data base on which model you load. If the test data doesn't exist yet, you need to generate it first. To do so, uncomment line 20 in `main.cpp` then run the test:
```
test.GenerateTestPointsAndResults();
```
If you change the parameter in `ClosestPiontUnitTest` constructor, you will have to regenerate the test data too.

# Usage: 
```
QueryClosestPoint [modelPath]
```
The query point is red and the result closest point is green.

# Operations:
1. Press TAB key to switch to console for query point and max search distance input. You need to manually switch the focus window to console
2. Right click mouse to enable camera movement (W A S D)
3. Press O key to toggle model kd-tree draw
4. Press I/K to increase/decrease maximum kd-tree depth to be drew

# Assumption
This application generate a KD tree for the test model for fast closest point query. It is a basic KD tree implementation which assume all model vertices are spread in 3D space evenly. It cycle through each dimension and choose the median point for split plane in each depth. That is, it doesn't consider situation that some points may only spread out in one dimension. I have also implemented different split axis choosing method, but for the Stanford dragon model, it doesn't seem improvement.

# Notes
1. The kd-tree search code is in `PotatoEngine::SearchNearest(\**\)`
2. MeshModel class is for surface rendering, and the shader isn't finished yet, please ignore it.
3. GLObject class is for share_ptr style managing OpenGL object. It isn't finished so ignore it as well.
4. There is still a lot of engineering need to be done such as a renderer, input/output, etc.
