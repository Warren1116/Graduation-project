#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"
#include "ProjectileWall.h"
#include "BrokenProjectileManager.h"

class SwingWeb : public Projectile
{
public:
	SwingWeb(ProjectileManager* manager);
	~SwingWeb() override;

	static SwingWeb& Instance() { return *instance; }

	// XVˆ—
	void Update(float elapsedTime) override;

private:
	static SwingWeb* instance;

};

