#include "pch.h"

#include "ECS/COMPONENTS/SpriteComponent.h"

#include "ECS/MainRegistry.h"
#include "LOGGER/log.h"
#include "RENDERER/TEXTURE/Texture.h";
#include "RESOURCES/AssetManager.h"

void SpriteComponent::CreateSpriteLuaBind( sol::state& lua, Registry& registry )
{
	lua.new_usertype<Color>(
		"Color",
		sol::call_constructor,
		sol::factories(
			[]( GLubyte r, GLubyte g, GLubyte b, GLubyte a ) {
				return Color{ .r = r, .g = g, .b = b, .a = a };
			} ),
		"r", &Color::r,
		"g", &Color::g,
		"b", &Color::b,
		"a", &Color::a );

	lua.new_usertype<UVs>(
		"UVs",
		sol::call_constructor,
		sol::factories(
			[]( float u, float v ) {
				return UVs{ .u = u, .v = v };
			} ),
		"u", &UVs::u,
		"v", &UVs::v,
		"uv_width", &UVs::uv_width,
		"uv_height", &UVs::uv_height );

	lua.new_usertype<SpriteComponent>(
		"Sprite",
		"type_id", &entt::type_hash<SpriteComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]( const std::string& textureName, float width, float height, int start_x, int start_y, int layer ) {
				return SpriteComponent{
					.width = width,
					.height = height,
					.uvs = UVs{},
					.color = Color{ 255, 255, 255, 255 },
					.start_x = start_x,
					.start_y = start_y,
					.layer = layer,
					.texture_name = textureName };
			} ),
		"texture_name", &SpriteComponent::texture_name,
		"width", &SpriteComponent::width,
		"height", &SpriteComponent::height,
		"start_x", &SpriteComponent::start_x,
		"start_y", &SpriteComponent::start_y,
		"layer", &SpriteComponent::layer,
		"color", &SpriteComponent::color,
		"bHidden", &SpriteComponent::bHidden,
		"uvs", &SpriteComponent::uvs,
		"generate_uvs", [&]( SpriteComponent& sprite ) {
			auto& mainRegistry = MAIN_REGISTRY();
			auto& assetManager = mainRegistry.GetAssetManager();

			auto& texture = assetManager.GetTexture( sprite.texture_name );
			if ( texture.getID() == 0 )
			{
				LOG_ERROR( "Failed to generate uvs -- Texture [{}] -- Does not exists or invalid", sprite.texture_name );
				return;
			}
			sprite.generate_uvs( texture.getWidth(), texture.getHeight() );
		} );
}
