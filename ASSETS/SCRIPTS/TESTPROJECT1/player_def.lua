assert(MoveState, "MoveState does not exist")


CharacterStates = 
{
	move = MoveState
}

PlayerDefs = 
{
	main_player = 
	{
		tag = "player",
		group = "",
		type = "PLAYER",
		components = 
		{
			transform = {
				position = vec2(528,224),
				scale = vec2(0.25,0.25),
				rotation = 0.0
			},
			sprite = {
				asset_name = "fireboy",
				width = 128, 
				height = 128, 
				start_x = 0, 
				start_y = 0, 
				layer = 2
			},
			animation = {
				num_frames = 2,
				frame_rate = 6,
				frame_offset = 0,
				bVertical = false,
				bLooped = true
			},
			circle_collider = {
				radius = 64,
				offset = vec2(0, 0)
			},
			physics_attributes = {
				type = BodyType.Dynamic,
				bInteractionType = BodyInteractionType.NORMAL,
				density = 75.0,
				friction = 1.0,
				restitution = 0.0,
				position = vec2(528,224),
				radius = 64,
				bCircle = true,
				bFixedRotation = true,
				gravityScale = -2,
				scale = vec2(0.25,0.25),
				object_data = {
					tag = "player",
					group = "",
					bCollider = true,
					bTrigger = false,
					bSensor = false
				}
			}
		},
		controller = { "move" },
		default_state = "move"
	}
}