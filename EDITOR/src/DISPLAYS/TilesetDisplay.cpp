#include "DISPLAYS/TilesetDisplay.h"

#include "SCENE/SceneManager.h"
#include "TOOLS/TileTool.h"
#include "TOOLS/ToolManager.h"

#include "ECS/MainRegistry.h"
#include "RENDERER/TEXTURE/Texture.h";
#include "RESOURCES/AssetManager.h"

void TilesetDisplay::Draw()
{
	if ( !ImGui::Begin( "Tileset" ) )
	{
		ImGui::End();
		return;
	}

	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	// Creates a dropdown combo box labeled "Choose Tileset", with the currently selected tileset m_sTileset as the preview text.
	if ( ImGui::BeginCombo( "Choose Tileset", m_sTileset.c_str() ) )
	{
		for ( const auto& sTileset : assetManager.GetTilesetNames() )
		{
			bool bIsSelected = m_sTileset == sTileset;
			if ( ImGui::Selectable( sTileset.c_str(), bIsSelected ) )
			{
				m_sTileset = sTileset;
				SCENE_MANAGER().SetTileSet( sTileset );
			}

			if ( bIsSelected )
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	if ( m_sTileset.empty() )
	{
		ImGui::End();
		return;
	}

	auto& pTexture = assetManager.GetTexture( m_sTileset );

	int textureWidth = pTexture.getWidth();
	int textureHeight = pTexture.getHeight();

	int cols = textureWidth / 16;
	int rows = textureHeight / 16;

	float uv_w = 16 / static_cast<float>( textureWidth );
	float uv_h = 16 / static_cast<float>( textureHeight );

	float ux{ 0.f }, uy{ 0.f }, vx{ uv_w }, vy{ uv_h };

	ImGuiTableFlags tableFlags{ 0 };
	tableFlags |= ImGuiTableFlags_SizingFixedFit; // Columns take up just enough space to fit content tightly.
	tableFlags |= ImGuiTableFlags_ScrollX;		  // Adds horizontal scrolling if content is wider than the table.

	int k{ 0 }, id{ 0 };

	if ( ImGui::BeginTable( "Tileset", cols, tableFlags ) )
	{
		for ( int i = 0; i < rows; i++ )
		{
			ImGui::TableNextRow();
			for ( int j = 0; j < cols; j++ )
			{
				ImGui::TableSetColumnIndex( j );

				if ( m_Selected == id )
					ImGui::TableSetBgColor( ImGuiTableBgTarget_CellBg,
						ImGui::GetColorU32( ImVec4{ 0.f, 0.9f, 0.f, 0.3f } ) );

				// Create unique id for the buttons
				// The main reason begind using the PushID and PopID is to distinguish between the button which have the same labelID("##Tile")
				ImGui::PushID( k++ );

				if ( ImGui::ImageButton( "##tile", (ImTextureID)(intptr_t)pTexture.getID(), ImVec2{
																								16.f * 1.5,
																								16.f * 1.5,
																							},
						 ImVec2{ ux, uy }, ImVec2{ vx, vy } ) )
				{
					m_Selected = id;
					// LOG_INFO("StartX: {}, StartY: {}", j, i);
					auto pActiveTool = SCENE_MANAGER().GetToolManager().GetActiveTool();
					if ( pActiveTool )
					{
						pActiveTool->SetSpriteUVs( j, i );
					}
				}

				ImGui::PopID();

				// Advance the UVs to the next column
				ux += uv_w;
				vx += uv_w;
				++id;
			}
			// Put the UVs back to the start column of the next row
			ux = 0.f;
			vx = uv_w;
			uy += uv_h;
			vy += uv_h;
		}
		ImGui::EndTable();
	}

	ImGui::End();
}
