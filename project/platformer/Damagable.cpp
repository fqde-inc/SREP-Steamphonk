#include "Damagable.hpp"
#include "PlatformerGame.hpp"

using namespace std;

Damagable::Damagable(GameObject* gameObject): Component(gameObject)
{
}

void Damagable::takeDamage(int damage)
{
	curLife -= damage;
	if (curLife <= 0) {
		gameObject->setConsumed(true);
	}
}

void Damagable::setMaxLife(int damage)
{
	maxLife = damage;
}

void Damagable::setLife(int damage)
{
	curLife = damage;
}

void Damagable::addLife(int damage)
{
	curLife += damage;
}

int Damagable::getCurLife() {
    return curLife;
}
