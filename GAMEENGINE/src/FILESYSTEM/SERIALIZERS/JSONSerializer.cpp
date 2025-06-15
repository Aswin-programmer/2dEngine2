#include "pch.h"

#include "FILESYSTEM/SERIALIZERS/JSONSerializer.h"

#include "LOGGER/log.h"

constexpr int MAX_DECIMAL_PLACES = 3;

JSONSerializer::JSONSerializer( const std::string& sFileName )
	: m_Filestream{},
	  m_StringBuffer{},
	  m_pWriter{ std::make_unique<rapidjson::PrettyWriter<rapidjson::StringBuffer>>( m_StringBuffer ) },
	  m_NumObjectStarted{ 0 },
	  m_NumArrayStarted{ 0 }
{
	m_Filestream.open( sFileName, std::ios::out | std::ios::trunc );
	assert( m_Filestream.is_open() && "Failed to open file!" );

	if ( !m_Filestream.is_open() )
	{
		throw std::runtime_error( std::format( "JSONSerializer failed to open file [{}]", sFileName ) );
	}

	m_pWriter->SetMaxDecimalPlaces( MAX_DECIMAL_PLACES );
}

JSONSerializer::~JSONSerializer()
{
	if ( m_Filestream.is_open() )
	{
		m_Filestream.close();
	}
}

bool JSONSerializer::StartDocument()
{
	assert( m_NumObjectStarted == 0 && "Document has already been started. Please Reset the serializer." );

	if ( m_NumObjectStarted != 0 )
	{
		LOG_ERROR( "Document has already been started. Please Reset the serializer." );
		return false;
	}
	++m_NumObjectStarted;

	return m_pWriter->StartObject();
}

bool JSONSerializer::EndDocument()
{
	assert( m_NumObjectStarted == 1 && "There should be only be one object opened." );
	assert( m_NumArrayStarted == 0 && "Too many arrays started. Did you forget to call EndArray()?." );

	if ( m_NumObjectStarted != 1 )
	{
		LOG_ERROR( "Failed to end document correctly. There should only be one object open. Did you forget to call EndArray()?." );
		return false;
	}

	if ( m_NumArrayStarted != 0 )
	{
		LOG_ERROR( "Failed to end document correctly. Too many arrays started. Did you forget to call EndArray()?." );
		return false;
	}

	m_pWriter->EndObject();
	--m_NumObjectStarted;
	m_Filestream << m_StringBuffer.GetString();
	m_Filestream.flush();

	return false;
}

bool JSONSerializer::Reset( const std::string& sFileName )
{
	assert( m_NumObjectStarted == 0 && "Cannot Reset, Document has not been finished." );
	assert( m_NumArrayStarted == 0 && "Cannot Reset, Too many arrays open" );

	if ( m_NumObjectStarted != 0 )
	{
		LOG_ERROR( "Cannot Reset, Document has not been finished." );
		return false;
	}

	if ( m_NumArrayStarted != 0 )
	{
		LOG_ERROR( "Cannot Reset, Too many arrays open" );
		return false;
	}

	if ( m_Filestream.is_open() )
	{
		m_Filestream.close();
	}

	m_Filestream.open( sFileName, std::ios::out | std::ios::trunc );
	assert( m_Filestream.is_open() && "Failed to open file!" );

	if ( !m_Filestream.is_open() )
	{
		throw std::runtime_error( std::format( "JSONSerializer failed to open file [{}]", sFileName ) );
	}

	return true;
}

JSONSerializer& JSONSerializer::StartNewObject( const std::string& key )
{
	++m_NumObjectStarted;
	if ( !key.empty() )
	{
		m_pWriter->Key( key.c_str() );
	}

	m_pWriter->StartObject();
	return *this;
}

JSONSerializer& JSONSerializer::EndNewObject()
{
	assert( m_NumObjectStarted > 1 && "EndObject called too many times!" );
	--m_NumObjectStarted;
	m_pWriter->EndObject();
	return *this;
}

JSONSerializer& JSONSerializer::StartNewArray( const std::string& key )
{
	++m_NumArrayStarted;

	m_pWriter->Key( key.c_str() );

	m_pWriter->StartArray();
	return *this;
}

JSONSerializer& JSONSerializer::EndNewArray()
{
	assert( m_NumArrayStarted > 0 && "EndArray() called too many times!" );
	--m_NumArrayStarted;
	m_pWriter->EndArray();
	return *this;
}
