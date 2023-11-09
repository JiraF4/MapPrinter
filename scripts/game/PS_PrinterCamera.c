[ComponentEditorProps(category: "GameScripted/Character", description: "")]
class PS_PrinterCameraClass : ScriptComponentClass
{
};


class PS_PrinterCamera : ScriptComponent
{
	/*
	int MaxPosX = 1792;
	int MaxPosY = 1612;
	*/
	
	int MaxPosX = 22;
	int MaxPosY = 22;
	
	int PosX = 0;
	int PosY = 0;
	
	float Distance = 30.0;
	
	float OrthoSize = 8.0;
	
	vector StartPos;
	
	int frameTime = 1000;
	
	int StartOffsetX = 1158;
	int StartOffsetY = 134;
		
	override void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;
		
		FileIO.MakeDirectory("$profile:MapPrint");
		
		//
		
		float sizeX;
		float sizeY;
		GetGame().GetWorkspace().GetScreenSize(sizeX, sizeY);
		float offsetX = OrthoSize * (sizeX / sizeY);
		float offsetY = OrthoSize;
		
		StartPos = "0 0 0";
		StartPos += vector.Right * offsetX * (float) StartOffsetX;
		StartPos += vector.Forward * offsetY * (float) StartOffsetY;
		
		GetOwner().SetOrigin("0 -10 0");
		
		//StartPos = owner.GetOrigin();
		
		GetGame().GetCallqueue().CallLater(PrintTile, 0);
	}
	
	void SetLOD()
	{
		vector minPos, maxPos;
		GetGame().GetWorld().GetBoundBox(minPos, maxPos);
		GetGame().GetWorld().QueryEntitiesByAABB(minPos, maxPos, SetEntityLOD);
		
		/*
		GetGame().GetCallqueue().CallLater(SetLOD, 100);
		IEntity entity = GetGame().GetWorld().FindEntityByName("MPTestTerrain");
		GetGame().GetWorld().GetAll
		while (entity)
		{
			entity.SetFixedLOD(0);
			entity = entity.GetSibling();
		}
		*/
		
		/*
		array<IEntity> entities = new array<IEntity>();
		GetGame().GetWorld().GetActiveEntities(entities);
		foreach(IEntity entity: entities)
		{
			entity.SetFixedLOD(0);
		}
		*/
		//GetGame().GetWorld().QueryEntitiesByAABB("0 0 0", "999999 999999 999999", SetEntityLOD);
		
		GetGame().GetCallqueue().CallLater(PrintTile, 0);
	}
	
	bool SetEntityLOD(IEntity entity)
	{
		Print(entity.GetName());
		entity.SetFixedLOD(0);
		return false;
	}
	
	
	void PrintTile()
	{
		GetGame().GetCallqueue().CallLater(PrintTile, frameTime);
				
		int cameraId;
		vector cameraMatrix[4];
		GetGame().GetWorld().GetCamera(cameraId, cameraMatrix);
		GetGame().GetWorld().SetCameraHDRBrightness(cameraId, 0.01);
		
		PlayerController playerController = GetGame().GetPlayerController();
		if (!playerController)
			return;
		
		int playerId = playerController.GetPlayerId();
		if (playerId <= 0)
			return;
		
		if (GetGame().GetWorld().GetWorldTime() < 5000)
			return;
		
		/*
		if (Replication.IsServer())
			return;
		*/
		
		if (PosY > MaxPosY)
			return;
		
		int playerOffsetY = (MaxPosY * (playerId - 1));
		int x = PosX;
		if (x == 0) x = MaxPosX;
		string path = "$profile:MapPrint/" + (StartOffsetX + x).ToString(4) + "-" + (PosY + playerOffsetY + StartOffsetY).ToString(4);
		
		
		if (GetOwner().GetOrigin()[1] > 1.0)
			System.MakeScreenshot(path);
		
		float sizeX;
		float sizeY;
		GetGame().GetWorkspace().GetScreenSize(sizeX, sizeY);
		
		vector dir;
		GetGame().GetWorld().ProjectViewportToWorld(sizeX, sizeY, cameraId, sizeX, sizeY, dir);
				
		vector origin = StartPos;
		float offsetX = dir[0] * Distance * 2;
		float offsetY = dir[0] * Distance * 2;
		
		offsetX = OrthoSize * (sizeX / sizeY);
		offsetY = OrthoSize;
		
		origin += vector.Right * offsetX * (float) PosX;
		origin += vector.Forward * offsetY * (float) (PosY + playerOffsetY);
		
		origin[1] = GetGame().GetWorld().GetSurfaceY(origin[0], origin[2]) + Distance;
		GetOwner().SetOrigin(origin);
		
		PosX++;
		if (PosX > MaxPosX)
		{
			PosX = 0;
			PosY++;
		}
	}
};