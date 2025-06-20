#pragma once

#include <DirectXMath.h>
#include "Mathf.h"
#include <vector>

class Character
{
public:
	Character() {};
	virtual ~Character() {};
	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// デバッグ用GUI描画
	virtual	void DrawDebugGUI() {}

	// デバッグプリミティブ描画
	virtual	void DrawDebugPrimitive() {}

	// 行列更新処理
	void UpdateTransform();

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	void SetTrianglePosition(const DirectX::XMFLOAT3& position, int index) { this->position = position; }

	const DirectX::XMFLOAT3& GetRight() const { return { transform._11, transform._12, transform._13 }; }
	const DirectX::XMFLOAT3& GetUp() const { return { transform._21, transform._22, transform._23 }; }
	const DirectX::XMFLOAT3& GetFront() const { return { transform._31, transform._32, transform._33 }; }
    void SetFront(const DirectX::XMFLOAT3& front) { transform._31 = front.x; transform._32 = front.y; transform._33 = front.z; }

	// 位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	// 回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	// スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// 半径取得
	float GetRadius() const { return radius; }

	// 高さ取得
	float GetHeight() const { return height; }

	DirectX::XMFLOAT3 GetVelocity() const { return velocity; }

	// 地面に接地しているか
	bool IsGround() const { return isGround; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// 健康状態を取得
	int GetHealth() const { return health; }

	// 最大健康状態を取得
	int GetMaxHealth() const { return maxHealth; }

	// 生死判定フラグ
	bool GetAlive() { return isAlive; }

	// スイングしてるフラグ
	bool GetOnSwing() { return onSwing; }
    bool SetOnSwing(bool onSwing) { this->onSwing = onSwing; }

    // プレイヤー壁登りフラグ
    bool GetOnClimb() { return onClimb; }

    bool GetHitWall() { return hitWall; }

	//ロックオンされているかどうか
	bool GetIsLockedOn() const { return IsLockedOn; }
	void SetLockedOn(bool lockStatus) { IsLockedOn = lockStatus; }



private:

        // roll axis rotation
        void Roll(float elapsedTime, float direction, float speed);
	// 垂直速力更新処理　
	void UpdateVerticalVelocity(float elapsedFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	// 水平速力更新処理　
	void UpdateHorizontalVelocity(float elapsedFrame);

	// 水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

    // 前方レイ飛ばしてない時対応（敵用）
    void CheckWallCollision(float elapsedTime);



protected:
	// 移動処理
	void Move(float vx, float vy, float vz, float speed);
	void Move(float vx, float vz, float speed);

	// 旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 速度処理更新
	void UpdateVelocity(float elapsedTime);

	// 無敵時間更新
	void UpdateInvincibleTimer(float elapsedTime);

	// 着地したときに呼ばれる
	virtual void OnLanding() {}

	// ダメージを受けたときに呼ばれる
	virtual void OnDamaged() {}

	// 死亡したときに呼ばれる
	virtual void OnDead() {}


protected:
    // 基本情報設定
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
	DirectX::XMFLOAT4X4 rotation = {

	};
	float radius = 1.2f;
	float height = 1.9f;
	float gravity = -1.0f;
	DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	bool isGround = false;
	int health = 100;
	int maxHealth = 5;
	float stepOffset = 1.5f;
	float invincibleTimer = 0.0f;
	float friction = 0.5f;
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float airControl = 0.3f;
	float slopeRate = 1.0f;

	float skinWidth = 0.01f;
	float slopeLimit = 45.0f;

    //　生死判定
	bool isAlive = true;

	//　プレイヤー判定用
	bool hitWall = false;
	bool onClimb = false;
	bool CanClimb = false;
	bool onSwing = false;
	bool isThrown = false;
	bool IsLockedOn = false;

    //　壁登り用
	float fallStartHeight = 0.0f;
	float landingHeightThreshold = 1.0f;

	// クォータニオンを使った回転を管理するためのメンバ変数
	DirectX::XMFLOAT4 climbRotationQuaternion;
	DirectX::XMFLOAT3 wallNormal;

};

