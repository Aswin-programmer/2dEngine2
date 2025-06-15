ProjectileDefs = 
{
	regular_shot = {
		tag = "regular_shot",
		group = "projectile",
		components = {
			transform = {
				position = vec2(0, 0),
				scale = vec2(1, 1),
				rotation = 0.0
			},
			sprite = {
				asset_name = "player_projectile",
				width = 16,
				height = 16,
				start_x = 0,
				start_y = 0,
				layer = 3
			},
			animation = {
				num_frames = 4,
				frame_rate = 20,
				frame_offset = 0,
				bVertical = false,
				bLooped = true
			},
			circle_collider = {
				radius = 8,
				offset = vec2(8, 8)
			},
			physics_attributes = {
				type = BodyType.Dynamic,
                bInteractionType = BodyInteractionType.SENSOR,
				density = 1.0,
				friction = 1.0,
				restitution = 0.0,
				position = vec2(0, 0),
				radius = 8,
				bCircle = true,
				bFixedRotation = true,
                gravityScale = -2,
				scale = vec2(1,1),
				object_data = {
					tag = "shot",
					group = "projectile",
					bCollider = false,
                    bTrigger = false,
                    bSensor = true
				}
			}
		}
	}
}
