AsteroidDefs = 
{
	asteroid_big = 
	{
		group = "asteroids",
		components = 
		{
			transform = {
				position = { x = -100, y = -100},
				scale = { x = 1, y = 1},
				rotation = 0
			}, 
			sprite = {
				asset_name = "ast_1",
				width = 98,
				height = 96,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 48
			}
		},
		type = "big",
		min_speed = -2,
		max_speed = 5
	},
	asteroid_small = 
	{
		group = "asteroids",
		components = 
		{
			transform = {
				position = { x = -100, y = -100},
				scale = { x = 1, y = 1},
				rotation = 0
			}, 
			sprite = {
				asset_name = "ast_2",
				width = 43,
				height = 43,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 22
			}
		},
		type = "small",
		min_speed = -5,
		max_speed = 5
	},
}
ShipDefs = 
{
	blue_ship = 
	{
		tag = "ship",
		components = 
		{
			transform = {
				position = { x = 320, y = 240},
				scale = { x = 1, y = 1},
				rotation = 0
			}, 
			sprite = {
				asset_name = "ship",
				width = 75,
				height = 99,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 38
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
	proj_1 =
	{
		group = "projectiles",
		components = 
		{
			transform = {
				position = { x = 0, y = 0},
				scale = { x = 1, y = 1},
				rotation = 0
			}, 
			sprite = {
				asset_name = "proj_1",
				width = 37,
				height = 13,
				start_x = 0, start_y = 0,
				layer = 2
			},
			circle_collider = {
				radius = 12
			}
		},
		life_time = 2000,
		proj_speed = 10
	}
}

HudDefs = 
{
	lives = 
	{
		group = "lives",
		components = 
		{
			transform = {
				position = { x = 0, y = 0},
				scale = { x = 1, y = 1},
				rotation = 0
			}, 
			sprite = {
				asset_name = "lives",
				width = 33,
				height = 26,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	score = 
	{
		tag = "score",
		components = 
		{
			transform = {
				position = { x = 200, y = 0},
				scale = { x = 2, y = 2},
				rotation = 0
			}, 
			sprite = {
				asset_name = "score",
				width = 80,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	number = 
	{
		group = "numbers",
		components = 
		{
			transform = {
				position = { x = 380, y = 0},
				scale = { x = 2, y = 2},
				rotation = 0
			}, 
			sprite = {
				asset_name = "numbers",
				width = 19,
				height = 19,
				start_x = 0, start_y = 0,
				layer = 10
			}
		}
	},
	game_over = 
	{
		group = "game_over",
		components = 
		{
			transform = {
				position = { x = (WINDOW_WIDTH/2) - 144, y = (WINDOW_HEIGHT/2) - 16},
				scale = { x = 2, y = 2},
				rotation = 0
			}, 
			sprite = {
				asset_name = "game_over",
				width = 144,
				height = 16,
				start_x = 0, start_y = 0,
				layer = 10,
				bHidden = true
			}
		}
	}
}
