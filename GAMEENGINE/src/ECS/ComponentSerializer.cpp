#include "pch.h"

#include "ECS/ComponentSerializer.h"
#include "FILESYSTEM/SERIALIZERS/JSONSerializer.h"

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, TransformComponent& transform )
{
	serializer
		.StartNewObject( "transform" )
		.StartNewObject( "position" )
		.AddKeyValuePair( "x", transform.position.x )
		.AddKeyValuePair( "y", transform.position.y )
		.EndNewObject()
		.StartNewObject( "scale" )
		.AddKeyValuePair( "x", transform.scale.x )
		.AddKeyValuePair( "y", transform.scale.y )
		.EndNewObject()
		.AddKeyValuePair( "rotation", transform.rotation )
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, SpriteComponent& sprite )
{
	serializer
		.StartNewObject( "sprite" )
		.AddKeyValuePair( "width", sprite.width )
		.AddKeyValuePair( "height", sprite.height )
		.AddKeyValuePair( "startX", sprite.start_x )
		.AddKeyValuePair( "startY", sprite.start_y )
		.AddKeyValuePair( "layer", sprite.layer )
		.AddKeyValuePair( "sTexture", sprite.texture_name )
		.StartNewObject( "uvs" )
		.AddKeyValuePair( "u", sprite.uvs.u )
		.AddKeyValuePair( "v", sprite.uvs.v )
		.AddKeyValuePair( "uv_width", sprite.uvs.uv_width )
		.AddKeyValuePair( "uv_height", sprite.uvs.uv_height )
		.EndNewObject()
		.StartNewObject( "color" )
		.AddKeyValuePair( "r", sprite.color.r )
		.AddKeyValuePair( "g", sprite.color.g )
		.AddKeyValuePair( "b", sprite.color.b )
		.AddKeyValuePair( "a", sprite.color.a )
		.EndNewObject()
		.AddKeyValuePair( "bHidden", sprite.bHidden )
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, AnimationComponent& animation )
{
	serializer
		.StartNewObject( "animation" )
		.AddKeyValuePair( "numFrames", animation.numFrames )
		.AddKeyValuePair( "frameRate", animation.frameRate )
		.AddKeyValuePair( "frameOffset", animation.frameOffset )
		.AddKeyValuePair( "bVertical", animation.bVertical )
		.AddKeyValuePair( "bLooped", animation.bLooped )
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, BoxColliderComponent& boxcollider )
{
	serializer
		.StartNewObject( "boxCollider" )
		.AddKeyValuePair( "width", boxcollider.width )
		.AddKeyValuePair( "height", boxcollider.height )
		.StartNewObject( "offset" )
		.AddKeyValuePair( "x", boxcollider.offset.x )
		.AddKeyValuePair( "y", boxcollider.offset.y )
		.EndNewObject()
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, CircleColliderComponent& circlecollider )
{
	serializer
		.StartNewObject( "circleCollider" )
		.AddKeyValuePair( "radius", circlecollider.radius )
		.StartNewObject( "offset" )
		.AddKeyValuePair( "y", circlecollider.offset.y )
		.AddKeyValuePair( "x", circlecollider.offset.x )
		.EndNewObject()
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, TextComponent& text )
{
	serializer
		.StartNewObject( "text" ) // Start text table
		.AddKeyValuePair( "text", text.sTextStr )
		.AddKeyValuePair( "fontName", text.sFontName )
		.AddKeyValuePair( "padding", text.padding )
		.AddKeyValuePair( "wrap", text.wrap )
		.AddKeyValuePair( "bHidden", text.bHidden )
		.StartNewObject( "color" ) // Start Color table
		.AddKeyValuePair( "r", static_cast<int>( text.color.r ) )
		.AddKeyValuePair( "g", static_cast<int>( text.color.g ) )
		.AddKeyValuePair( "b", static_cast<int>( text.color.b ) )
		.AddKeyValuePair( "a", static_cast<int>( text.color.a ) )
		.EndNewObject()	 // End Color table
		.EndNewObject(); // End Text table
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, PhysicsComponent& physics )
{
	PhysicsAttributes& attributes = physics.GetAttributes();

	serializer
		.StartNewObject( "physics" )
		.StartNewObject( "attributes" )
		.AddKeyValuePair( "type", static_cast<int>( attributes.eType ) )
		.AddKeyValuePair( "interactionType", static_cast<int>( attributes.bInteractionType ) )
		.AddKeyValuePair( "density", attributes.density )
		.AddKeyValuePair( "friction", attributes.friction )
		.AddKeyValuePair( "restitution", attributes.restitution )
		.AddKeyValuePair( "restitutionThreshold", attributes.restitutionThreshold )
		.AddKeyValuePair( "radius", attributes.radius )
		.AddKeyValuePair( "gravityScale", attributes.gravityScale )
		.StartNewObject( "position" )
		.AddKeyValuePair( "x", attributes.position.x )
		.AddKeyValuePair( "y", attributes.position.y )
		.EndNewObject()
		.StartNewObject( "scale" )
		.AddKeyValuePair( "x", attributes.scale.x )
		.AddKeyValuePair( "y", attributes.scale.y )
		.EndNewObject()
		.StartNewObject( "boxSize" )
		.AddKeyValuePair( "x", attributes.boxSize.x )
		.AddKeyValuePair( "y", attributes.boxSize.y )
		.EndNewObject()
		.StartNewObject( "offset" )
		.AddKeyValuePair( "x", attributes.offset.x )
		.AddKeyValuePair( "y", attributes.offset.y )
		.EndNewObject()
		.AddKeyValuePair( "bCircle", attributes.bCircle )
		.AddKeyValuePair( "bBoxShape", attributes.bBoxShape )
		.AddKeyValuePair( "bFixedRotation", attributes.bFixedRotation )
		.AddKeyValuePair( "bIsSensor", attributes.bIsSensor )
		.AddKeyValuePair( "filterCategory", static_cast<unsigned>( attributes.filterCategory ) )
		.AddKeyValuePair( "filterMask", static_cast<unsigned>( attributes.filterMask ) )
		.AddKeyValuePair( "groupIndex", static_cast<int>( attributes.groupIndex ) )
		.StartNewObject( "objectData" )
		.AddKeyValuePair( "tag", attributes.objectData.tag )
		.AddKeyValuePair( "group", attributes.objectData.group )
		.AddKeyValuePair( "bCollider", attributes.objectData.bCollider )
		.AddKeyValuePair( "bTrigger", attributes.objectData.bTrigger )
		.AddKeyValuePair( "bIsSensor", attributes.objectData.bSensor )
		.EndNewObject()
		.EndNewObject()
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent( JSONSerializer& serializer, RigidBodyComponent& rigidbody )
{
	serializer
		.StartNewObject( "rigidBody" ) // Start rigidbody table
		.StartNewObject( "velocity" )  // Start velocity table
		.AddKeyValuePair( "x", rigidbody.velocity.x )
		.AddKeyValuePair( "y", rigidbody.velocity.y )
		.EndNewObject()	 // end velocity table
		.EndNewObject(); // End rigid body table
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, TransformComponent& transform )
{
	transform.position =
		glm::vec2{ jsonValue["position"]["x"].GetFloat(), jsonValue["position"]["y"].GetFloat() };

	transform.scale = glm::vec2{ jsonValue["scale"]["x"].GetFloat(), jsonValue["scale"]["y"].GetFloat() };
	transform.rotation = jsonValue["rotation"].GetFloat();
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, SpriteComponent& sprite )
{
	sprite.width = jsonValue["width"].GetFloat();
	sprite.height = jsonValue["height"].GetFloat();

	sprite.uvs = UVs{ .u = jsonValue["uvs"]["u"].GetFloat(),
		.v = jsonValue["uvs"]["v"].GetFloat(),
		.uv_width = jsonValue["uvs"]["uv_width"].GetFloat(),
		.uv_height = jsonValue["uvs"]["uv_height"].GetFloat() };

	sprite.start_x = jsonValue["startX"].GetInt();
	sprite.start_y = jsonValue["startY"].GetInt();
	sprite.layer = jsonValue["layer"].GetInt(), sprite.bHidden = jsonValue["bHidden"].GetBool();
	sprite.texture_name = jsonValue["sTexture"].GetString();
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, AnimationComponent& animation )
{
	animation.numFrames = jsonValue["numFrames"].GetInt();
	animation.frameRate = jsonValue["frameRate"].GetInt();
	animation.frameOffset = jsonValue["frameOffset"].GetInt();
	animation.bVertical = jsonValue["bVertical"].GetBool();
	animation.bLooped = jsonValue["bLooped"].GetBool();
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, BoxColliderComponent& boxCollider )
{
	boxCollider.width = jsonValue["width"].GetInt();
	boxCollider.height = jsonValue["height"].GetInt();
	boxCollider.offset = glm::vec2{ jsonValue["offset"]["x"].GetFloat(), jsonValue["offset"]["y"].GetFloat() };
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, CircleColliderComponent& circleCollider )
{
	circleCollider.radius = jsonValue["radius"].GetFloat();
	circleCollider.offset = glm::vec2{ jsonValue["offset"]["x"].GetFloat(), jsonValue["offset"]["y"].GetFloat() };
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, TextComponent& text )
{
	text.sTextStr = jsonValue["text"].GetString();
	text.sFontName = jsonValue["fontName"].GetString();
	text.padding = jsonValue["padding"].GetInt();
	text.wrap = jsonValue["wrap"].GetInt();
	text.bHidden = jsonValue["bHidden"].GetBool();

	text.color = Color{ .r = static_cast<GLubyte>( jsonValue["color"]["r"].GetInt() ),
		.g = static_cast<GLubyte>( jsonValue["color"]["g"].GetInt() ),
		.b = static_cast<GLubyte>( jsonValue["color"]["b"].GetInt() ),
		.a = static_cast<GLubyte>( jsonValue["color"]["a"].GetInt() ) };
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, PhysicsComponent& physics )
{
}

void ComponentSerializer::DeserializeComponent( const rapidjson::Value& jsonValue, RigidBodyComponent& rigidBody )
{
	rigidBody.velocity.x = jsonValue["velocity"]["x"].GetFloat();
	rigidBody.velocity.y = jsonValue["velocity"]["x"].GetFloat();
}

TransformComponent ComponentSerializer::DeserializeTransformComponent( const rapidjson::Value& jsonValue )
{
	return TransformComponent{
		.position = glm::vec2{ jsonValue["position"]["x"].GetFloat(), jsonValue["position"]["y"].GetFloat() },
		.scale = glm::vec2{ jsonValue["scale"]["x"].GetFloat(), jsonValue["scale"]["y"].GetFloat() },
		.rotation = jsonValue["rotation"].GetFloat() };
}

SpriteComponent ComponentSerializer::DeserializeSpriteComponent( const rapidjson::Value& jsonValue )
{
	return SpriteComponent{ .width = jsonValue["width"].GetFloat(),
		.height = jsonValue["height"].GetFloat(),
		.uvs = UVs{ .u = jsonValue["uvs"]["u"].GetFloat(),
			.v = jsonValue["uvs"]["v"].GetFloat(),
			.uv_width = jsonValue["uvs"]["uv_width"].GetFloat(),
			.uv_height = jsonValue["uvs"]["uv_height"].GetFloat() },
		.start_x = jsonValue["startX"].GetInt(),
		.start_y = jsonValue["startY"].GetInt(),
		.layer = jsonValue["layer"].GetInt(),
		.texture_name = jsonValue["sTexture"].GetString(),
		.bHidden = jsonValue["bHidden"].GetBool() };
}

AnimationComponent ComponentSerializer::DeserializeAnimationComponent( const rapidjson::Value& jsonValue )
{
	return AnimationComponent();
}

BoxColliderComponent ComponentSerializer::DeserializeBoxColliderComponent( const rapidjson::Value& jsonValue )
{
	return BoxColliderComponent();
}

CircleColliderComponent ComponentSerializer::DeserializeCircleColliderComponent( const rapidjson::Value& jsonValue )
{
	return CircleColliderComponent();
}

TextComponent ComponentSerializer::DeserializeTextComponent( const rapidjson::Value& jsonValue )
{
	return TextComponent();
}

PhysicsComponent ComponentSerializer::DeserializePhysicsComponent( const rapidjson::Value& jsonValue )
{
	return PhysicsComponent();
}

RigidBodyComponent ComponentSerializer::DeserializeRigidBodyComponent( const rapidjson::Value& jsonValue )
{
	return RigidBodyComponent();
}
