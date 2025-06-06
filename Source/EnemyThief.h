#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "EnemyStateMachine.h"
#include "Telegram.h"
#include "Audio/Audio.h"

// エネミーアニメーション
enum class EnemyAnimation {
    Idle, Walk, Run, AttackShot, Die, GetHit,
    AttackPunch, GetThrow, HoldGun, KickDown
};

//ステートマシン
class EnemyStateMachine;

class EnemyThief :public Enemy
{
public:
    EnemyThief();
    ~EnemyThief()override;

    // ステート
    enum class State { Search, Battle, Recieve, Dead, Max };  //親ステート
    enum class Search { Wander, Idle, Max };            //子 (Search用)
    enum class Battle { Pursuit, Attack, Punch, Shot,
        Damage, Standby, Max };                   //子 (Battle用)
    enum class Recieve { Called, Max };                 //子（MetaAI用）

    enum class AttackType { Null, Punch, Shot };        //攻撃の種類
    AttackType randomType = AttackType::Null;

    float				stateTimer = 0.0f;  // ステートタイマー

    //関数
    void Update(float elapsedTime)override;             // 更新処理
    void DrawDebugPrimitive() override;                 // デバッグプリミティブ描画
    void SetTerritory(const DirectX::XMFLOAT3& origin,
        float range);                                   // 縄張り設定
    void DrawDebugGUI();                                // デバッグエネミー情報表示
    void SetRandomTargetPosition();                     // ターゲット位置をランダム設定
    void MoveToTarget(float elapsedTime,
        float speedRate);                               // 目標地点へ移動
    bool SearchPlayer();                                // プレイヤー索敵
    void SetTargetPosition(DirectX::XMFLOAT3 position)
    { targetPosition = position; }                      // ターゲットポジション設定
    void OnDead();                                      // 死亡したときに呼ばれる      
    void OnDamaged() override;                          // ダメージを受けたときに呼ばれる
    bool OnMessage(const Telegram& msg);                //　メッセージ受信関数を追加

    ///////////////////////////////////////////////////////////////////////////////////
    //                              ゲッター・セッター                               //
    ///////////////////////////////////////////////////////////////////////////////////
    DirectX::XMFLOAT3 GetPosition() { return position; }    // ポジション取得

    //攻撃
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }        // ターゲットポジション取得
    float GetAttackRange() { return attackRange; }    // 攻撃範囲取得
    float GetPunchRange() { return punchRange; }    // パンチ範囲取得

    //投げされる
    void SetThrow(bool thown) { GetThrowFlag = thown; }
    bool IsGetThrow() const { return GetThrowFlag; }

    //ステートマシン
    EnemyStateMachine* GetStateMachine() { return stateMachine; }    //ステートマシン取得

    //モデル
    Model* GetModel() { return model.get(); }    //モデル取得

    //ステート
    void SetStateTimer(float timer) {stateTimer = timer;}    // ステートタイマー設定
    float GetStateTimer() { return stateTimer; }    // ステートタイマー取得
private:
    // モデル
    std::unique_ptr<Model> model;

    // ステートマシン
    EnemyStateMachine* stateMachine = nullptr;

    //ステート
    State				state = State::Search;

    //索敵
    DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
    float				territoryRange = 7.0f;
    float				searchRange = 7.0f;

    //移動
    float				moveSpeed = 3.5f;
    float				turnSpeed = DirectX::XMConvertToRadians(360);

    //攻撃
    float				attackRange = 10.0f;
    float				punchRange = 1.5f;

    //投げされる時用
    float timeElapsed = 0.0f;
    float totalFlightTime;
    float webTimer;
    bool GetThrowFlag;

    float deadTimer = 0.0f;    // 死亡時のタイマー
    float deadTimerMax = 120.0f;    // 死亡時のタイマー最大値


};