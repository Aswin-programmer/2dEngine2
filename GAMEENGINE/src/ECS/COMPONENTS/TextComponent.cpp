#include "pch.h"

#include "ECS/COMPONENTS/TextComponent.h"

void TextComponent::CreateLuaTextBindings( sol::state& lua )
{
	lua.new_usertype<TextComponent>(
		"TextComponent",
		"type_id", entt::type_hash<TextComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]( const std::string& sFontName, const std::string& sTextStr, Color color, int padding, float wrap ) {
				return TextComponent{
					.sFontName = sFontName,
					.sTextStr = sTextStr,
					.padding = padding,
					.wrap = wrap,
					.color = color };
			},
			[]( const std::string& sFontName, const std::string& sTextStr ) {
				return TextComponent{
					.sFontName = sFontName,
					.sTextStr = sTextStr };
			} ),
		"textStr", &TextComponent::sTextStr,
		"fontName", &TextComponent::sFontName,
		"padding", &TextComponent::padding,
		"padding", &TextComponent::padding,
		"bHidden", &TextComponent::bHidden,
		"wrap", &TextComponent::wrap,
		"color", &TextComponent::color );
}
