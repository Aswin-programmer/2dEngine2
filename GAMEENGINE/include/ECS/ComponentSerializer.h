#pragma once

#include "COMPONENTS/AllComponents.h"

class JSONSerializer;

#define SERIALIZE_COMPONENT( serializer, component ) ComponentSerializer::Serialize( serializer, component )
#define DESERIALIZE_COMPONENT( serializer, compref ) ComponentSerializer::Deserialize( serializer, compref )
#define DESERIALIZE_COMPONENT_DEPRICARTED( COMP, serializer ) ComponentSerializer::Deserialize<COMP>( serializer )

class ComponentSerializer
{
public:
	ComponentSerializer() = delete;

	template <typename TComponent, typename TSerializer>
	static void Serialize( TSerializer& serializer, TComponent& component );

	template <typename TComponent, typename TTable>
	static void Deserialize( const TTable& table, TComponent& component );

	template <typename TComponent, typename TTable>
	static auto Deserialize( const TTable& table );

private:
	// JSON Serializer

	static void SerializeComponent( JSONSerializer& serializer, TransformComponent& transform );
	static void SerializeComponent( JSONSerializer& serializer, SpriteComponent& sprite );
	static void SerializeComponent( JSONSerializer& serializer, AnimationComponent& animation );
	static void SerializeComponent( JSONSerializer& serializer, BoxColliderComponent& boxcollider );
	static void SerializeComponent( JSONSerializer& serializer, CircleColliderComponent& circlecollider );
	static void SerializeComponent( JSONSerializer& serializer, TextComponent& text );
	static void SerializeComponent( JSONSerializer& serializer, PhysicsComponent& physics );
	static void SerializeComponent( JSONSerializer& serializer, RigidBodyComponent& rigidbody );

	static void DeserializeComponent( const rapidjson::Value& jsonValue, TransformComponent& transform );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, SpriteComponent& sprite );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, AnimationComponent& animation );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, BoxColliderComponent& boxCollider );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, CircleColliderComponent& circleCollider );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, TextComponent& text );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, PhysicsComponent& physics );
	static void DeserializeComponent( const rapidjson::Value& jsonValue, RigidBodyComponent& rigidBody );

	// The below functions are depreciated.

	static TransformComponent DeserializeTransformComponent( const rapidjson::Value& jsonValue );
	static SpriteComponent DeserializeSpriteComponent( const rapidjson::Value& jsonValue );
	static AnimationComponent DeserializeAnimationComponent( const rapidjson::Value& jsonValue );
	static BoxColliderComponent DeserializeBoxColliderComponent( const rapidjson::Value& jsonValue );
	static CircleColliderComponent DeserializeCircleColliderComponent( const rapidjson::Value& jsonValue );
	static TextComponent DeserializeTextComponent( const rapidjson::Value& jsonValue );
	static PhysicsComponent DeserializePhysicsComponent( const rapidjson::Value& jsonValue );
	static RigidBodyComponent DeserializeRigidBodyComponent( const rapidjson::Value& jsonValue );
};

#include "ComponentSerializer.inl"
