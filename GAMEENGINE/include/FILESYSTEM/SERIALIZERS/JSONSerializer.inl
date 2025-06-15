template <typename TValue>
inline JSONSerializer& JSONSerializer::AddKeyValuePair( const std::string& key, const TValue& value )
{
	m_pWriter->Key( key.c_str() );

	if constexpr ( std::is_same_v<TValue, std::string> )
	{
		m_pWriter->String( value.c_str() );
	}
	else if constexpr ( std::is_integral_v<TValue> )
	{
		m_pWriter->Int64( value );
	}
	else if constexpr ( std::is_unsigned_v<TValue> )
	{
		m_pWriter->Uint64( value );
	}
	else if constexpr ( std::is_floating_point_v<TValue> ) // 1>C:\Aswin_Game_DEV\2DEngine2\Project1\GAMEENGINE\ENGINE\FILESYSTEM\SERIALIZERS\JSONSerializer.inl(20,21): error C2100: illegal indirection
	{
		m_pWriter->Double( value );
	}
	else
	{
		assert( false ); //"Type is not supported!"
	}

	return *this;
}

template <>
inline JSONSerializer& JSONSerializer::AddKeyValuePair( const std::string& key, const bool& value )
{
	m_pWriter->Key( key.c_str() );
	m_pWriter->Bool( value );

	return *this;
}
