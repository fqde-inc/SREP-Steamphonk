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

void Damagable::overrideDamageSound(std::function<void()> action) {
    hasDamageSound = true;
    damageSoundAction = action;
}

void Damagable::takeDamage(int damage)
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

void Damagable::setMaxLife(int life)
{
	cout << "Setting max life from " << maxLife << " to: " << life << endl;
	maxLife = life;
}

void Damagable::setLife(int life)
{
	curLife = life;
}

void Damagable::addLife(int life)
{
	curLife += life;
}

void Damagable::resetLife()
{
	cout << "Resetting life from " << curLife << " to: " << maxLife << endl;
	curLife = maxLife;
}

int Damagable::getCurLife() {
    return curLife;
}

int Damagable::getMaxLife() {
    return maxLife;
}
