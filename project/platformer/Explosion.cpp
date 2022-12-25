
#include "Explosion.hpp"
#include "PlatformerGame.hpp"

using namespace std;

Explosion::Explosion(GameObject* gameObject): Component(gameObject)
{
	gameObject->name = "Explosion";

    timer = gameObject->addComponent<TimerComponent>();

    timer->initTimer(duration);
	
	// Particles ?
    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
	sprite.setScale(glm::vec2{5.f});
    spriteComponent = gameObject->addComponent<SpriteComponent>();
	spriteComponent->setSprite(sprite);

    auto physicsScale = PlatformerGame::instance->physicsScale;

    radius = 50/physicsScale;
	physics =  gameObject->addComponent<PhysicsComponent>();
    physics->initCircle(b2_kinematicBody, radius, gameObject->getPosition()/physicsScale, 0);
    physics->setAutoUpdate(true);
    physics->setSensor(true);
}

void Explosion::update(float deltaTime)
{
	if(timer->isRunning) {
		if(physics->getBody()->IsAwake());
			physics->getBody()->SetAwake(false);
			
		auto sprite = spriteComponent->getSprite();
		sprite.setColor(glm::vec4{ 1, 1, 1, sprite.getColor().a - 10.0f });
		return;
	}
	
	gameObject->setConsumed(true);
}


        // raycast callback
float32 Explosion::ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
									const b2Vec2& normal, float32 fraction) {
	return 0;
};

void Explosion::onCollisionStart(PhysicsComponent *comp){
	comp->addImpulse(glm::vec2{0.05f});
};

void Explosion::onCollisionEnd(PhysicsComponent *comp) {
	
};