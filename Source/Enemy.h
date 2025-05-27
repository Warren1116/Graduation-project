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

    // 関数
    virtual void Update(float elapsedTime) = 0;	    // 更新処理
    void Destroy();	                                // 破棄
    virtual void DrawDebugPrimitive();	            // デバッグプリミティブ描画
    virtual void DrawDebugGUI() {}                  // デバッグGUI描画
    virtual bool OnMessage(const Telegram& msg);	// メッセージ受信したときの処理
    void OnDamaged() override {};	                // ダメージを受けたときに呼ばれる
    void OnDead() override {};	                    // 死亡したときに呼ばれる

    ///////////////////////////////////////////////////////////////////////////////////
    //                              ゲッター・セッター                               //
    ///////////////////////////////////////////////////////////////////////////////////
    virtual bool	GetAttackFlg() { return attackFlg; }	        // 攻撃フラグ取得
    virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };	// 攻撃フラグセット
    virtual void	SetId(int id) { this->id = id; }
    virtual int		GetId() { return id; }
    virtual float GetAttackRange() { return attackRange; }	        // 攻撃範囲取得
    Model* GetModel() const { return model; }	                    // モデル取得

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

