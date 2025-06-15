#pragma once

class JSONSerializer
{
private:
	std::fstream m_Filestream;
	rapidjson::StringBuffer m_StringBuffer;
	std::unique_ptr<rapidjson::PrettyWriter<rapidjson::StringBuffer>> m_pWriter;
	int m_NumObjectStarted, m_NumArrayStarted;

public:
	JSONSerializer( const std::string& sFileName );
	~JSONSerializer();

	bool StartDocument();
	bool EndDocument();

	bool Reset( const std::string& sFileName );

	JSONSerializer& StartNewObject( const std::string& key = "" );
	JSONSerializer& EndNewObject();

	JSONSerializer& StartNewArray( const std::string& key );
	JSONSerializer& EndNewArray();

	template <typename TValue>
	JSONSerializer& AddKeyValuePair( const std::string& key, const TValue& value );

	template <>
	JSONSerializer& AddKeyValuePair( const std::string& key, const bool& value );
};

#include "JSONSerializer.inl"
