#include "Damagable.hpp"
#include "PlatformerGame.hpp"

using namespace std;

Damagable::Damagable(GameObject* gameObject): Component(gameObject)
{
}

void Damagable::overrideDeathAction(std::function<void()> action) {
	hasDeathAction = true;
	deathAction = action;
}

void Damagable::takeDamage(int damage)
{
	curLife -= damage;
	if (curLife <= 0) {
		if (hasDeathAction)
		{
			(this->deathAction)();
			return;
		}
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

void Damagable::resetLife()
{
	curLife = maxLife;
}