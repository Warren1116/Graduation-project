#pragma once
#include "Camera.h"

class System
{
public:
  System();
  ~System();

  static System& Instance()
  {
    static System instance;
    return instance;
  }

  void Update(float elapsedTime);

  bool GetSettingMode() { return settingMode; }

  void SetCamera();

private:
  float fovY;
  float nearZ;
  float farZ;
  bool settingMode;
};