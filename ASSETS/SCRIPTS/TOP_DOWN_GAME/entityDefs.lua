PlayerDef = 
{
	playerIDLE =
	{
		tag = "player_idle",
		group = "player",
		components = 
		{
			transform = 
			{
				position = { x = 320, y = 240},
				scale = { x = 1, y = 1},
				rotation = 0
			},
			sprite =
			{
				asset_name = "playerIdle",
				width = 200,
				height = 132,
				start_x = 0,
				start_y = 0,
				layer = 2
			},
			animation = 
			{
				num_frames = 0,
				frame_rate = 0,
				frame_offset = 0,
				bVertical = false,
				bLooped = false
			}
		}
	}
}

ProjectileDefs = 
{
	proj1 = 
	{
		group = "projectiles",
		components = 
		{
			transform = 
			{
				position = {x=0,y=0},
				scale = {x=1,y=1},
				rotation = 0
			},
			sprite = 
			{
				asset_name = "proj_1",
				width = 8,
				height = 8,
				start_x = 0, start_y = 0,
				layer = 2
			}
		},
		life_time = 2000,
		proj_speed = 10
	}
}

ZombieDefs = 
{
	Zombie = 
	{
		group = "zombie",
		components = 
		{
			transform = 
			{
				position = {x=0,y=0},
				scale = {x=1,y=1},
				rotation = 0
			},
			sprite = 
			{
				asset_name = "zombieMove",
				width = 288,
				height = 311,
				start_x = 0, start_y = 0,
				layer = 3
			}
		},
		zombie_speed_min = 1,
		zombie_speed_max = 2,
		zombie_health = 15
	}
}
