#include "Crosshair.hpp"
#include "PlatformerGame.hpp"

Crosshair::Crosshair(GameObject* gameObject): Component(gameObject)
{
}

void Crosshair::update(float deltaTime)
{
	auto mouseMotion = PlatformerGame::instance->mouseMotion;
	auto playerPos = PlatformerGame::instance->getPlayerPosition();
	auto mousePx = vec2(mouseMotion.x - PlatformerGame::windowSize.x / 2.0f, PlatformerGame::windowSize.y / 2.0f - mouseMotion.y);
	auto mouseCenter = mousePx + playerPos;
	gameObject->setPosition(mouseCenter);
}
