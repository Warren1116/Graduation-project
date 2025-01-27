#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Telegram.h"

// 前方宣言
class EnemyManager;

// エネミー
class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() override {}
	
	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	virtual void DrawDebugGUI() {}

	// 攻撃フラグ取得
	virtual bool	GetAttackFlg() { return attackFlg; }
	// 攻撃フラグセット
	virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };
	virtual void	SetId(int id) { this->id = id; }
	virtual int		GetId() { return id; }
	// 攻撃範囲取得
	virtual float GetAttackRange() { return attackRange; }

	// メッセージ受信したときの処理
	virtual bool OnMessage(const Telegram& msg);

    // モデル取得
	Model* GetModel() const { return model; }

protected:
	Model* model = nullptr;
	bool		camerashake = false;
	EnemyManager* manager = nullptr;

	int	state = 0;
	bool attackFlg = false;
	int id = 0;
	float searchRange = 0.0f;
	float attackRange = 0.0f;

};

