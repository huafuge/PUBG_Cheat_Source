#pragma once
#include <windef.h>
#include "defines.h"
class Config
{
public:
    Vector2 AimSpeed{};
    float BoxThick = 2.0f, SkeletonThick = 1.0f, Fov = 200.0f, Distance = 600.0f, ItemDist = 55.f,menu = 0.4, AimSmooth = 0.75, AimSleep = 7;
    int RadarDist = 200;
    int Mode = 1, Target = 1, ItemMode = 1;
    bool DeathBox = true, Box = true, Skeleton = true, Item = true, HealthBar = true, DebugMode = false, Prediction = false, Menu = true, LineESP = true, DistanceESP = true, WeaponVisible = false, VehicleESP = false, ESPColor = true, AimVisible = false, PlayerName = true, fightmode = false, SuperJump = false, SpeedHack = false, AimBot = false, YSmooth = false, Spectator = true, MenuEng = true, MenuKR = false, MagicBullet = false, radar = false;
    DWORD AimKey = VK_RBUTTON;
};

Config* cfg = new Config;
