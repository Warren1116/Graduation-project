#include "Character.h"
#include "StageManager.h"
#include "Mathf.h"
#include "Input/Input.h"

//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);

}

bool Character::ApplyDamage(int damage)
{
    if (damage == 0)    return false;
    if (health <= 0)    return false;

    health -= damage;

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }
    return true;
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
    if (damage == 0)    return false;
    if (health <= 0)    return false;

    if (invincibleTimer > 0) return false;

    health -= damage;

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        invincibleTimer = invincibleTime;
        OnDamaged();
        //UpdateInvincibleTimer(invincibleTime);

    }
    return true;

}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}


//移動処理
void Character::Move(float vx,float vy, float vz, float speed)
{
    moveVecX = vx;
    moveVecZ = vz;
    moveVecY = vy;
    maxMoveSpeed = speed;

}
void Character::Move(float vx, float vz, float speed)
{
    moveVecX = vx;
    moveVecZ = vz;
    maxMoveSpeed = speed;

}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    //if (vx == 0 && vz == 0) return;
    float length = sqrtf((vx * vx) + (vz * vz));
    if (length < 0.001f) return;
    //進行ベクトルを単位ベクトル化
    vx /= length;
    vz /= length;

    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (frontX * vx) + (frontZ * vz);

    //内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    //小さいほど1.0に近づくという性質を利用して回転速度を調節する
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;

    //左右判定を行うために２つの単位ベクトルの外積を計算する
    float cross = (frontZ * vx) - (frontX * vz);

    if (cross < 0.0f)
    {
        //angle.y -= speed;
        angle.y -= rot;
    }
    else
    {
        //angle.y += speed;
        angle.y += rot;
    }

}

void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime)
{
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    //水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    //水平移動更新処理
    UpdateHorizontalMove(elapsedTime);

}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //重力処理
    if (!onClimb)
    {
        velocity.y += gravity * elapsedFrame;
    }
    else 
    {
        velocity.y = climbSpeed;
    }
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    ////Y軸移動処理
    //position.y += velocity.y * elapsedTime;
    ////地面判定
    //if (position.y < 0.0f)
    //{
    //    position.y = 0.0f;
    //    velocity.y = 0.0f;
    //    if (!isGround)
    //    {
    //        OnLanding();
    //    }
    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}


    //垂直方向の移動量
    float my = velocity.y * elapsedTime;


    slopeRate = 0.0f;

    //キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            normal = hit.normal;

            //地面に接地している
            //position.y = hit.position.y;
            position = hit.position;

            //回転
            angle.y += hit.rotation.y;

            //着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

        }
        else if(!onClimb)
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    //上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //地面の向きに沿うようにXZ軸回転
    {
        //Y軸が法線ベクトル方向に向くオイラー角回転を算出する
        float ax = atan2f(normal.z, normal.y);
        float az = -atan2f(normal.x, normal.y);

        //線形補完で滑らかに回転する
        float time = 0.2f;
        angle.x = Mathf::Lerp(angle.x, ax, time);
        angle.z = Mathf::Lerp(angle.z, az, time);
    }
}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //XZ平面の速力を減速する
    float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elapsedFrame;


        //空中にいるときは摩擦力を減らす
        if (!isGround) friction *= airControl;


        //摩擦による横方向の減速処理
        if (length > friction)
        {
            //単位ベクトル化
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            float vy = velocity.y / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
            velocity.y -= vy * friction;
        }
        //横方向の速力が摩擦力以下になったので速力を無効化
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }
    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecY * moveVecY + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            float acceleration = this->acceleration * elapsedFrame;

            //空中にいるときは加速力を減らす
            if (!isGround) acceleration *= airControl;

            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //下り坂でガタガタすしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime)
{
    ////XZ軸移動処理
    //position.x += velocity.x * elapsedTime;
    //position.z += velocity.z * elapsedTime;

    //水平速力量計算
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;
        float my = velocity.y * climbSpeed * elapsedTime;

        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset ,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //壁からぶち込んたのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
            //入射ベクトルを法線に射影
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f);


            //補正位置の計算
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            HitResult hit2;
            if (!StageManager::Instance().RayCast(start, collectPosition, hit2))
            {
                hitWall = true;
                GamePad& gamePad = Input::Instance().GetGamePad();
                if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
                {
                   onClimb = true;
                   position.x = collectPosition.x;
                   position.z = collectPosition.z;
                }

            }
            else
            {
                
                position.x = hit2.position.x;
                position.y = hit.position.y;
                position.z = hit2.position.z;
            }

        }
        else
        {
            onClimb = false;
            position.x += mx;
            position.z += mz;
            position.y += my;
        }
    }
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

