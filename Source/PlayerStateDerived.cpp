#include "PlayerStateDerived.h"
#include "Player.h"
#include "Input/Input.h"
#include "SwingWeb.h"
#include "SceneGame.h"
#include "scenemanager.h"
#include "SceneLoading.h"

void PlayerStates::IdleState::Enter()
{
    owner->state = Player::State::Idle;
    owner->onSwing = false;
    owner->firstSwing = true;

    if (owner->lastState == Player::State::Swing)
    {
        // �O���ɐi�ނ��߂̃x�N�g�����v�Z
        DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&owner->GetFront());
        DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&owner->GetUp());

        forwardVec = DirectX::XMVector3Normalize(forwardVec);
        upVec = DirectX::XMVector3Normalize(upVec);

        forwardVec = DirectX::XMVectorScale(forwardVec, 5.0f); // �����𒲐�
        upVec = DirectX::XMVectorScale(upVec, 8.0f);

        // ���x�ɉ��Z
        DirectX::XMVECTOR velocityVec = DirectX::XMLoadFloat3(&owner->velocity);
        velocityVec = DirectX::XMVectorAdd(velocityVec, DirectX::XMVectorAdd(forwardVec, upVec));
        DirectX::XMStoreFloat3(&owner->velocity, velocityVec);

    }

    //�@�N���C�~���O���Ȃ�ʂ̑ҋ@���[�V����
    if (owner->onClimb)
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_HoldInWall), true);
    }
    //�@���ʂ��ҋ@���[�V����
    else
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Idle), true);
    }
    if (owner->attacking && owner->attackCount >= owner->attackLimit)
    {
        owner->attackTimer = 0;
        owner->attackCount = 0;
        owner->attacking = false;

    }

}
void PlayerStates::IdleState::Execute(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    owner->lastState = owner->state;
    //�ړ����͏���
    if (owner->InputMove(elapsedTime))
    {
        owner->SetState(Player::State::Move);
    }

    //�W�����v���͏���
    if (owner->InputJump())
    {
        owner->SetState(Player::State::Jump);

    }

    if (!owner->onClimb)
    {
        // �����͏���
        if (owner->InputDodge())
        {
            owner->SetState(Player::State::Dodge);
        }

        //�e�ۓ��͏���
        if (owner->InputProjectile())
        {
            owner->SetState(Player::State::Shot);
        }

        //�U�����͏���
        if (owner->InputAttack())
        {
            owner->SetState(Player::State::Attack);
        }
    }

    // �񕜓��͏���
    if (owner->skillTime > 1.0f)
    {
        owner->InputHealing();
    }

    //  �����Z���͏���
    if (owner->lockonEnemy && owner->skillTime >= 1)
    {
        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT || gamePad.GetButtonDown() & GamePad::BTN_Y)
        {
            owner->IsUseGrab = true;
            owner->SetState(Player::State::Grab);
        }

    }

    //  �K�E�Z���͏���
    if (owner->skillTime >= 3)
    {
        if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER || gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_V)
        {
            owner->SetState(Player::State::Ultimate);
        }
    }



}
void PlayerStates::IdleState::Exit()
{
}

void PlayerStates::MoveState::Enter()
{
    if (!owner->attacking)
    {
        owner->attackCount = 0;
        owner->attackTimer = 0;
        owner->attacking = false;
    }

    owner->state = Player::State::Move;
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Running), true);
    }
}

void PlayerStates::MoveState::Execute(float elapsedTime)
{
    if (!owner->canSwing)
    {
        owner->swingCooldownTimer -= elapsedTime;
        if (owner->swingCooldownTimer <= 0.0f)
        {
            owner->canSwing = true;
        }
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    //  �ړ���SHIFT�L�[�����ɉ�����
    if ((gamePad.GetButton() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && owner->InputMove(elapsedTime) && owner->firstSwing)
    {
        owner->lastState = owner->state;
        if (owner->canSwing)
        {
            //  �X�C���O�X�e�[�g�ւ̑J��
            owner->canSwing = false;
            owner->swingCooldownTimer = owner->swingCooldown;
            owner->SetState(Player::State::Swing);
        }
    }

    if (owner->InputDodge())
    {
        owner->SetState(Player::State::Dodge);
    }

    //  �ڂ̑O�ɕǂ����铯����Speac�L�[��������
    if (owner->hitWall && owner->onClimb)
    {
        //�N���C�~���O�X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Climb);
    }

    //  �ړ����ĂȂ��Ȃ�
    if (!owner->InputMove(elapsedTime))
    {
        //�ҋ@�X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Idle);
    }

    //�@�����ڂ̑O�ɕǂ����Ȃ��A�W�����v���͏���
    if (!owner->hitWall && owner->InputJump())
    {
        //�W�����u�X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Jump);
    }

    // �e�ۓ��͏���
    if (owner->InputProjectile())
    {
        owner->SetState(Player::State::Shot);
    }

    // �U�����͏���
    if (owner->InputAttack())
    {
        owner->SetState(Player::State::Attack);
    }

    // �񕜓��͏���
    if (owner->skillTime > 1.0f)
    {
        owner->InputHealing();
    }

    //  �K�E�Z���͏���
    if (owner->skillTime >= 3)
    {
        if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER || gamePad.GetButtonDown() & GamePad::BTN_KEYBOARD_V)
        {
            owner->SetState(Player::State::Ultimate);
        }
    }

    //�@�����Z���͏���
    if (owner->lockonEnemy && owner->skillTime >= 1)
    {
        Mouse& mouse = Input::Instance().GetMouse();
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT || gamePad.GetButtonDown() & GamePad::BTN_Y)
        {
            owner->IsUseGrab = true;
            owner->SetState(Player::State::Grab);
        }
    }
}

void PlayerStates::MoveState::Exit()
{
}

void PlayerStates::JumpState::Enter()
{
    owner->state = Player::State::Jump;

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Jump), false);
}

void PlayerStates::JumpState::Execute(float elapsedTime)
{
    if (owner->InputAttack())
    {
        owner->SetState(Player::State::SwingToKick);
    }

    GamePad& gamePad = Input::Instance().GetGamePad();

    //  �W�����u���ړ���SHIFT�L�[�����ɉ�����
    if ((gamePad.GetButton() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButton() & GamePad::BTN_RIGHT_TRIGGER) && owner->InputMove(elapsedTime))
    {
        owner->lastState = owner->state;
        //  �X�C���O�X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Swing);
    }

    //  �����N���C�~���O���Ȃ�A�N���C�~���O��Ԃ��L�����Z������
    if (owner->onClimb)
    {
        owner->onClimb = false;
    }

    //�ړ����͏���
    owner->InputMove(elapsedTime);

    if (!owner->model->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Idle);
    }

    //  �e�ۂ̓��͏���
    if (owner->InputProjectile())
    {
        //  ���˃X�e�[�g�ւ̑J��
        owner->SetState(Player::State::Shot);
    }
}

void PlayerStates::JumpState::Exit()
{
}

void PlayerStates::LandState::Enter()
{
    //  �X�C���O�����ɒ��n����ƁAvelocity�̉e���ŏ��Ɋ���̂�
    //���̏󋵂�h������ velocity��0�ɂ���
    owner->velocity.x = 0;
    owner->velocity.y = 0;
    owner->velocity.z = 0;
    //  �ŏ��̔��肪�Ȃ��ƁA�����ɔ�ё�����̂�
    owner->firstSwing = true;
    owner->state = Player::State::Land;

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Landing), false);
}

void PlayerStates::LandState::Execute(float elapsedTime)
{
    owner->onClimb = false;
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
}

void PlayerStates::LandState::Exit()
{
}

void PlayerStates::AttackState::Enter()
{
    owner->state = Player::State::Attack;
    owner->attacking = true;
    // �U���A�j���[�V�����Đ�
    owner->PlayAttackAnimation();
}
void PlayerStates::AttackState::Execute(float elapsedTime)
{
    //�@���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
    //�@�V���[�g���͏���
    if (owner->InputProjectile())
    {
        owner->SetState(Player::State::Shot);
    }
    //�@Dodge���͏���
    if (owner->InputDodge())
    {
        owner->SetState(Player::State::Dodge);
    }

    // �A�j���[�V�����̍Đ����Ԃ��擾
    float animationTime = owner->model->GetCurrentAnimationSeconds();
    float animationSpeed = owner->model->GetCurrentAnimationSpeed();

    // �A�j���[�V�����̑��x�ɉ����čU���̓����蔻��𒲐�
    float adjustedStartTime = 0.3f / animationSpeed;
    float adjustedEndTime = 1.0f / animationSpeed;
    owner->attackCollisionFlag = animationTime >= adjustedStartTime && animationTime <= adjustedEndTime;

    //  �U���̓����蔻��
    if (owner->attackCollisionFlag)
    {
        //  �J�������b�N���U�������
        if (owner->lockonState == Player::LockonState::Locked && owner->lockonEnemy != nullptr)
        {
            //  ������Ƃ����G�̕����ړ�����
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.075f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&owner->lockDirection, directionToEnemy);
            owner->Turn(elapsedTime, owner->lockDirection.x, owner->lockDirection.z, owner->turnSpeed);

            if (owner->attackCount <= 2)
            {
                playerPos = DirectX::XMVectorAdd(playerPos, moveVector);
                DirectX::XMStoreFloat3(&owner->position, playerPos);
            }
            else
            {
                float moveDistance = 0.1;
                playerPos = DirectX::XMVectorAdd(playerPos, moveVector);
                DirectX::XMStoreFloat3(&owner->position, playerPos);
            }
        }

        //  �A���ɂ���āA�m�[�h�ƃG�l�~�[�̏Փˏ���
        switch (owner->attackCount)
        {
        case 1:
            owner->CollisionNodeVsEnemies("mixamorig:LeftHandMiddle1", owner->attackRadius);
            break;
        case 2:
            owner->CollisionNodeVsEnemies("mixamorig:RightHandMiddle2", owner->attackRadius);
            break;
        case 3:
            owner->CollisionNodeVsEnemies("mixamorig:RightFoot", owner->attackRadius + 0.2f);
            break;

        }

    }
}
void PlayerStates::AttackState::Exit()
{
}

void PlayerStates::ShotState::Enter()
{
    owner->state = Player::State::Shot;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Shoting), false);
}
void PlayerStates::ShotState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
    }
}
void PlayerStates::ShotState::Exit()
{

}

void PlayerStates::DeathState::Enter()
{
    //  ���S������HP��0�ɂ���
    if (owner->health <= 0)
    {
        owner->health = 0;
    }

    owner->state = Player::State::Death;

    // ���S�A�j���[�V�����Đ�
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Death), false);
}

void PlayerStates::DeathState::Execute(float elapsedTime)
{
    //�@���[�V�������I�������V�[����؂�ւ���
    if (!owner->model->IsPlayAnimation())
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
}

void PlayerStates::DeathState::Exit()
{
}


void PlayerStates::DodgeState::Enter()
{
    owner->state = Player::State::Dodge;
    float attackAnimationSpeed = 1.2f; // �A�j���[�V�������x�𑬂����邽�߂̔{��

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Dodge), false, 0.2, attackAnimationSpeed);
    owner->getAttacksoon = false;

}

void PlayerStates::DodgeState::Execute(float elapsedTime)
{
    //���G����
    owner->invincibleTimer = 2.0f;

    // �h�b�W�̕����Ɋ�Â��Ĉړ�
    DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&owner->velocity);
    vel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&owner->dodgeDirection), 1.2f);

    // �v���C���[�̈ʒu���X�V
    DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
    playerPos = DirectX::XMVectorAdd(playerPos, DirectX::XMVectorScale(vel, elapsedTime));

    DirectX::XMStoreFloat3(&owner->velocity, vel);
    DirectX::XMStoreFloat3(&owner->position, playerPos);

    if (!owner->model->IsPlayAnimation()) owner->SetState(Player::State::Idle);
}

void PlayerStates::DodgeState::Exit()
{
}

void PlayerStates::ClimbState::Enter()
{
    owner->state = Player::State::Climb;
    if (owner->velocity.y >= 0)
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Climb), true);
    }
    else
    {
        owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_ClimbDown), true);
    }
    owner->onSwing = false;
}

void PlayerStates::ClimbState::Execute(float elapsedTime)
{
    //�@�N���C�~���O��Space�L�[�����Ό��̏�Ԃɖ߂�
    if (owner->InputJump())
    {
        owner->SetState(Player::State::Jump);
    }

    //  �ړ����ĂȂ��Ȃ�ҋ@�X�e�[�g�ւ̑J��
    if (!owner->InputMove(elapsedTime))
    {
        owner->SetState(Player::State::Idle);
    }

    //�@�ǂ̏�ɓo��邩�ǂ����̔���
    if (owner->IsNearWallTop())
    {
        owner->SetState(Player::State::ClimbTop);
    }
    else
    {
        owner->CanClimb = false;
    }
}

void PlayerStates::ClimbState::Exit()
{
}

void PlayerStates::DamageState::Enter()
{
    owner->state = Player::State::Damage;
    owner->Damage->Play(false, 0.4f);

    // �_���[�W�A�j���[�V�����Đ�
    float attackAnimationSpeed = 1.5f; // �A�j���[�V�������x�𑬂����邽�߂̔{��
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_GetHit1), false, 0.2f, attackAnimationSpeed);

    MessageData::CAMERASHAKEDATA	p = { 0.1f, 1.0f };
    Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);

    // �_���[�W���R���g���[���[�U��
    GamePad& gamePad = Input::Instance().GetGamePad();
    gamePad.SetVibration(0.5f, 0.5f);
}
void PlayerStates::DamageState::Execute(float elapsedTime)
{
    // �_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!owner->model->IsPlayAnimation())
    {
        // �R���g���[���[�U����~
        GamePad& gamePad = Input::Instance().GetGamePad();
        gamePad.StopVibration();

        owner->SetState(Player::State::Idle);
    }
}
void PlayerStates::DamageState::Exit()
{
}

void PlayerStates::SwingState::Enter()
{
    if (owner->onClimb) return;
    owner->state = Player::State::Swing;
    //  �ړ����X�C���O�����
    if (owner->lastState == Player::State::Move)
    {
        if (owner->firstSwing)
        {
            owner->FirstSwing->Play(false, 0.8f);
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_StartSwing), false);
            owner->firstSwing = false;
            //  �X�C���O���f���i���݉��AGeometric�ɕύX�\��j
            SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  �����_���[�ɓo�^
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }
        else
        {
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Swinging), false);
            SwingWeb* swingWebLeft = new SwingWeb(&owner->projectileManager, true);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  �����_���[�ɓo�^
                sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            }

        }
        //  �X�C���O�ʒu��T��
        owner->FindWallSwingPoint();
        owner->onSwing = true;
    }
    //  �W�����v���X�C���O�����
    else if (owner->lastState == Player::State::Jump)
    {
        //�A���X�C���O�̃��[�V����
        if (!owner->onSwing)
        {
            owner->FirstSwing->Play(false, 0.8f);
            //  �X�C���O���f���i���݉��AGeometric�ɕύX�\��j
            SwingWeb* swingWebLeft = new SwingWeb(&owner->projectileManager, true);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  �����_���[�ɓo�^
                sceneGame.RegisterRenderModel(swingWebLeft->GetModel());
            }

        }
        //�@����X�C���O�̃��[�V����  
        {
            owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Swinging), false);
            //  �X�C���O���f���i���݉��AGeometric�ɕύX�\��j
            SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                //  �����_���[�ɓo�^
                sceneGame.RegisterRenderModel(swingWebRight->GetModel());
            }
        }
        //  �X�C���O�ʒu��T��
        owner->FindWallSwingPoint();
        owner->onSwing = true;
    }
}

void PlayerStates::SwingState::Execute(float elapsedTime)
{
    //�X�C���O�̓����蔻��
    owner->SwingCollision(elapsedTime);

    //�U��q�^���̌v�Z
    owner->HandleSwingPhysics(elapsedTime, 10.0f, 10.0f, 0.005f);

    GamePad& gamePad = Input::Instance().GetGamePad();

    //  �L�[��������ĂȂ����X�C���O���L�����Z������
    if (gamePad.GetButtonUp() & GamePad::BTN_KEYBOARD_SHIFT || gamePad.GetButtonUp() & GamePad::BTN_RIGHT_TRIGGER)
    {
        owner->lastState = owner->state;
        owner->SetState(Player::State::Idle);
    }
}

void PlayerStates::SwingState::Exit()
{
}

void PlayerStates::ClimbTopState::Enter()
{
    owner->state = Player::State::ClimbTop;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_ClimbUpWall), false);
}

void PlayerStates::ClimbTopState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation()) owner->SetState(Player::State::Idle);
}

void PlayerStates::ClimbTopState::Exit()
{
}

void PlayerStates::GrabState::Enter()
{
    owner->state = Player::State::Grab;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_GrabAndDrop), false);
    owner->webTimer = 0.0f;
    owner->skillTime -= 1.0f;
    owner->getShotsoon = false;
}

void PlayerStates::GrabState::Execute(float elapsedTime)
{
    owner->webTimer += elapsedTime;

    if (!owner->model->IsPlayAnimation())
    {
        owner->shotWebPlayed = false;
        owner->fallSoundPlayed = false;

        owner->lockonEnemy->ApplyDamage(110.0, 1.0);
        owner->IsUseGrab = false;

        if (owner->lockonEnemy)
        {
            // lockonEnemy �� Character �Ɠ������ꍇ�ɃN���A
            CharacterManager& manager = CharacterManager::Instance();
            for (int ii = 0; ii < manager.GetCharacterCount(); ++ii)
            {
                Character* character = manager.GetCharacter(ii);
                if (character == owner->lockonEnemy)
                {
                    manager.Remove(character);
                    owner->lockonEnemy = nullptr;
                    break;
                }
            }
        }
        owner->SetState(Player::State::Idle);
    }
    //  �J�������b�N���̏���
    if (owner->lockonState == Player::LockonState::Locked && owner->lockonEnemy != nullptr)
    {
        if (owner->webTimer <= 0.82)
        {
            //�v���C���[�������X�V����
            DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&owner->position);
            DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
            DirectX::XMVECTOR directionToEnemy = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemyPos, playerPos));

            float moveDistance = 0.04f;
            DirectX::XMVECTOR moveVector = DirectX::XMVectorScale(directionToEnemy, moveDistance);

            DirectX::XMStoreFloat3(&owner->lockDirection, directionToEnemy);
            owner->Turn(elapsedTime, owner->lockDirection.x, owner->lockDirection.z, owner->turnSpeed);
        }

        if (owner->webTimer > 0.82f)
        {
            //�G�̈ʒu���擾
            auto enemyPos = Player::Instance().GetLockonEnemy()->GetPosition();
            float animationProgress = (owner->model->GetCurrentAnimationSeconds() / owner->model->GetCurrentAnimationLength()) * 0.2f;
            enemyPos.y += 1.0f;
            enemyPos.y = enemyPos.y * (1.0f + animationProgress);

            //��̈ʒu���擾
            Model::Node* RightHandPos = owner->model->FindNode("mixamorig:RightHand");
            DirectX::XMFLOAT3 pos;
            pos.x = RightHandPos->worldTransform._41;
            pos.y = RightHandPos->worldTransform._42;
            pos.z = RightHandPos->worldTransform._43;
            //���𐶐�
            if (owner->ActiveGrabWeb(pos, enemyPos) && !owner->shotWebPlayed)
            {
                owner->ShotWeb->Play(false, 0.8f);
                owner->shotWebPlayed = true;
            }
        }

        if (owner->webTimer >= 3.8f && !owner->fallSoundPlayed)
        {
            owner->Fall->Play(false);
            owner->fallSoundPlayed = true; // �������Đ����ꂽ���Ƃ��L�^
        }
    }
}

void PlayerStates::GrabState::Exit()
{
}

void PlayerStates::CrouchIdleState::Enter()
{
    owner->state = Player::State::CrouchIdle;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_CrouchIdle), false);
}

void PlayerStates::CrouchIdleState::Execute(float elapsedTime)
{
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::TitleIdle);
    }
}

void PlayerStates::CrouchIdleState::Exit()
{

}

void PlayerStates::TitleIdleState::Enter()
{
    owner->state = Player::State::TitleIdle;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Crouch), false);
}

void PlayerStates::TitleIdleState::Execute(float elapsedTime)
{

}

void PlayerStates::TitleIdleState::Exit()
{
}

void PlayerStates::SwingToKickState::Enter()
{
    owner->state = Player::State::SwingToKick;

    SwingWeb* swingWebRight = new SwingWeb(&owner->projectileManager, false);
    SceneGame& sceneGame = SceneGame::Instance();
    if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
    {
        //  �����_���[�ɓo�^
        sceneGame.RegisterRenderModel(swingWebRight->GetModel());
    }

    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_SwingToKick), false);
    owner->IsUseSwingKick = true;

    if (owner->lockonEnemy)
    {
        DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&owner->lockonEnemy->GetPosition());
        DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0f, 13.0f, 0.0f, 0.0f); // �����12.0f�̃I�t�Z�b�g
        DirectX::XMVECTOR swingPointVec = DirectX::XMVectorAdd(enemyPos, upVec);
        DirectX::XMStoreFloat3(&owner->swingPoint, swingPointVec);
    }
    else
    {
        owner->FindWallSwingPoint();
    }
}

void PlayerStates::SwingToKickState::Execute(float elapsedTime)
{
    //�X�C���O�̓����蔻��
    owner->SwingCollision(elapsedTime);

    //�U��q�^���̌v�Z
    owner->HandleSwingPhysics(elapsedTime, 12.0f, 20.0f, 0.0001f);

    // �A�j���[�V�����̍Đ����Ԃ��擾
    float animationTime = owner->model->GetCurrentAnimationSeconds();

    //�@�E���̍U������
    owner->CollisionNodeVsEnemies("mixamorig:RightToeBase", owner->attackRadius);

    //�@���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
        owner->IsUseSwingKick = false;
    }
}

void PlayerStates::SwingToKickState::Exit()
{
}

void PlayerStates::UltimateState::Enter()
{
    owner->state = Player::State::Ultimate;
    owner->skillTime -= 3;
    owner->model->PlayAnimation(static_cast<int>(PlayerAnimation::Anim_Ultimate), false);
}

void PlayerStates::UltimateState::Execute(float elapsedTime)
{
    static float timeSinceLastShot = 0.0f;
    static int currentDirectionIndex = 0;
    static int shotsPerDirection = 0;
    static std::vector<DirectX::XMFLOAT3> directions;

    // �A�j���[�V�������Đ����łȂ��ꍇ�A�ҋ@�X�e�[�g�ɑJ��
    if (!owner->model->IsPlayAnimation())
    {
        owner->SetState(Player::State::Idle);
        timeSinceLastShot = 0.0f;
        currentDirectionIndex = 0;
        shotsPerDirection = 0;
        directions.clear();
        return;
    }

    // ����̂݃����_���ȕ����𐶐�
    if (directions.empty())
    {
        for (int i = 0; i < 8; ++i)
        {
            float x, y, z;
            do
            {
                x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
            } while (x * x + y * y + z * z > 1.0f || y < 0.0f); // ������ɐ���

            float length = sqrtf(x * x + y * y + z * z);
            x /= length;
            y /= length;
            z /= length;

            directions.push_back({ x, y, z });
        }
    }

    // �v���C���[�̈ʒu���擾
    DirectX::XMFLOAT3 playerPos = { owner->position.x,owner->position.y + 1.0f,owner->position.z };

    // ���ˊԊu��ݒ�
    const float shotInterval = 0.2f;

    // ���Ԃ��X�V
    timeSinceLastShot += elapsedTime;

    // ���ˊԊu���o�߂�����e�ۂ𔭎�
    if (timeSinceLastShot >= shotInterval)
    {
        timeSinceLastShot = 0.0f;

        // ���݂̕����ɒe�ۂ𔭎�
        if (shotsPerDirection < 2)
        {
            owner->ShotWeb->Play(false, 0.8f);
            // �e�ۂ𐶐����Ĕ���
            ProjectileStraight* projectile = new ProjectileStraight(&owner->projectileManager);
            projectile->Launch(directions[currentDirectionIndex], playerPos);

            // ���f���������_���[�ɓo�^
            SceneGame& sceneGame = SceneGame::Instance();
            if (sceneGame.shadowmapRenderer && sceneGame.sceneRenderer)
            {
                sceneGame.RegisterRenderModel(projectile->GetModel());
            }

            shotsPerDirection++;
        }
        else
        {
            // ���̕����Ɉړ�
            currentDirectionIndex++;
            shotsPerDirection = 0;

            // �S�����ɔ��˂��I�������I��
            if (currentDirectionIndex >= directions.size())
            {
                owner->SetState(Player::State::Idle);
                return;
            }
        }
    }
    // �U���͈͂̐ݒ�
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

        // �v���C���[�ƓG�̋������v�Z
        float dx = enemyPos.x - owner->position.x;
        float dz = enemyPos.z - owner->position.z;
        float distance = sqrtf(dx * dx + dz * dz);

        // �G���U���͈͓��ɂ���ꍇ�A�_���[�W��^����
        if (distance <= owner->ultimateAttackRadius)
        {
            enemy->ApplyDamage(50, 0.5f); // �_���[�W�ʂƖ��G���Ԃ�ݒ�
        }
    }

}

void PlayerStates::UltimateState::Exit()
{
}
