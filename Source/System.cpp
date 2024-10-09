#include "System.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include <iostream>

System::System()
{

}

System::~System()
{

}

void System::Update(float elapsedTime)
{
  Camera& camera = Camera::Instance();
  GamePad& gamePad = Input::Instance().GetGamePad();

  // 仮でスペースキーでゲーム設定を起動
  if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
  {
    settingMode = !settingMode;
  }

  // ゲーム設定モード
  if (settingMode)
  {
    SetCamera();
  }
}

void System::SetCamera()
{
  std::cin >> nearZ;

  Camera::Instance().SetPerspectiveFov(
    fovY,
    Graphics::Instance().GetScreenWidth() / Graphics::Instance().GetScreenHeight(),
    nearZ,
    farZ);
}