[ComponentEditorProps(category: "GameScripted/Character", description: "")]
class PS_PrinterCameraClass : ScriptComponentClass
{
};


class PS_PrinterCamera : ScriptComponent
{
	/* size of everon in 8x8m tiles
	int MaxPosX = 1792;
	int MaxPosY = 1612;
	*/
	
	// Size of camera rendering space in meters
	float OrthoSize = 8.0;
	
	// Position of rendering rect
	int StartOffsetX = 1158;
	int StartOffsetY = 134;
		
	// Size of rendering rect
	int MaxPosX = 22;
	int MaxPosY = 22;
	
	// Position IN rendering rect
	// Can be set manualy for continue rendering
	int PosX = 0;
	int PosY = 0;
	
	// Distance from ground for rendering camera
	float Distance = 30.0;
	
	// Time spend for each tile
	int frameTime = 1000;
	
	vector StartPos;	
	override void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;
		
		// Create dirrectory for tiles
		FileIO.MakeDirectory("$profile:MapPrint");
		
		// Set starting world position for camera
		float sizeX;
		float sizeY;
		GetGame().GetWorkspace().GetScreenSize(sizeX, sizeY);
		float offsetX = OrthoSize * (sizeX / sizeY);
		float offsetY = OrthoSize;
		StartPos = "0 0 0";
		StartPos += vector.Right * offsetX * (float) StartOffsetX;
		StartPos += vector.Forward * offsetY * (float) StartOffsetY;
		GetOwner().SetOrigin("0 -10 0");
		
		GetGame().GetCallqueue().CallLater(PrintTile, 0);
	}
	
	void PrintTile()
	{
		// Next frame
		GetGame().GetCallqueue().CallLater(PrintTile, frameTime);
		
		// Get current camera and fight with HDR
		int cameraId;
		vector cameraMatrix[4];
		GetGame().GetWorld().GetCamera(cameraId, cameraMatrix);
		GetGame().GetWorld().SetCameraHDRBrightness(cameraId, 0.01);
		
		// Await player controller
		PlayerController playerController = GetGame().GetPlayerController();
		if (!playerController)
			return;
		int playerId = playerController.GetPlayerId();
		if (playerId <= 0)
			return;
		
		// Await 5 seconds, some thing need to be loaded
		if (GetGame().GetWorld().GetWorldTime() < 5000)
			return;
		
		// Stop rendering at rect end position
		if (PosY > MaxPosY)
			return;
		
		// There some multithread system if you use it with server
		int playerOffsetY = (MaxPosY * (playerId - 1));
		
		// Make tile path
		int x = PosX;
		if (x == 0) x = MaxPosX;
		string path = "$profile:MapPrint/" + (StartOffsetX + x).ToString(4) + "-" + (PosY + playerOffsetY + StartOffsetY).ToString(4);
		
		// Make screenshoot if camera y > 1.0
		if (GetOwner().GetOrigin()[1] > 1.0)
			System.MakeScreenshot(path);
		
		/* For perspective camera
		vector dir;
		GetGame().GetWorld().ProjectViewportToWorld(sizeX, sizeY, cameraId, sizeX, sizeY, dir);
		vector origin = StartPos;
		float offsetX = dir[0] * Distance * 2;
		float offsetY = dir[0] * Distance * 2;
		*/
		
		// Get screen size
		float sizeX;
		float sizeY;
		GetGame().GetWorkspace().GetScreenSize(sizeX, sizeY);
		
		// Get offset from start position
		float offsetX = OrthoSize * (sizeX / sizeY);
		float offsetY = OrthoSize;
		
		// Get new camera position
		origin += vector.Right * offsetX * (float) PosX;
		origin += vector.Forward * offsetY * (float) (PosY + playerOffsetY);
		origin[1] = GetGame().GetWorld().GetSurfaceY(origin[0], origin[2]) + Distance;
		GetOwner().SetOrigin(origin); // And move camera
		
		// Next grid position
		PosX++;
		if (PosX > MaxPosX)
		{
			PosX = 0;
			PosY++;
		}
	}
};