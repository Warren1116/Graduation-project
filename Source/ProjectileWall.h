#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Audio/Audio.h"

// 壁にぶつかった弾丸（仮）
class ProjectileWall : public Projectile
{
public:
	ProjectileWall(BrokenProjectileManager* manager);
	~ProjectileWall() override;

	// 更新処理
	void Update(float elapsedTime) override;

private:
	float lifeTimer = 3.0f;

private:

	struct decal_texture
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> color_shader_resource_view;
	};
	struct decal_data
	{
		DirectX::XMFLOAT3 translation{ 0,0,0 };
		DirectX::XMFLOAT3 scaling{ 1,1,1 };
		DirectX::XMFLOAT3 rotation{ 0,0,0 };
		DirectX::XMFLOAT4 color{ 1,1,1,1 };
		int decal_index = -1;
	};

};

