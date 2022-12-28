
#include "Explosion.hpp"
#include "PlatformerGame.hpp"

using namespace std;

Explosion::Explosion(GameObject* gameObject): Component(gameObject)
{
	gameObject->name = "Explosion";

    timer = gameObject->addComponent<TimerComponent>();

    timer->initTimer(duration);
	
	// Particles ?
    spriteComponent = gameObject->addComponent<SpriteComponent>();

    animationSprites = {PlatformerGame::instance->explosionAtlas->get("Circle_explosion4.png"),
                        PlatformerGame::instance->explosionAtlas->get("Circle_explosion5.png"),
                        PlatformerGame::instance->explosionAtlas->get("Circle_explosion6.png"),
                        PlatformerGame::instance->explosionAtlas->get("Circle_explosion7.png"),
    };

    auto physicsScale = PlatformerGame::instance->physicsScale;

    radius = 50/physicsScale;
	physics =  gameObject->addComponent<PhysicsComponent>();
    physics->initCircle(b2_kinematicBody, radius, gameObject->getPosition()/physicsScale, 0);
    physics->setAutoUpdate(true);
    physics->setSensor(true);

    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::MISSILE;
    filter.maskBits     = PlatformerGame::MISSILE | PlatformerGame::ENEMY | PlatformerGame::PLAYER;
    physics->getFixture()->SetFilterData(filter);
}

void Explosion::update(float deltaTime)
{
	if(timer->hasFinished){
		gameObject->setConsumed(true);
		return;
	}

    animationTime += deltaTime;

    if(animationTime >= animationFrameRate) {
        animationIndex = (animationIndex + 1) % animationSprites.size();
        animationTime = 0;
    }

    spriteComponent->setSprite(animationSprites[animationIndex]);
}

// raycast callback
float32 Explosion::ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
									const b2Vec2& normal, float32 fraction) {
	return 0;
};

void Explosion::onCollisionStart(PhysicsComponent *comp){

    auto go = comp->getGameObject();
    
    auto damageable = go->getComponent<Damagable>();
    if(damageable != nullptr && go->name != "Player"){
        damageable->takeDamage(damage);
    }

    if(go->name == "Player"){
        // Get propulsion direction
        glm::vec2 propel = gameObject->getPosition() - go->getPosition();
        comp->addImpulse( - glm::normalize(propel)  * 0.15f );
        physics->getBody()->SetAwake(false);
    }

};

void Explosion::onCollisionEnd(PhysicsComponent *comp) {
	
};