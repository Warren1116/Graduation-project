#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "CameraController.h"
#include "ProjectileManager.h"
#include "BrokenProjectileManager.h"
#include <memory.h>
#include "Enemy.h"
#include "EnemyThief.h"
#include "PlayerStateMachine.h"
#include "PlayerStateDerived.h"


//�A�j���[�V����
enum class PlayerAnimation
{
    Anim_Idle, Anim_Walking, Anim_GetHit1, Anim_Death, Anim_Running,
    Anim_Climb, Anim_Landing, Anim_Jump, Anim_HoldInWall, Anim_Attack,
    Anim_Attack2, Anim_Kick, Anim_StartSwing, Anim_Swinging, Anim_Swinging2,
    Anim_Shoting, Anim_Dodge, Anim_ClimbUpWall, Anim_ClimbDown, Anim_SwingToLand,
    Anim_GrabAndDrop, Anim_CrouchIdle, Anim_TitleIdle, Anim_Crouch,
    Anim_SwingToKick, Anim_Ultimate
};

//�@�X�e�[�g�}�V��
class PlayerStateMachine;

// �v���C���[�̏�Ԃ��Ǘ�����N���X
namespace PlayerStates
{
    class IdleState; class MoveState; class JumpState; class ClimbState;
    class AttackState; class DamageState; class SwingState; class SwingToKickState;
    class LandState; class ShotState; class GrabState; class DodgeState;
    class ClimbTopState; class UltimateState; class DeathState;
    class CrouchIdleState; class TitleIdleState; class BounceState;
}


// �v���C���[
class Player : public Character
{
    //�@Friend�N���X�錾
    friend class PlayerStates::IdleState;
    friend class PlayerStates::MoveState;
    friend class PlayerStates::JumpState;
    friend class PlayerStates::ClimbState;
    friend class PlayerStates::AttackState;
    friend class PlayerStates::DamageState;
    friend class PlayerStates::SwingState;
    friend class PlayerStates::SwingToKickState;
    friend class PlayerStates::LandState;
    friend class PlayerStates::ShotState;
    friend class PlayerStates::GrabState;
    friend class PlayerStates::DodgeState;
    friend class PlayerStates::ClimbTopState;
    friend class PlayerStates::UltimateState;
    friend class PlayerStates::DeathState;
    friend class PlayerStates::CrouchIdleState;
    friend class PlayerStates::TitleIdleState;
    friend class PlayerStates::BounceState;

public:
    // �X�e�[�g
    enum class State
    {
        Idle, Move, Jump, Land, Attack, Damage, Death, Dodge,
        Climb, Swing, Shot, ClimbTop, Grab, CrouchIdle, TitleIdle,
        SwingToKick, Ultimate, Bounce, EventMode
    };

    // ���b�N�I���X�e�[�g
    enum class LockonState
    {
        NotLocked,
        Locked,
    };

    Player(bool flag);
    ~Player();
    static Player& Instance() { return *instance; }

    // �X�e�[�g�}�V��
    PlayerStateMachine* stateMachine = nullptr;
    //�@���f��
    std::unique_ptr<Model> model = nullptr;

    // �֐�
    void Update(float elapsedTime);     // �X�V����
    void DrawDebugGUI();                // �f�o�b�O�pGUI�`��
    bool InputMove(float elapsedTime);  // �ړ����͏���
    bool InputJump();                   // �W�����v���͏���
    bool InputProjectile();             //�@�e�ۂ̓��͏���
    bool InputAttack();                 // �U�����͏���
    bool InputDodge();                  // Dodge���͏���
    bool InputHealing();                //�@�񕜓��͏���
    void DrawDebugPrimitive();          // �f�o�b�O�v���~�e�B�u�`��

    ///////////////////////////////////////////////////////////////////////////////////
    //                              �Q�b�^�[�E�Z�b�^�[                               //
    ///////////////////////////////////////////////////////////////////////////////////
    //�e��
    ProjectileManager& GetProjectileManager() { return projectileManager; }    //	�e�ۂ̃}�l�[�W���[�����
    BrokenProjectileManager& GetBrokenProjectileManager() { return brokenprojectileManager; }    //	�j�󂵂��e�ۂ̃}�l�[�W���[�����

    //�G
    Enemy* GetLockonEnemy() const { return static_cast<Enemy*>(lockonEnemy); }    //�@���b�N���̓G���擾
    EnemyThief* GetAttackEnemy() { return attackEnemy; }                          //�@���U������̓G���擾����
    void SetAttackEnemy(EnemyThief* enemy) { attackEnemy = enemy; }
    bool GetAttackSoon() { return getAttacksoon; }                                //�@�U���\�m�t���O�����
    void SetgetAttackSoon(bool getattack) { getAttacksoon = getattack; }
    bool GetShotSoon() { return getShotsoon; }                                    //  �U���\�m�t���O�����(Shot)
    void SetgetShotSoon(bool getshot) { getShotsoon = getshot; }

    //�X�C���O
    DirectX::XMFLOAT3 GetswingPoint() { return swingPoint; }    //	�X�C���O�ʒu���擾
    DirectX::XMFLOAT3 GetPreviousSwingPoint() { return previousSwingPoint; }    //	�L�^���ꂽ�X�C���O�|�C���g���擾

    //�@�X�L��
    bool GetIsUseGrab() { return IsUseGrab; }               //    �����Z�t���O�����
    bool GetIsUseSwingKick() { return IsUseSwingKick; }    //    �X�C���O�L�b�N�t���O�����
    float GetWebTimer() const { return webTimer; }    //  �����˂̃^�C�}�[
    float GetSkillTime() const { return skillTime; }    //  �X�L���̉񐔂����

    // �X�e�[�g
    const State& GetState() const { return state; }
    State SetState(State state) { return this->state = state; }
    State ChangeState(State state) { return this->state = state; }
    LockonState GetlockonState() { return lockonState; }    //	�v���C���[�̃��b�N�X�e�[�g�����

protected:
    // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDamaged() override;
    // ���S�����Ƃ��ɌĂ΂��
    void OnDead() override;
    //�@���n�̎��ɌĂ΂��
    void OnLanding() override;

private:
    // �R���W�������b�V��
    struct CollisionMesh
    {
        struct Triangle
        {
            DirectX::XMFLOAT3	positions[3];
            DirectX::XMFLOAT3	normal;
        };
        std::vector<Triangle>	triangles;

        struct Area
        {
            DirectX::BoundingBox	boundingBox;
            std::vector<int>		triangleIndices;
        };
        std::vector<Area>		areas;
    };

    // �B���instance
    static Player* instance;
    //���f���m�[�h
    std::vector<Model::NodePose> nodePoses;

    ///////////////////////////////////////////////////////////////////////////////////
    //                                   �v���C���[                                  //
    ///////////////////////////////////////////////////////////////////////////////////
    // �X�e�[�g
    State state = State::Idle;
    State lastState;

    // �퓬
    bool attacking = false;    // �U������
    float attackTimer = 0;
    int attackCount = 0;    // �U���̉�
    const int attackLimit = 3;
    float attackRadius = 0.4f;    // �U�����a
    bool attackCollisionFlag = false;
    float maxAngleX = DirectX::XMConvertToRadians(35);
    float mixAngleX = DirectX::XMConvertToRadians(-35);
    bool getAttacksoon = false;    //	�U���\�m
    bool getShotsoon = false;    //	�U���\�m

    // �X�C���O
    DirectX::XMFLOAT3 previousSwingPoint;        //	�O��̃X�C���O�|�C���g���L�^
    DirectX::XMFLOAT3 swingPoint;    //	�X�C���O�|�C���g�̈ʒu
    DirectX::XMFLOAT3 swingwebDirection;    //	�X�C���O�|�C���g�̕���
    DirectX::XMVECTOR SwingwebDirection;
    bool firstSwing = true;    //	����̃X�C���O�𔻒�
    float swingCooldown = 0.3f;    //�A���X�C���O������邽��
    float swingCooldownTimer = 0.0f;
    bool canSwing = true;

    // �o�E���X
    bool ropeAttached = false;
    float bounceTimer = 0.0f;
    const float ropeCoolDown = 1.0f;

    // �J�������b�N�p
    LockonState			lockonState = LockonState::NotLocked;
    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;
    Character* lockonEnemy = nullptr;
    DirectX::XMFLOAT3	lockDirection;
    const float MAX_LOCKON_DISTANCE = 15.0f;

    // �����Z�E�K�E�Z�p
    bool IsUseGrab = false;         // �����Z
    float webTimer;                 // ���̃^�C�}�[
    bool IsUseSwingKick = false;    // �X�C���O�L�b�N
    float ultimateAttackRadius = 5.0f;// �U���͈͂̔��a
    float skillTime;                // 
    float skillTimeMax = 5.0f;      // 

    // ���E�G�t�F�N�g
    std::unique_ptr<Effect> hitEffect = nullptr;        //�@�q�b�g�G�t�F�N�g
    std::unique_ptr<AudioSource> punch = nullptr;       //	�U����
    std::unique_ptr<AudioSource> punch2 = nullptr;      //	�U����
    std::unique_ptr<AudioSource> kick = nullptr;        //	�U����
    std::unique_ptr<AudioSource> Fall = nullptr;        //  ���Ƃ���
    bool fallSoundPlayed = false;
    std::unique_ptr<AudioSource> spiderSense = nullptr; //  �X�p�C�_�[�Z���X
    bool spiderSensePlayed = false;
    std::unique_ptr<AudioSource> FirstSwing = nullptr;  //�@�V���[�g��
    std::unique_ptr<AudioSource> Swing = nullptr;       //�@�V���[�g��
    std::unique_ptr<AudioSource> ShotWeb = nullptr;     //�@�V���[�g��
    bool shotWebPlayed = false;
    std::unique_ptr<AudioSource> Healing = nullptr;     //  �񕜉�
    std::unique_ptr<AudioSource> Damage = nullptr;      //	�_���[�W��

    // �e��
    ProjectileManager projectileManager;
    BrokenProjectileManager brokenprojectileManager;

    // �Ǔ����蔻��̃`�F�b�N
    DirectX::XMFLOAT3 checkpos;
    DirectX::XMVECTOR checkDirection;

    // �ړ�
    float moveSpeed = 5.0f;                                //�@�ړ��X�r�[�g
    float climbSpeed = 5.0f;                               //�@�N���C�~���O�X�r�[�g
    float turnSpeed = DirectX::XMConvertToRadians(720);    //�@��]�X�r�[�g
    float jumpSpeed = 16.0f;                               //�@�W�����u�X�r�[�g
    DirectX::XMFLOAT3 dodgeDirection;                      //  ����̕���

    // �G
    EnemyThief* attackEnemy = nullptr;                     //�@�U���Ώ�

    // �֐�
    void CollisionPlayerVsEnemies();    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionProjectileVsEnemies();    //	�e�ۂƓG�Ƃ̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void UpdateCameraState(float elapsedTime);    // �J�����X�e�[�g�̍X�V
    void PlayAttackAnimation();    //	�A���U���̃��[�V����
    bool IsNearWallTop();    //	�ǒ[�����̔���
    bool FindWallSwingPoint();    //	�X�C���O�|�C���g��T��
    void SwingCollision(float elapsedTime);    //	�X�C���O�̓����蔻��
    // �X�C���O�̌v�Z
    void HandleSwingPhysics(float elapsedTime, float ropeLength, float gravityStrength, float dragCoefficient);
    //�@�����ZActive
    bool ActiveGrabWeb(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);
    DirectX::XMFLOAT3 GetMoveVec() const;    // �ړ��x�N�g���擾
};

