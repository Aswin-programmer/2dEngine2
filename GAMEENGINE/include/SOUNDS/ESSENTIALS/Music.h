#pragma once

#include "SoundParams.h"

class Music
{
private:
	SoundParams m_Params{};
	std::unique_ptr<ma_decoder> m_pDecoder;
	bool m_IsLoaded{ false };

public:
	Music( const SoundParams& params );
	~Music();

	bool Load();

	inline const std::string& GetName() const { return m_Params.name; }
	inline const std::string& GetFilename() const { return m_Params.filename; }
	inline const std::string& GetDescription() const { return m_Params.description; }
	inline const double GetDuration() const { return m_Params.duration; }
	inline ma_decoder* GetDecoder() { return m_pDecoder.get(); }
	inline bool IsLoaded() const { return m_IsLoaded; }
};
