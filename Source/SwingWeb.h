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

	// çXêVèàóù
	void Update(float elapsedTime) override;

private:
	static SwingWeb* instanceLeft;
	static SwingWeb* instanceRight;

	bool isLeftHand = true;


};

