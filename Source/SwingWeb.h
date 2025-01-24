#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"
#include "ProjectileWall.h"
#include "BrokenProjectileManager.h"

class SwingWeb : public Projectile
{
public:
	static SwingWeb& InstanceLeft() { return *instanceLeft; }
	static SwingWeb& InstanceRight() { return *instanceRight; }


	SwingWeb(ProjectileManager* manager, bool leftHand);
	~SwingWeb() override;

	static SwingWeb& Instance() { return *instance; }

	// çXêVèàóù
	void Update(float elapsedTime) override;

private:
	static SwingWeb* instanceLeft;
	static SwingWeb* instanceRight;
	static SwingWeb* instance;

	bool isLeftHand = true;


};

