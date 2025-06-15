#pragma once

class Registry;

class TilemapLoader
{
private:
	bool SaveTilemapJSON( Registry& registry, const std::string& sTilemapFile );
	bool LoadTilemapJSON( Registry& registry, const std::string& sTilemapFile );

	// TODO: Save and load functions for lua
public:
	TilemapLoader() = default;
	~TilemapLoader() = default;

	bool SaveTilemap( Registry& registry, const std::string& sTilemapFile, bool bUseJSON = false );
	bool LoadTilemap( Registry& registry, const std::string& sTilemapFile, bool bUseJSON = false );
};
