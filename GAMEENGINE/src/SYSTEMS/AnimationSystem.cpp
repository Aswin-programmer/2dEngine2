#include "pch.h"

#include "SYSTEMS/AnimationSystem.h"

#include "ECS/Registry.h"
#include "ECS/COMPONENTS/AnimationComponent.h"
#include "ECS/COMPONENTS/SpriteComponent.h"
#include "ECS/COMPONENTS/TransformComponent.h"

AnimationSystem::AnimationSystem( Registry& registry )
	: m_Registry{ registry }
{
}

void AnimationSystem::Update()
{
	auto view = m_Registry.GetRegistry().view<AnimationComponent, SpriteComponent, TransformComponent>();
	for ( auto entity : view )
	{
		const auto& transform = view.get<TransformComponent>( entity );
		auto& sprite = view.get<SpriteComponent>( entity );
		auto& animation = view.get<AnimationComponent>( entity );

		// Calculate elapsed time in seconds
		double currentTime = glfwGetTime();

		if ( animation.numFrames <= 0 )
			continue;

		// if we are not looped and the current_frame == num_frame, skip
		if ( !animation.bLooped && animation.currentFrame >= animation.numFrames - 1 )
			continue;

		// Calculate current frame based on time and frame rate
		// frameRate is in frames per second, so we multiply by time to get current frame
		animation.currentFrame = static_cast<int>( ( ( currentTime - animation.startTime ) * animation.frameRate ) ) % animation.numFrames;
		// Update UV coordinates based on animation direction
		if ( animation.bVertical )
		{
			// For vertical spritesheet, update v coordinate
			sprite.uvs.v = ( animation.currentFrame * sprite.uvs.uv_height );
			sprite.uvs.u = ( animation.frameOffset * sprite.uvs.uv_width );
		}
		else
		{
			// For horizontal spritesheet, update u coordinate
			sprite.uvs.u = ( animation.frameOffset + animation.currentFrame ) * sprite.uvs.uv_width;
		}
	}
}
