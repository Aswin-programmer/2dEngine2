#pragma once

#include "ECS/COMPONENTS/AllComponents.h"
#include "PHYSICS/UserData.h"

constexpr const char* DROP_TEXTURE_SRC = "DropTextureSource";
constexpr const char* DROP_FONT_SRC = "DropFontSource";
constexpr const char* DROP_SOUNDFX_SRC = "DropSoundFxSource";
constexpr const char* DROP_MUSIC_SRC = "DropMusicSource";
constexpr const char* DROP_SCENE_SRC = "DropSceneSource";

struct Canvas
{
	int width{ 640 }, height{ 480 }, tileWidth{ 16 }, tileHeight{ 16 };
};

struct Tile
{
	uint32_t id{ entt::null };
	TransformComponent transform{};
	SpriteComponent sprite{};
	AnimationComponent animation{};
	BoxColliderComponent boxCollider{};
	CircleColliderComponent circleCollider{};
	PhysicsComponent physics{};
	ObjectData objectData{};

	bool bCollider{ false }, bAnimation{ false }, bPhysics{ false }, bCircle{ false };
};
