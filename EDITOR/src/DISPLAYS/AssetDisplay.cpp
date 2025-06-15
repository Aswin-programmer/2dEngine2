#include "DISPLAYS/AssetDisplay.h"

#include "SCENE/SceneManager.h"
#include "SCENE/SceneObject.h"
#include "TOOLS/TileTool.h"
#include "TOOLS/ToolManager.h"
#include "UTILITIES/EditorUtilities.h"

#include "ECS/Entity.h"
#include "ECS/MainRegistry.h"
#include "ECS/Registry.h"
#include "LOGGER/log.h"
#include "RENDERER/CORE/BatchRenderer.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/TEXTURE/Texture.h";
#include "RESOURCES/AssetManager.h"
#include "SCRIPTING/InputManager.h"
#include "UTILITIES/2dEngineUtilities.h"

constexpr float DEFAULT_ASSET_SIZE = 128.f;
constexpr ImVec2 DRAG_ASSET_SIZE = ImVec2{ 32.f, 32.f };

void AssetDisplay::SetAssetType()
{
	if ( !m_bAssetTypeChanged )
		return;

	if ( m_sSelectedType == "TEXTURES" )
	{
		m_eSelectedType = AssetType::TEXTURE;
		m_sDragSource = std::string{ DROP_TEXTURE_SRC };
	}
	else if ( m_sSelectedType == "FONTS" )
	{
		m_eSelectedType = AssetType::FONT;
		m_sDragSource = std::string{ DROP_FONT_SRC };
	}
	else if ( m_sSelectedType == "SOUNDFX" )
	{
		m_eSelectedType = AssetType::SOUNDFX;
		m_sDragSource = std::string{ DROP_SOUNDFX_SRC };
	}
	else if ( m_sSelectedType == "MUSIC" )
	{
		m_eSelectedType = AssetType::MUSIC;
		m_sDragSource = std::string{ DROP_MUSIC_SRC };
	}
	else if ( m_sSelectedType == "SCENES" )
	{
		m_eSelectedType = AssetType::SCENE;
		m_sDragSource = std::string{ DROP_SCENE_SRC };
	}
	else
	{
		m_eSelectedType = AssetType::NO_TYPE;
		m_sDragSource = "NO_ASSET_TYPE";
	}

	m_bAssetTypeChanged = false;
}

void AssetDisplay::DrawSelectedAssets()
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	std::vector<std::string> assetNames{};

	if ( m_eSelectedType == AssetType::SCENE )
	{
		assetNames = SCENE_MANAGER().GetSceneNames();
	}
	else
		assetNames = assetManager.GetAssetKeyNames( m_eSelectedType );

	if ( assetNames.empty() )
		return;

	float windowWidth = ImGui::GetWindowWidth();

	int numCols = static_cast<int>( ( windowWidth - m_AssetSize ) / m_AssetSize );
	int numRows = static_cast<int>( assetNames.size() / ( numCols <= 1 ? 1 : numCols ) + 1 );

	if ( !numCols || !numRows )
		return;

	ImGuiTableFlags tableFlags{ 0 };
	tableFlags |= ImGuiTableFlags_SizingFixedFit;

	int k{ 0 }, id{ 0 };

	auto assetItr = assetNames.begin();

	if ( ImGui::BeginTable( "Assets", numCols, tableFlags ) )
	{
		for ( int row = 0; row < numRows; row++ )
		{
			ImGui::TableNextRow();
			for ( int col = 0; col < numCols; col++ )
			{
				if ( assetItr == assetNames.end() )
					break;

				ImGui::PushID( k++ );
				ImGui::TableSetColumnIndex( col );

				bool bSelectedAsset{ m_SelectedID == id };

				if ( bSelectedAsset )
					ImGui::TableSetBgColor( ImGuiTableBgTarget_CellBg,
						ImGui::GetColorU32( ImVec4{ 0.f, 0.9f, 0.f, 0.3f } ) );

				GLuint textureID{ GetTextureID( *assetItr ) };
				std::string sCheckName{ m_sRenameBuf.data() };

				if ( textureID == 0 )
					break;

				ImGui::ImageButton( "##AssetTexture", (ImTextureID)(intptr_t)textureID, ImVec2{ m_AssetSize, m_AssetSize } );

				if ( ImGui::IsItemHovered() && ImGui::IsMouseClicked( 0 ) && !m_bRename )
					m_SelectedID = id;

				auto sAssetName = ( *assetItr ).c_str();

				/*ImGui::BeginPopupContextItem() :
					1.Opens a context popup menu when the user right - clicks on the item.
					2.Returns true if the popup is open and you can draw its content.
					3.The default ID is based on the last ItemID(e.g., a button or selectable).
					4.Usage: Good for showing per - item right - click menus(e.g., rename, delete).*/

				if ( bSelectedAsset && ImGui::BeginPopupContextItem() )
				{
					OpenAssetContext( *assetItr );

					ImGui::EndPopup();
				}

				if ( ImGui::BeginDragDropSource() ) // Begins a drag-and-drop source. Returns true if the user has started dragging the item (e.g., left mouse button held on a widget).
				{
					/*SetDragDropPayload()
						Sets the data to send when the user drops this item
						Parameters:
						1.m_sDragSource.c_str() � the payload type as a string(receiver must match this)
						2.sAssetName � the actual data being dragged(e.g., file name)
						3.Payload size is strlen(sAssetName) + 1 to include the null terminator, multiplied by sizeof(char)
						4.ImGuiCond_Once ensures the payload is only set once per drag.*/
					ImGui::SetDragDropPayload( m_sDragSource.c_str(),
						sAssetName,
						( strlen( sAssetName ) + 1 ) * sizeof( char ),
						ImGuiCond_Once );
					/*ImGui::Image() :
						1.Displays an image during the drag operation, acting as a visual representation of the dragged asset
						2.textureID is cast to ImTextureID � typically an OpenGL or DirectX texture handle
						3.DRAG_ASSET_SIZE is likely a ImVec2(e.g., { 64, 64 }) specifying the image size.

						EndDragDropSource() ends the drag source block.*/
					ImGui::Image( (ImTextureID)textureID, DRAG_ASSET_SIZE );
					ImGui::EndDragDropSource();
				}

				if ( !m_bRename || !bSelectedAsset )
					ImGui::Text( sAssetName ); // Renders a text.

				if ( m_bRename && bSelectedAsset )
				{
					ImGui::SetKeyboardFocusHere();
					if ( ImGui::InputText(
							 "##rename", m_sRenameBuf.data(), 255, ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						if ( !DoRenameAsset( *assetItr, sCheckName ) )
						{
							LOG_ERROR( "Failed to change asset name." );
						}

						m_sRenameBuf.clear();
						m_bRename = false;
					}
					else if ( m_bRename && ImGui::IsKeyPressed( ImGuiKey_Escape ) )
					{
						m_sRenameBuf.clear();
						m_bRename = false;
					}
				}

				if ( !m_bRename && bSelectedAsset && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
				{
					m_sRenameBuf.clear();
					m_sRenameBuf = *assetItr;
					m_bRename = true;
				}

				if ( m_bRename && bSelectedAsset )
				{
					if ( sAssetName != sCheckName )
						CheckRename( sCheckName );
				}

				++id;
				++assetItr;
				ImGui::PopID();
			}
		}

		ImGui::EndTable();
	}
}

unsigned int AssetDisplay::GetTextureID( const std::string& sAssetName ) const
{
	auto& assetManager = MAIN_REGISTRY().GetAssetManager();
	switch ( m_eSelectedType )
	{
		case AssetType::TEXTURE:
		{
			auto& pTexture = assetManager.GetTexture( sAssetName );
			return pTexture.getID();

			break;
		}
		case AssetType::FONT:
		{
			auto pFont = assetManager.GetFont( sAssetName );
			if ( pFont )
				return pFont->GetFontAtlasID();

			break;
		}
		case AssetType::SOUNDFX:
		case AssetType::MUSIC:
		{
			auto& pTexture = assetManager.GetTexture( "music_icon" );
			return pTexture.getID();

			break;
		}
		case AssetType::SCENE:
		{
			auto& pTexture = assetManager.GetTexture( "scene_icon" );
			return pTexture.getID();

			break;
		}
	}

	return 0;
}

bool AssetDisplay::DoRenameAsset( const std::string& sOldName, const std::string& sNewName ) const
{
	if ( sNewName.empty() )
		return false;

	if ( m_eSelectedType == AssetType::SCENE )
	{
		// TODO: Change the scene name
	}
	else
	{
		auto& assetManager = MAIN_REGISTRY().GetAssetManager();
		return assetManager.ChangeAssetName( sOldName, sNewName, m_eSelectedType );
	}

	assert( false && "How did it get here??" );
	return false;
}

void AssetDisplay::CheckRename( const std::string& sCheckName ) const
{
	if ( sCheckName.empty() )
	{
		ImGui::TextColored( ImVec4{ 1.f, 0.f, 0.f, 1.f }, "Rename text cannot be blank!" );
		return;
	}

	bool bHasAsset{ false };

	if ( m_eSelectedType == AssetType::SCENE )
	{
		// TODO: Check is scene name already exists
	}
	else
	{
		auto& assetManager = MAIN_REGISTRY().GetAssetManager();
		if ( assetManager.CheckHasAsset( sCheckName, m_eSelectedType ) )
			bHasAsset = true;
	}

	if ( bHasAsset )
		ImGui::TextColored( ImVec4{ 1.f, 0.f, 0.f, 1.f },
			std::format( "Asset name [{}] already exists!", sCheckName ).c_str() );
}

void AssetDisplay::OpenAssetContext( const std::string& sAssetName )
{
	/*ImGui::Selectable() creates a selectable text item that
						1.Looks like a clickable row or menu item
						2.Can be clicked to trigger an action
						3.Can be visually highlighted if selected
						4.Can be used in lists, menus, popups, etc.*/

	if ( ImGui::Selectable( "rename" ) ) // Right click to see the option.
	{
		m_bRename = true;
	}
	if ( ImGui::Selectable( "delete" ) ) // Right click to see the option.
	{
		if ( m_eSelectedType == AssetType::SCENE )
		{
			// TODO: Check is scene name already exists
		}
		else
		{
			auto& assetManager = MAIN_REGISTRY().GetAssetManager();
			if ( !assetManager.DeleteAsset( sAssetName, m_eSelectedType ) )
			{
				LOG_ERROR( "Failed to delete the asset {}.", sAssetName );
			}
		}
	}
}

AssetDisplay::AssetDisplay()
	: m_bAssetTypeChanged{ true }, m_bRename{ false }, m_bWindowSelected{ false }, m_bWindowHovered{ false }, m_sSelectedAssetName{ "" }, m_sSelectedType{ "TEXTURES" }, m_sDragSource{ "" }, m_sRenameBuf{ "" }, m_eSelectedType{ AssetType::TEXTURE }, m_AssetSize{ DEFAULT_ASSET_SIZE }, m_SelectedID{ -1 }
{
}

void AssetDisplay::Draw()
{
	if ( !ImGui::Begin( "Assets" ) )
	{
		ImGui::End();
		return;
	}

	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	ImGui::Text( "Asset Type" );
	ImGui::SameLine( 0.f, 10.f );
	if ( ImGui::BeginCombo( "##AssetType", m_sSelectedType.c_str() ) )
	{
		for ( const auto& sAssetType : m_SelectableTypes )
		{
			bool bIsSelected = m_sSelectedType == sAssetType;
			if ( ImGui::Selectable( sAssetType.c_str(), bIsSelected ) )
			{
				m_bAssetTypeChanged = true;
				m_sSelectedType = sAssetType;
				m_SelectedID = -1;
				SetAssetType();
			}

			if ( bIsSelected )
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if ( ImGui::BeginChild( "##AssetTable",
			 ImVec2{ 0.f, 0.f },
			 NULL,
			 ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_ChildWindow ) )
	{
		m_bWindowHovered = ImGui::IsWindowHovered();
		m_bWindowSelected = ImGui::IsWindowFocused();

		DrawSelectedAssets();

		ImGui::EndChild();
	}

	ImGui::End();
}

void AssetDisplay::Update()
{
}
