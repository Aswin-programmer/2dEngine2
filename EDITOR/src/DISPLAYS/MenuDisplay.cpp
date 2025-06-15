#include "DISPLAYS/MenuDisplay.h"

#include "SCENE/SceneManager.h"
#include "SCENE/SceneObject.h"

#include "FILESYSTEM/DIALOGS/FileDialog.h"
#include "LOADERS/TilemapLoader.h"
#include "LOGGER/log.h"

void MenuDisplay::Draw()
{
	if ( ImGui::BeginMainMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem( "New", "Ctrl + N" ) )
			{
				LOG_ERROR( "NEW PRESSED" );
			}

			if ( ImGui::MenuItem( "Open", "Ctrl + O" ) )
			{
				std::string basePath = std::filesystem::current_path().string();

				FileDialog fd{};
				auto file = fd.OpenFileDialog( "Open tests", basePath, { "*.png", ".jpg" } );

				if ( !file.empty() )
				{
					LOG_INFO( "FILE OPENED : {}", file );
				}
			}

			if ( ImGui::MenuItem( "Save", "Ctrl + S" ) )
			{
				std::string basePath = std::filesystem::current_path().string();

				FileDialog fd{};
				auto file = fd.SaveFileDialog( "Save Tilemap Test", basePath, { "*.json" } );
				if ( !file.empty() )
				{
					auto pCurrentScene = SCENE_MANAGER().GetCurrentScene();
					if ( pCurrentScene )
					{
						TilemapLoader tl{};
						if ( !tl.SaveTilemap( pCurrentScene->GetRegistry(), file, true ) )
						{
							LOG_ERROR( "Failed to save the tilemap!." );
						}
					}
					else
					{
						LOG_ERROR( "Failed to save tilemap. No active scene." );
						return;
					}
				}
			}

			if ( ImGui::MenuItem( "Exit" ) )
			{
				LOG_ERROR( "SHOULD EVENTUALLY EXIT!" );
			}

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Edit" ) )
		{
			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Tools" ) )
		{
			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Settings" ) )
		{
			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Help" ) )
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuDisplay::Update()
{
}
