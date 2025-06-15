#pragma once

class Registry;
class Camera2D;
class SpriteBatchRenderer;

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem() = default;
	/*
	 * @brief Loops through all of the entities in the registry that have a sprite
	 * and transform component. Applies all the necessary transformations and adds them
	 * to a Batch to be rendered.
	 */
	void Update( Registry& registry, Camera2D& camera );

private:
	std::unique_ptr<SpriteBatchRenderer> m_pBatchRenderer;
};
