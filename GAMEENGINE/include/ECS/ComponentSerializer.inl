template <typename TComponent, typename TSerializer>
inline void ComponentSerializer::Serialize( TSerializer& serializer, TComponent& component )
{
	SerializeComponent( serializer, component );
}

template <typename TComponent, typename TTable>
inline void ComponentSerializer::Deserialize( const TTable& table, TComponent& component )
{
	DeserializeComponent( table, component );
}

template <typename TComponent, typename TTable>
inline auto ComponentSerializer::Deserialize( const TTable& table )
{
	if constexpr ( std::is_same_v<TComponent, TransformComponent> )
	{
		DeserializeTransformComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, SpriteComponent> )
	{
		DeserializeSpriteComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, AnimationComponent> )
	{
		DeserializeAnimationComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, BoxColliderComponent> )
	{
		DeserializeBoxColliderComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, CircleColliderComponent> )
	{
		DeserializeCircleColliderComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, TextComponent> )
	{
		DeserializeTextComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, PhysicsComponent> )
	{
		DeserializePhysicsComponent( table );
	}
	else if constexpr ( std::is_same_v<TComponent, RigidBodyComponent> )
	{
		DeserializeRigidBodyComponent( table );
	}
	else
	{
		// assert(false, "Component type is invalid!.");
	}
}
