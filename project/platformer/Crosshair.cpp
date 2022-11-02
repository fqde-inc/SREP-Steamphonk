#include "Crosshair.hpp"
#include "PlatformerGame.hpp"

Crosshair::Crosshair(GameObject* gameObject): Component(gameObject)
{
}

void Crosshair::update(float deltaTime)
{
	auto mouseMotion = PlatformerGame::instance->mouseMotion;
	glm::vec2 camCoords = PlatformerGame::instance->camera->pixelToCameraSpace(glm::vec2((int)mouseMotion.x, PlatformerGame::windowSize.y - (int)mouseMotion.y ));

	gameObject->setPosition(camCoords);
}
