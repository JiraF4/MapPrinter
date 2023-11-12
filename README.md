Repository contains:
Reforger addon - root folder.
MapPrinter/Add/RunPrint.bat - File for launching the game.
MapPrinter/Add/ReforgerMapPrinter/ReforgerMapPrinter.sln - C# solution for working with images.

Mostly it's just bunch of scripts, many things are done manually.

Just copy it to your add-ons folder and open it in your workbench.
Create a subscene of your world with camera and gamemode.
You can copy layer from Everon map.

Set up rendering zone in:
MapPrinter/scripts/game/PS_PrinterCamera.c

MaxPosX, MaxPosY - Rendering rect bounds (In grid size each cell is OrthoSizexOrthoSize)
StartOffsetX , StartOffsetY- Rendering rect start position (In grid size each cell is OrthoSizexOrthoSize)
frameTime - Time spend for each frame, you can set it to 0, for make screenshoots every frame, but game takes time for load meshes.
Distance - Distance from ground for rendering camera.
OrthoSize - Size in meter for ortho camera, you may change it also in world. (Bigger than ~16.0 broke camera for some reason)

Then configure and run:
MapPrinter/Add/RunPrint.bat
Change pathes to yours, set -screenWidth and -screenHeight to preffered resolution, -world to world you render and -profile to folder where you want store screenshoots.

MapPrinter/Add/ReforgerMapPrinter/ReforgerMapPrinter.sln
C# solution to work with screenshoots, it's a "little" dirty...
At least I added comments, I hope this will help you figure it out.
