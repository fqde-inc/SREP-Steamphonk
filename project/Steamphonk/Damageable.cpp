#include "Damageable.hpp"
#include "PlatformerGame.hpp"

using namespace std;

Damageable::Damageable(GameObject* gameObject): Component(gameObject)
{
}

void Damageable::overrideDeathAction(std::function<void()> action) {
	hasDeathAction = true;
	deathAction = action;
}

void Damageable::overrideDamageSound(std::function<void()> action) {
    hasDamageSound = true;
    damageSoundAction = action;
}

void Damageable::takeDamage(int damage)
{
	curLife -= damage;

    if(hasDamageSound)
        (this->damageSoundAction)();

	if (curLife <= 0) {
		if (hasDeathAction)
		{
			(this->deathAction)();
			return;
		}
		gameObject->setConsumed(true);
	}
}

void Damageable::setMaxLife(int life)
{
	cout << "Setting max life from " << maxLife << " to: " << life << endl;
	maxLife = life;
}

void Damageable::setLife(int life)
{
	curLife = life;
}

void Damageable::addLife(int life)
{
	curLife += life;
}

void Damageable::resetLife()
{
	cout << "Resetting life from " << curLife << " to: " << maxLife << endl;
	curLife = maxLife;
}

int Damageable::getCurLife() {
    return curLife;
}

int Damageable::getMaxLife() {
    return maxLife;
}
