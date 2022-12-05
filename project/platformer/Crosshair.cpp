#include "Crosshair.hpp"
#include "PlatformerGame.hpp"
using namespace std;

Crosshair::Crosshair(GameObject* gameObject): Component(gameObject)
{
}

void Crosshair::update(float deltaTime)
{
	auto mouseMotion = PlatformerGame::instance->mouseMotion;
	auto camPos = PlatformerGame::instance->camera->offset;

	gameObject->setPosition(glm::vec2(
		mouseMotion.x * sensitivity - PlatformerGame::windowSize.x/2 + camPos.x, 
		PlatformerGame::windowSize.y - mouseMotion.y * sensitivity - PlatformerGame::windowSize.y/2 + camPos.x
	));
}
