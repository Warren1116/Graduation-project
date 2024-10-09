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

  // ���ŃX�y�[�X�L�[�ŃQ�[���ݒ���N��
  if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
  {
    settingMode = !settingMode;
  }

  // �Q�[���ݒ胂�[�h
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