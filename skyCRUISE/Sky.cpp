#include "defines.h"
#include "DriverInterface.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include <string>
#include <tchar.h>
#include "Dwmapi.h"
#include <time.h>
#include <algorithm>
#include "Config.h"
#include <winhttp.h>
#include <urlmon.h>
#include <cstring>
#include <fstream>
#include <timeapi.h>
#include <mmsystem.h>
#include "UGameData.h"
#include <Shlwapi.h>
//#include "C:\Users\Siege\Desktop\//VMProtect_Ultimate_3.4\Include\C\//VMProtectSDK.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Overlay.h"
#include "Images.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "dwmapi.lib")
//#pragma comment(lib, "VMProtectSDK64.lib")
#define wclass "UnrealWindow"
#define wname NULL
using namespace std;
#define M_PI_F		((float)(M_PI))
#define DEG2RAD(x)  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define RAD2DEG(x)  ( (float)(x) * (float)(180.f / M_PI_F) )


extern Vector3 WorldToScreen(Vector3 WorldLocation, FCameraCacheEntry CameraCacheL);
extern D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2);
RECT rc, rc2;
HWND hwnd, gwnd, topw;
LPD3DXFONT P_Font;
ID3DXLine* d3dLine;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

HRGN rgn;
DWM_BLURBEHIND blur;
bool CreateDeviceD3D(HWND hWnd), menu = true;
void CleanupDeviceD3D();
void ResetDevice();
HWND nwnd;
MSG msg;
int WarningCount = 0;
int Width = GetSystemMetrics(SM_CXSCREEN), Height = GetSystemMetrics(SM_CYSCREEN), teamid, myteamid, d;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

float BoxThick1 = 1.0f, SkeletonThick1 = 1.0f, AimbotSpeed1 = 5.0f, AimbotSleep1 = 1.5f;
bool BoxOnOff = true, SkeletonOnOff = true, ItemOnOff = true, VehicleOnOff = true, AimbotOnOff = true, bResult = false;
float BoxColorS[] = { 0.00f, 1.00f, 0.00f, 1.00f };
float BoxColorB[] = { 0.00f, 1.00f, 0.00f, 1.00f };
float SkeletonColorS[] = { 0.00f, 1.00f, 0.00f, 1.00f };
float SkeletonColorB[] = { 0.00f, 1.00f, 0.00f, 1.00f };
float Dist200[] = { 1.00f, 0.00f, 0.00f, 1.00f };
float Dist400[] = { 0.43f, 1.00f, 0.00f, 1.00f };
float Dist600[] = { 0.00f, 0.00f, 1.00f, 1.00f };
float VehicleColor1[] = { 0.00f, 1.00f, 1.00f, 1.00f };
float Equipments[] = { 0.00f, 0.00f, 1.00f, 1.00f };
float Weapons[] = { 1.00f, 0.00f, 0.00f, 1.00f };
float Attachments[] = { 0.13f, 1.00f, 0.00f, 1.00f };
float Medical[] = { 0.97f, 1.00f, 0.00f, 1.00f };
float Scopes[] = { 0.90f, 0.00f, 1.00f, 1.00f };

wstring get_utf16(const string& str, int codepage)
{
    if (str.empty()) return wstring();
    int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
    wstring res(sz, 0);
    MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

static const int B64index[256] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
    0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
    0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

D3DCOLOR F2C(float Col[])
{
    ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
    float r = (col32_no_alpha >> 24) & 255;
    float g = (col32_no_alpha >> 16) & 255;
    float b = (col32_no_alpha >> 8) & 255;
    float a = col32_no_alpha & 255;
    return D3DCOLOR_RGBA((int)(Col[0] * 255.f), (int)(Col[1] * 255.f), (int)(Col[2] * 255.f), (int)(Col[3] * 255.f));
}

const string b64decode(const void* data, const size_t& len)
{
    //VMProtectBeginUltra("Base64");
    if (len == 0) return "";

    unsigned char* p = (unsigned char*)data;
    size_t j = 0,
        pad1 = len % 4 || p[len - 1] == '=',
        pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
    const size_t last = (len - pad1) / 4 << 2;
    string result(last / 4 * 3 + pad1 + pad2, '\0');
    unsigned char* str = (unsigned char*)&result[0];

    for (size_t i = 0; i < last; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad1)
    {
        int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
        str[j++] = n >> 16;
        if (pad2)
        {
            n |= B64index[p[last + 2]] << 6;
            str[j++] = n >> 8 & 0xFF;
        }
    }
    return result;
    //VMProtectEnd();
}

string b64decode(const string& str64)
{
    return b64decode(str64.c_str(), str64.size());
}

string encryptDecrypt(string toEncrypt) {
    char key = 'K';
    string output = toEncrypt;
    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key;
    return output;
}

std::string get_system_uuid()
{
    if (std::system("wmic csproduct get uuid > HWID.txt") == 0)
    {
        auto file = ::fopen("HWID.txt", "rt, ccs=UNICODE"); // open the file for unicode input

        enum { BUFFSZ = 1000, UUID_SZ = 36 };
        wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters

        if (file && // file was succesffully opened
            ::fgetws(wbuffer, BUFFSZ, file) && // successfully read (and discarded) the first line
            ::fgetws(wbuffer, BUFFSZ, file)) // yfully read the second line
        {
            char cstr[BUFFSZ]; // buffer to hold the converted c-style string
            if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ) // convert unicode to utf-8
            {
                std::string uuid = cstr;
                while (!uuid.empty() && std::isspace(uuid.back()))
                    uuid.pop_back(); // discard trailing white space
                return uuid;
            }
        }
    }
    return {}; // failed, return empty string
}

int NearCount = 0;

//Draw->AddImage(img, ImVec2(100, 100), ImVec2(244, 300));

void InitImages() {
    //    AKM, M416, M16A4, SCAL, G36C, UMP, AUG, Groza, AWM, Mini14, SKS, QBZ, M249, MK47, Beryl, QBU, VectorGun,
    //    Kar98K, M24, VSS,
    //    Armor2, Armor3, Head2, Head3, Back2, Back3,
    //    FirstAid, MedKit, EnergyDrink, PainKiller, Adrenaline,
    //    Scope3x, Scope4x, Scope6x, Scope8x, ExtendedQuickDraw, SniperExtendedQuickDraw, Ammo_556, Ammo_762, Ammo_300,
    //    Suppressor, Stock, Compensator, SniperSuppressor, RedDot, HoloGram,
    //    Grenade, SmokeBomb, FlareGun, ProjGrenade, AirDropImg;

    /////////////////////////////////////////////// AirDrop ///////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &AirDropSprite, sizeof(AirDropSprite), &AirDropImg);

    /////////////////////////////////////////////// AR ////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &AKMSprite, sizeof(AKMSprite), &AKM);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &M416Sprite, sizeof(M416Sprite), &M416);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &SCALSprite, sizeof(SCALSprite), &SCAL);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &M16A4Sprite, sizeof(M16A4Sprite), &M16A4);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &G36CSprite, sizeof(G36CSprite), &G36C);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &UMPSprite, sizeof(UMPSprite), &UMP);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &AUGSprite, sizeof(AUGSprite), &AUG);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &GrozaSprite, sizeof(GrozaSprite), &Groza);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &AWMSprite, sizeof(AWMSprite), &AWM);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Mini14Sprite, sizeof(Mini14Sprite), &Mini14);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &SKSSprite, sizeof(SKSSprite), &SKS);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &QBZSprite, sizeof(QBZSprite), &QBZ);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &M249Sprite, sizeof(M249Sprite), &M249);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &MK47Sprite, sizeof(MK47Sprite), &MK47);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &BerylSprite, sizeof(BerylSprite), &Beryl);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &QBUSprite, sizeof(QBUSprite), &QBU);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &VectorGunSprite, sizeof(VectorGunSprite), &VectorGun);

    ////////////////////////////////////////////////// SR //////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Kar98KSprite, sizeof(Kar98KSprite), &Kar98K);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &M24Sprite, sizeof(M24Sprite), &M24);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &VSSSprite, sizeof(VSSSprite), &VSS);

    ///////////////////////////////////////////////// EquipMents ////////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Armor2Sprite, sizeof(Armor2Sprite), &Armor2);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Armor3Sprite, sizeof(Armor3Sprite), &Armor3);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Head2Sprite, sizeof(Head2Sprite), &Head2);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Head3Sprite, sizeof(Head3Sprite), &Head3);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Back2Sprite, sizeof(Back2Sprite), &Back2);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Back3Sprite, sizeof(Back3Sprite), &Back3);

    ///////////////////////////////////////////////// Heal ////////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &FirstAidSprite, sizeof(FirstAidSprite), &FirstAid);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &MedKitSprite, sizeof(MedKitSprite), &MedKit);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &EnergyDrinkSprite, sizeof(EnergyDrinkSprite), &EnergyDrink);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &PainKillerSprite, sizeof(PainKillerSprite), &PainKiller);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &AdrenalineSprite, sizeof(AdrenalineSprite), &Adrenaline);

    ///////////////////////////////////////////////// WeaponParts //////////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Scope3xSprite, sizeof(Scope3xSprite), &Scope3x);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Scope4xSprite, sizeof(Scope4xSprite), &Scope4x);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Scope6xSprite, sizeof(Scope6xSprite), &Scope6x);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Scope8xSprite, sizeof(Scope8xSprite), &Scope8x);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &ExtendedQuickDrawSprite, sizeof(ExtendedQuickDrawSprite), &ExtendedQuickDraw);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &SniperExtendedQuickDrawSprite, sizeof(SniperExtendedQuickDrawSprite), &SniperExtendedQuickDraw);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &SuppressorSprite, sizeof(SuppressorSprite), &Suppressor);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &StockSprite, sizeof(StockSprite), &Stock);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &CompensatorSprite, sizeof(CompensatorSprite), &Compensator);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &RedDotSprite, sizeof(RedDotSprite), &RedDot);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &HoloGramSprite, sizeof(HoloGramSprite), &HoloGram);
    
    ////////////////////////////////////////////////// Ammo //////////////////////////////////////////////////
        
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Ammo_556Sprite, sizeof(Ammo_556Sprite), &Ammo_556);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Ammo_762Sprite, sizeof(Ammo_762Sprite), &Ammo_762);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &Ammo_300Sprite, sizeof(Ammo_300Sprite), &Ammo_300);

    /////////////////////////////////////////////////// Grenade ///////////////////////////////////////////////

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &GrenadeSprite, sizeof(GrenadeSprite), &Grenade);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &SmokeBombSprite, sizeof(SmokeBombSprite), &SmokeBomb);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &FlareGunSprite, sizeof(FlareGunSprite), &FlareGun);
}
int ReturnDist();
void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
{
    ImDrawList* Draw = ImGui::GetOverlayDrawList();
    float scale = (bot.y - top.y) / 3.0f;
    float length = scale / 2;
    Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 1.0f); //  --- Top left
    Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 1.0f); // | Top left
    Draw->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 1.0f); // --- Top right
    Draw->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 1.0f); // | Top right
    Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 1.0f); // --- Bottom left
    Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 1.0f); // | Bottom left
    Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 1.0f); // --- Bottom right
    Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 1.0f); // | Bottom right
}

int ReturnDist() {
    float closest_dist = FLT_MAX;
    int cloest_index = -1;

    if (!GameData::Actors.size())
        return 0;

    Vector3 local_pos = GameData::Camera.POV.Location;

    for (int i = 0; i < GameData::Actors.size(); i++)
    {
        const auto ent = GameData::Actors[i].AActor_Base;
        if (GameData::Actors[i].TeamNum == GameData::mynumber || !GameData::Actors[i].Health > 0.0)
            continue;

        Vector3 ent_pos = GameData::Actors[i].Location;
        const auto dist = ent_pos.Distance(local_pos);

        if (dist < closest_dist)
        {
            closest_dist = dist;
            cloest_index = i;
        }
    }
    return cloest_index;
}

void DrawScense() { // ESP
    ImVec2 DeathDropTitle = ImGui::CalcTextSize("| DeathDrop");
    ImDrawList* Draw = ImGui::GetOverlayDrawList();
    ImU32 cColor, dColor, bColor, VehicleColor;
    if (cfg->ESPColor) {
        cColor = ImGui::GetColorU32(ImVec4(0.843, 1, 0.29, 1));
        dColor = ImGui::GetColorU32(ImVec4(1, 1, 1, 1));
        bColor = ImGui::GetColorU32(ImVec4(0.662, 0.952, 0.9, 1));
        VehicleColor = ImGui::GetColorU32(ImVec4(0, 1, 1, 1));
    }
    else {
        cColor = ImGui::GetColorU32(ImVec4(0, 1, 0, 1));
        dColor = ImGui::GetColorU32(ImVec4(1, 1, 1, 1));
        bColor = ImGui::GetColorU32(ImVec4(1, 0, 0, 1));
        VehicleColor = ImGui::GetColorU32(ImVec4(0, 1, 1, 1));
    }
    FCameraCacheEntry DrawCamera;
    if (&GameData::Camera != NULL)
        DrawCamera = GameData::Camera;
    uint64_t PlayerPawn;
    uint64_t LocalMesh;
    for (int i = 0; i < GameData::Actors.size(); i++)
    {
        if (GameData::Actors[i].AActor_Base != NULL)
            PlayerPawn = GameData::Actors[i].AActor_Base;
        ImVec2 HeadDist = ImGui::CalcTextSize(("[ " + to_string((int)GameData::Actors[i].Distance) + " ]").c_str());
        int MyRenderTime;
        Vector3 W2S;
        LocalMesh = GameData::LocalPlayer_Pawn_Mesh;
        Vector3 localhead = GameData::GetBoneWithRotation(LocalMesh, 15);
        if (PlayerPawn != NULL)
            GameData::Actors[i].RenderTime = GameData::getLastRenderTime(PlayerPawn);
        MyRenderTime = GameData::getLastRenderTime(GameData::LocalPlayer_Pawn);
        if (&GameData::Actors[i].Location != NULL) {
            W2S = WorldToScreen(GameData::Actors[i].Location, DrawCamera);
        }

        if (cfg->DebugMode) {
            Draw->AddText(ImVec2(W2S.x, W2S.y), cColor, to_string(GameData::Actors[i].ActorID).c_str());
        }

        int distance;
        if ((int)GameData::Actors[i].Distance >= 0 && (int)GameData::Actors[i].Distance <= 16000)
            distance = (int)GameData::Actors[i].Distance;

        for (int i = 0; i < GameData::Actors.size(); i++)
        {
            if (GameData::Actors[i].Distance <= 200.0f && GameData::Actors[i].TeamNum != GameData::mynumber)
                NearCount++;
        }

        if (cfg->Spectator) // 관전자
        {
            Draw->AddText(ImVec2(Width / 2, 0), F2C(VehicleColor1), ("Spectator[" + to_string(GameData::SpectatedCount) + "]").c_str());
        }
        int distid = ReturnDist();

        if (GameData::Actors[distid].Distance != NULL)
            GameData::MinDist = GameData::Actors[distid].Distance;

        if (GameData::Actors[i].TeamNum != GameData::mynumber && PlayerPawn != NULL) {
            Vector3 W2S = WorldToScreen(GameData::Actors[i].Location, DrawCamera);
            float Health = GameData::Actors[i].Health;
            ImVec2 HealthSize = ImGui::CalcTextSize((to_string(Health) + "/100").c_str());
            ImVec2 DistSize = ImGui::CalcTextSize((to_string((int)round(distance)) + "M").c_str());

            Vector3 w2s_1 = WorldToScreen(Vector3(GameData::Actors[i].Skeleton.ForeHead.x, GameData::Actors[i].Skeleton.ForeHead.y, GameData::Actors[i].Skeleton.ForeHead.z + 20.0f), DrawCamera);
            Vector3 boxtop = WorldToScreen(Vector3(GameData::Actors[i].Skeleton.pelvis.x, GameData::Actors[i].Skeleton.pelvis.y, GameData::Actors[i].Skeleton.pelvis.z + 90.0f), DrawCamera);
            Vector3 boxbot = WorldToScreen(GameData::Actors[i].Skeleton.root, DrawCamera);
            Vector3 vforehead, vneck, vupperarm_l, varm_l, vhand_l, vupperarm_r, varm_r, vhand_r, vspine1, vspine2, vpelvis, vthigh_l, vcalf_l, vfoot_l, vthigh_r, vcalf_r, vfoot_r, vroot;

            if (GameData::Actors[i].Type == AirDrop && W2S.x < Width && W2S.x > 0 && W2S.y < Width && W2S.y > 0) { // 보급
                Draw->AddImage(AirDropImg, ImVec2(W2S.x - 36, W2S.y - 50), ImVec2(W2S.x + 36, W2S.y + 50));
                Draw->AddText(ImVec2(W2S.x - (HeadDist.x / 2), W2S.y), F2C(VehicleColor1), ("[ " + to_string((int)distance) + " ]").c_str());
            }

            if (GameData::Actors[i].Type == Grenades && W2S.x < Width && W2S.x > 0 && W2S.y < Width && W2S.y > 0) { // 아이템
                Draw->AddImage(AirDropImg, ImVec2(W2S.x - 16, W2S.y - 16), ImVec2(W2S.x + 16, W2S.y + 16));
                Draw->AddText(ImVec2(W2S.x - (HeadDist.x / 2), W2S.y), F2C(VehicleColor1), ("[ " + to_string((int)distance) + " ]").c_str());
            }

            if (W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0) {

                vforehead = WorldToScreen(GameData::Actors[i].Skeleton.ForeHead, DrawCamera);
                vneck = WorldToScreen(GameData::Actors[i].Skeleton.Neck, DrawCamera);

                vupperarm_l = WorldToScreen(GameData::Actors[i].Skeleton.upperarm_l, DrawCamera);
                varm_l = WorldToScreen(GameData::Actors[i].Skeleton.arm_l, DrawCamera);
                vhand_l = WorldToScreen(GameData::Actors[i].Skeleton.hand_l, DrawCamera);

                vupperarm_r = WorldToScreen(GameData::Actors[i].Skeleton.upperarm_r, DrawCamera);
                varm_r = WorldToScreen(GameData::Actors[i].Skeleton.arm_r, DrawCamera);
                vhand_r = WorldToScreen(GameData::Actors[i].Skeleton.hand_r, DrawCamera);

                vspine1 = WorldToScreen(GameData::Actors[i].Skeleton.Spine1, DrawCamera);
                vspine2 = WorldToScreen(GameData::Actors[i].Skeleton.Spine2, DrawCamera);
                vpelvis = WorldToScreen(GameData::Actors[i].Skeleton.pelvis, DrawCamera);

                vthigh_l = WorldToScreen(GameData::Actors[i].Skeleton.thigh_l, DrawCamera);
                vcalf_l = WorldToScreen(GameData::Actors[i].Skeleton.calf_l, DrawCamera);
                vfoot_l = WorldToScreen(GameData::Actors[i].Skeleton.foot_l, DrawCamera);

                vthigh_r = WorldToScreen(GameData::Actors[i].Skeleton.thigh_r, DrawCamera);
                vcalf_r = WorldToScreen(GameData::Actors[i].Skeleton.calf_r, DrawCamera);
                vfoot_r = WorldToScreen(GameData::Actors[i].Skeleton.foot_r, DrawCamera);

                vroot = WorldToScreen(GameData::Actors[i].Skeleton.root, DrawCamera);
            }

            ImU32 DistColor;

            ImVec2 CalcedPlayerName = ImGui::CalcTextSize(("Team : " + to_string(GameData::Actors[i].TeamNum)).c_str());
            if (distance < cfg->Distance && Health > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && cfg->PlayerName) {
                if (distance <= 600)
                    DistColor = F2C(Dist600);
                else if (distance <= 400)
                    DistColor = F2C(Dist400);
                else if (distance <= 200)
                    DistColor = F2C(Dist200);
                Draw->AddText(ImVec2(vpelvis.x - (CalcedPlayerName.x / 2), vroot.y - 10), DistColor, ("Team : " + to_string(GameData::Actors[i].TeamNum)).c_str());
            }
            if (distance < cfg->Distance && Health > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && cfg->HealthBar) { // 체력바 + 팀번호
                Draw->AddRectFilled(ImVec2(vpelvis.x - 21, vroot.y + 3), ImVec2(vpelvis.x + 21, vroot.y + 17), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));
                Draw->AddRectFilled(ImVec2(vpelvis.x - 20, vroot.y + 4), ImVec2(vpelvis.x + 20, vroot.y + 16), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));
                Draw->AddRectFilled(ImVec2(vpelvis.x - 20, vroot.y + 4), ImVec2(vpelvis.x - 20 + (Health * 0.4), vroot.y + 16), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)));
            }
            ImVec2 CalcedDistance = ImGui::CalcTextSize(to_string((int)distance).c_str());
            if (distance < cfg->Distance && Health > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && cfg->DistanceESP) {
                Draw->AddText(ImVec2(vpelvis.x - (CalcedDistance.x / 2), vroot.y + 4), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), to_string((int)distance).c_str());
            }

            if (GameData::aimindex != -1) {
                Vector3 Target, HeadPos, NeckPos;

                if (cfg->Target == 1)
                    Target = GameData::Actors[GameData::aimindex].Skeleton.ForeHead;
                else if (cfg->Target == 2)
                    Target = GameData::Actors[GameData::aimindex].Skeleton.Neck;
                else if (cfg->Target == 3)
                    Target = GameData::Actors[GameData::aimindex].Skeleton.Spine1;
                if (GetAsyncKeyState(VK_SHIFT))
                    Target = GameData::Actors[GameData::aimindex].Skeleton.ForeHead;

                HeadPos = WorldToScreen(GameData::Actors[GameData::aimindex].Skeleton.ForeHead, GameData::Camera);
                NeckPos = WorldToScreen(GameData::Actors[GameData::aimindex].Skeleton.Neck, GameData::Camera);

                float Length = HeadPos.Distance(NeckPos);

                Vector3 W2S = WorldToScreen(Target, GameData::Camera);

                Draw->AddRect(ImVec2(W2S.x - Length, W2S.y - Length), ImVec2(W2S.x + Length, W2S.y + Length), ImGui::GetColorU32(ImVec4(0, 1, 1, 1)), 0.0f, 15, 1.0f);
            }

            ImU32 SkelCS = F2C(SkeletonColorS);
            ImU32 SkelCB = F2C(SkeletonColorB);

            if (GameData::Actors[i].RenderTime == MyRenderTime && distance < cfg->Distance && Health > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && &SkeletonOnOff) { // 스켈레톤
                Draw->AddLine(ImVec2(vforehead.x, vforehead.y), ImVec2(vneck.x, vneck.y), SkelCS, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_l.x, vupperarm_l.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_l.x, vupperarm_l.y), ImVec2(varm_l.x, varm_l.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(varm_l.x, varm_l.y), ImVec2(vhand_l.x, vhand_l.y), SkelCS, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_r.x, vupperarm_r.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_r.x, vupperarm_r.y), ImVec2(varm_r.x, varm_r.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(varm_r.x, varm_r.y), ImVec2(vhand_r.x, vhand_r.y), SkelCS, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vspine2.x, vspine2.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vspine2.x, vspine2.y), ImVec2(vspine1.x, vspine1.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vspine1.x, vspine1.y), ImVec2(vpelvis.x, vpelvis.y), SkelCS, SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_l.x, vthigh_l.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_l.x, vthigh_l.y), ImVec2(vcalf_l.x, vcalf_l.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_l.x, vcalf_l.y), ImVec2(vfoot_l.x, vfoot_l.y), SkelCS, SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_r.x, vthigh_r.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_r.x, vthigh_r.y), ImVec2(vcalf_r.x, vcalf_r.y), SkelCS, SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_r.x, vcalf_r.y), ImVec2(vfoot_r.x, vfoot_r.y), SkelCS, SkeletonThick1);
            }
            else if (GameData::Actors[i].RenderTime != MyRenderTime && distance < cfg->Distance && Health > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && &SkeletonOnOff) {
                Draw->AddLine(ImVec2(vforehead.x, vforehead.y), ImVec2(vneck.x, vneck.y), SkelCB, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_l.x, vupperarm_l.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_l.x, vupperarm_l.y), ImVec2(varm_l.x, varm_l.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(varm_l.x, varm_l.y), ImVec2(vhand_l.x, vhand_l.y), SkelCB, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_r.x, vupperarm_r.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_r.x, vupperarm_r.y), ImVec2(varm_r.x, varm_r.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(varm_r.x, varm_r.y), ImVec2(vhand_r.x, vhand_r.y), SkelCB, SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vspine2.x, vspine2.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vspine2.x, vspine2.y), ImVec2(vspine1.x, vspine1.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vspine1.x, vspine1.y), ImVec2(vpelvis.x, vpelvis.y), SkelCB, SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_l.x, vthigh_l.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_l.x, vthigh_l.y), ImVec2(vcalf_l.x, vcalf_l.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_l.x, vcalf_l.y), ImVec2(vfoot_l.x, vfoot_l.y), SkelCB, SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_r.x, vthigh_r.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_r.x, vthigh_r.y), ImVec2(vcalf_r.x, vcalf_r.y), SkelCB, SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_r.x, vcalf_r.y), ImVec2(vfoot_r.x, vfoot_r.y), SkelCB, SkeletonThick1);
            }
            else if (distance < cfg->Distance && GameData::Actors[i].GroggyHealth > 0.0 && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0 && &SkeletonOnOff) {
                Draw->AddLine(ImVec2(vforehead.x, vforehead.y), ImVec2(vneck.x, vneck.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_l.x, vupperarm_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_l.x, vupperarm_l.y), ImVec2(varm_l.x, varm_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(varm_l.x, varm_l.y), ImVec2(vhand_l.x, vhand_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vupperarm_r.x, vupperarm_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vupperarm_r.x, vupperarm_r.y), ImVec2(varm_r.x, varm_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(varm_r.x, varm_r.y), ImVec2(vhand_r.x, vhand_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);

                Draw->AddLine(ImVec2(vneck.x, vneck.y), ImVec2(vspine2.x, vspine2.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vspine2.x, vspine2.y), ImVec2(vspine1.x, vspine1.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vspine1.x, vspine1.y), ImVec2(vpelvis.x, vpelvis.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_l.x, vthigh_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_l.x, vthigh_l.y), ImVec2(vcalf_l.x, vcalf_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_l.x, vcalf_l.y), ImVec2(vfoot_l.x, vfoot_l.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);

                Draw->AddLine(ImVec2(vpelvis.x, vpelvis.y), ImVec2(vthigh_r.x, vthigh_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vthigh_r.x, vthigh_r.y), ImVec2(vcalf_r.x, vcalf_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
                Draw->AddLine(ImVec2(vcalf_r.x, vcalf_r.y), ImVec2(vfoot_r.x, vfoot_r.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), SkeletonThick1);
            }
            if (&BoxOnOff && distance < cfg->Distance && Health > 0.0) {
                if (GameData::Actors[i].RenderTime == MyRenderTime && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0)
                    DrawEdges(ImVec2(boxtop.x, boxtop.y), ImVec2(boxbot.x, boxbot.y), ImVec2(vpelvis.x, 0), F2C(BoxColorS));
                else if (GameData::Actors[i].RenderTime != MyRenderTime && W2S.x < Width && W2S.x > 0 && W2S.y < Height && W2S.y > 0)
                    DrawEdges(ImVec2(boxtop.x, boxtop.y), ImVec2(boxbot.x, boxbot.y), ImVec2(vpelvis.x, 0), F2C(BoxColorB));
            }
        }
        if (GameData::Actors[i].Type == Vehicles && PlayerPawn != NULL && cfg->VehicleESP && !cfg->fightmode) {

        }
        //if (GameData::Actors[i].Type == DeathBox && PlayerPawn != NULL && cfg->DeathBox && distance < cfg->ItemDist) // 시체박스
        //{
        //    Vector3 ScPos = WorldToScreen(GameData::Actors[i].Location, DrawCamera);
        //    uint64_t DeathDropItems = mem->RVM<uint64_t>(PlayerPawn + offs_ITEM_PACKAGE);
        //    int DeathDropItemCount = mem->RVM<int>(PlayerPawn + offs_ITEM_PACKAGE + 0x8);
        //    if (DeathDropItemCount > 0 && DeathDropItems != NULL)
        //    {
        //        ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)), u8"| DeathDrop");
        //        ScPos.y += DeathDropTitle.y;
        //        for (int i = 0; i < DeathDropItemCount; i++)
        //        {
        //            uint64_t Item = mem->RVM<uint64_t>(DeathDropItems + (i * 0x8));
        //            int ItemID = mem->RVM<int>(Item + offs_UITEM_ID);
        //            std::string ItemName = GameData::GetItemNameByID(ItemID);
        //            if (ItemName != "NULL" && Item != NULL)
        //            {
        //                if (ItemName == "AKM" || ItemName == "M16A4" || ItemName == "SCAL-L" || ItemName == "G36-C" || ItemName == "UMP" || ItemName == "Mini14" || ItemName == "QBZ" || ItemName == "M249" || ItemName == "MK47" || ItemName == "BerylM762" || ItemName == "QBU" || ItemName == "Vector" || ItemName == "VSS")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "M416" || ItemName == "AUG" || ItemName == "Groza" || ItemName == "SKS" || ItemName == "Kar98K" || ItemName == "M24" || ItemName == "AWM")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(0.5, 0, 1, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "FlareGun" || ItemName == "Lv.2 Vest" || ItemName == "Lv.3 Vest" || ItemName == "Lv.2 Helmat" || ItemName == "Lv.3 Helmat" || ItemName == "Lv.3 BackPack" || ItemName == "Suppressor" || ItemName == "Suppressor(SR)" || ItemName == "Chock" || ItemName == "Compensator" || ItemName == "x6 Scope" || ItemName == "x4 Scope" || ItemName == "x8 Scope" || ItemName == "ExtendedQuickDraw" || ItemName == "ExtendedQuickDraw(SR)")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "5.56mm" || ItemName == "7.62mm" || ItemName == "300 Mag")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(1, 1, 0, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "RedDot" || ItemName == "HoloSite" || ItemName == "Grenade" || ItemName == "SmokeBomb" || ItemName == "Lv.2 BackPack" || ItemName == "x3")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "FirstAid" || ItemName == "MediKit")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(1, 0.2, 0.6, 1)), (u8"| " + ItemName).c_str());
        //                else if (ItemName == "EnergyDrink" || ItemName == "PainKiller" || ItemName == "Adrenaline")
        //                    ImGui::GetOverlayDrawList()->AddText(NULL, 13.f, ImVec2(ScPos.x, ScPos.y), ImGui::GetColorU32(ImVec4(0, 0, 0.5, 1)), (u8"| " + ItemName).c_str());
        //                ScPos.y += DeathDropTitle.y;
        //            }
        //        }
        //    }
        //}
        //if (GameData::Actors[i].Type == Item && cfg->Item && !cfg->fightmode && distance < cfg->ItemDist) // 아이템
        //{
        //    uint64_t pItemArray =  mem->RVM<uint64_t>(GameData::Actors[i].AActor_Base + offs_DroppedItemGroup);
        //    int pItemCount= mem->RVM<int>(GameData::Actors[i].AActor_Base + offs_DroppedItemGroup + 0x8);

        //    if (!pItemArray || !pItemCount || pItemCount > 100)
        //        continue;

        //    for (int j = 0; j < pItemCount; j++)
        //    {
        //        uint64_t pItemObject = mem->RVM<uint64_t>(pItemArray + (j * 0x10));
        //        if (pItemObject)
        //        {
        //            uint64_t pUItemAddress = mem->RVM<uint64_t>(pItemObject + offs_DroppedItemGroup_UItem);
        //            if (pUItemAddress)
        //            {
        //                int pUItemID = mem->RVM<int>(pUItemAddress + offs_UITEM_ID);

        //                if (pUItemID > 0 && pUItemID < 399999)
        //                {
        //                    string ItemName = GameData::GetItemNameByID(pUItemID);

        //                    if (ItemName != "NULL")
        //                    {
        //                        Vector3 Pos = mem->RVM<Vector3>(pItemObject + __OFFSET__Location);
        //                        Vector3 ScPos = WorldToScreen(Pos, DrawCamera);
        //                        Pos.z -= 5.0f;
        //                        Vector3 TextPos = WorldToScreen(Pos, DrawCamera);

        //                        if (ScPos.x > 0 && ScPos.x < Width && ScPos.y > 0 && ScPos.y < Height)
        //                        {
        //                            //총기류
        //                            if (ItemName == "AKM") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| AKM\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "M416") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| M416\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "M16A4") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| M16A4\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "SCAL-L") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| SCAL-L\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "G36-C") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| G36C\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "UMP") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| UMP\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "AUG") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| AUG\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "Groza") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| Groza\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "AWM") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| AWM\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "Mini14") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| Mini14\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "SKS") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| SKS\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "QBZ") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| QBZ\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "M249") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| M249\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "MK47") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| MK47\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "BerylM762") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| BerylM762\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "QBU") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| QBU\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "Vector") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| Vector\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "Kar98K") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| Kar98K\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "M24") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| M24\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == "VSS") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| VSS\n| " + to_string(distance)).c_str());
        //                            }

        //                            //장비류
        //                            if (ItemName == u8"Lv.2 Vest") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.2 Vest\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Lv.3 Vest") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.3 Vest\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Lv.2 Helmat") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.2 Helmats\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Lv.3 Helmat") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.3 Helmat\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Lv.2 BackPack") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.2 BackPack\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Lv.3 BackPack") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Equipments[0], Equipments[1], Equipments[2], 1.00f)), ("| Lv.3 BackPack\n| " + to_string(distance)).c_str());
        //                            }

        //                            //힐템류
        //                            if (ItemName == u8"FirstAid") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Medical[0], Medical[1], Medical[2], 1.00f)), ("| FirstAid\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"MediKit") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Medical[0], Medical[1], Medical[2], 1.00f)), ("| MediKit\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"EnergyDrink") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Medical[0], Medical[1], Medical[2], 1.00f)), ("| EnergyDrink\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Painkiller") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Medical[0], Medical[1], Medical[2], 1.00f)), ("| PainKillr\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Adrenaline") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Medical[0], Medical[1], Medical[2], 1.00f)), ("| Adrenaline\n| " + to_string(distance)).c_str());
        //                            }

        //                            //파츠류
        //                            if (ItemName == u8"x3 Scope") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| x3 Scope\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"x4 Scope") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| x4 Scope\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"x6 Scope") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| x6 Scope\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"x8 Scope") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| x8 Scope\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"ExtendedQuickDraw") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| ExtendedQuickDraw\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"ExtendedQuickDraw(SR)") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| ExtendedQuickDraw(SR)\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Suppressor") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| Suppressor\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Suppressor(SR)") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| Suppressor(SR)\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Chock") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| Chock(AR)\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"Compensator") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Attachments[0], Attachments[1], Attachments[2], 1.00f)), ("| Compensator\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"RedDot") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| RedDot\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"HoloSite") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Scopes[0], Scopes[1], Scopes[2], 1.00f)), ("| HoloSite\n| " + to_string(distance)).c_str());
        //                            }

        //                            //수류탄류
        //                            if (ItemName == u8"Grenade") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| Grenade\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"SmokeBomb") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| SmokeBomb\n| " + to_string(distance)).c_str());
        //                            }
        //                            if (ItemName == u8"FlareGun") {
        //                                Draw->AddText(ImVec2(TextPos.x - (HeadDist.x / 2), TextPos.y), ImGui::GetColorU32(ImVec4(Weapons[0], Weapons[1], Weapons[2], 1.00f)), ("| FlareGun\n| " + to_string(distance)).c_str());
        //                            }
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }
        //}
    }
}
#include <tlhelp32.h>
#include "VMP/VMProtectSDK.h"
//#include "VMP/VMProtectSDK.h"

bool SetupWindow(HWND& hWnd) //ESP 창 (메모장후킹)
{
    while (!hWnd) {
        hWnd = HiJackNotepadWindow();
        Sleep(100);
    }

    if (!CreateDeviceD3D(hWnd))
    {
        CleanupDeviceD3D();
        return false;
    }

    MARGINS margin = { 0,0,Width,Height };
    DwmExtendFrameIntoClientArea(hWnd, &margin);

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE Handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    __int64 Alloced = (__int64)VirtualAllocEx(Handle, 0, 512, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    unsigned char shellcode[] = { 0xB9, 0x01, 0x00, 0x00, 0x00, 0xBA, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x15, 0x02, 0x00, 0x00, 0x00, 0xEB, 0x08, 0xA0, 0x3D, 0xF8, 0x19, 0xF9, 0x7F, 0x00, 0x00, 0xC3 };
    HMODULE hUser32 = GetModuleHandleA("User32.dll");
    __int64 pSetWindowDisplayAffinity = (__int64)GetProcAddress(hUser32, "SetWindowDisplayAffinity");

    *(__int64*)(shellcode + sizeof(shellcode) - 9) = pSetWindowDisplayAffinity;
    *(int*)(shellcode + 1) = (int)hwnd;
    *(int*)(shellcode + 6) = 1;
    SIZE_T written;
    WriteProcessMemory(Handle, (LPVOID)Alloced, shellcode, sizeof(shellcode), &written);
    CreateRemoteThread(Handle, 0, 0, (LPTHREAD_START_ROUTINE)Alloced, 0, 0, 0);

    SetMenu(hwnd, NULL);
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

    //SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniSavingRate = 0;
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
}

//bool SetupWindow(HWND& hWnd)
//{
//	WNDCLASSEX overlayWindowClass;
//	ZeroMemory(&overlayWindowClass, sizeof(WNDCLASSEX));
//	overlayWindowClass.cbClsExtra = NULL;
//	overlayWindowClass.cbWndExtra = NULL;
//	overlayWindowClass.cbSize = sizeof(WNDCLASSEX);
//	overlayWindowClass.style = CS_HREDRAW | CS_VREDRAW;
//	overlayWindowClass.lpfnWndProc = WndProc;
//	overlayWindowClass.hInstance = NULL;
//	overlayWindowClass.lpszClassName = "Nvidia";
//	overlayWindowClass.lpszMenuName = "Geforce Expirience";
//	RegisterClassEx(&overlayWindowClass);
//
//
//	hWnd = CreateWindowEx(NULL, "Nvidia", "Geforce Expirience", WS_POPUP , 0, 0, Width, Height, NULL, NULL, NULL, NULL);
//
//	if (!CreateDeviceD3D(hWnd))
//	{
//		CleanupDeviceD3D();
//		UnregisterClass(overlayWindowClass.lpszClassName, overlayWindowClass.hInstance);
//		return false;
//	}
//
//	ShowWindow(hWnd, SW_SHOW);
//	UpdateWindow(hWnd);
//
//    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
//	MARGINS margin = { 0,0,Width,Height };
//	DwmExtendFrameIntoClientArea(hWnd, &margin);
//
//	SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT);
//
//	SetWindowDisplayAffinity(hwnd, WDA_MONITOR);
//
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO();
//	io.IniSavingRate = 0;
//	ImGui_ImplWin32_Init(hwnd);
//	ImGui_ImplDX9_Init(g_pd3dDevice);
//}

bool Draw = true;

DWORD WINAPI UpdateUData(PVOID) { // 인겜 업데이트
    while (1) {
        GameData::UpdateUData();
        GameData::UpdateActors();
        Sleep(5000);
    }
}

const float GravityAcceleration = 9.8, BulletVelocity = GameData::VDragCoefficient;

float GetBulletDrop(float height, float DepthPlayerTarget)
{
    float pitch = (atan2(height, DepthPlayerTarget));
    float BulletVelocityXY = BulletVelocity * cos(pitch);
    float Time = DepthPlayerTarget / BulletVelocityXY;
    float TotalVerticalDrop = (0.5f * GravityAcceleration * Time * Time);
    return TotalVerticalDrop;
}

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

DWORD WINAPI Top(PVOID) { // 화면 맨 첫번째 권한?
    while (1) {
    }
}

int CurrentShotIndex = -1;
int Result = -1;

int GetAimIndex() // 에임봇 로직
{
    if (!GameData::Actors.size())
        return -1;

    Vector3 CrossHair = Vector3(Width / 2.0f, Height / 2.0f, 0);
    float Health;
    int TeamNum, MyNum;
    try{
        TeamNum = GameData::Actors[CurrentShotIndex].TeamNum;
        MyNum = GameData::mynumber;
        Health = GameData::Actors[CurrentShotIndex].Health;
    }
    catch(...){}
    if (CurrentShotIndex != -1 && TeamNum != MyNum && Health > 0.0)
    {
        Vector3 Screen = WorldToScreen(GameData::Actors[CurrentShotIndex].Skeleton.ForeHead, GameData::Camera);

        if (CrossHair.Distance(Screen) <= 300.0f)
            return CurrentShotIndex;
    }
    else
    {
        vector<float> Dist;
        vector<float> RDist;

        for (int i = 0; i < GameData::Actors.size(); i++)
        {
            AActor CurrentActor;
            if (GameData::Actors[i].TeamNum != GameData::mynumber)
                CurrentActor = GameData::Actors[i];

            Vector3 Screen = WorldToScreen(CurrentActor.Skeleton.ForeHead, GameData::Camera);

			if (CrossHair.Distance(Screen) <= 300.0f && CurrentActor.Health > 0.0f)
			{
				Dist.push_back(CrossHair.Distance(Screen));
			}
			else
			{
				Dist.push_back(0xFFFFFFFF);
			}
		}

		int MI = 0;

		for (int i = 0; i < Dist.size(); i++)
		{
            if (Dist[MI] > Dist[i])
            {
                MI = i;
            }
		}

        if (Dist[MI] <= 300.0f)
        {
            CurrentShotIndex = MI;

            Result = MI;
        }
	}
    return Result;
}

bool Move = false;

int AimBot_API(float X, float Y) // 에임봇 마우스이벤트 로직
{
    INPUT InputMouse = { 0 };
    InputMouse.type = INPUT_MOUSE;
    InputMouse.mi.dwFlags = MOUSEEVENTF_MOVE;
    if(abs((LONG)X * cfg->AimSmooth) > 1)
        InputMouse.mi.dx = (LONG)X * (AimbotSpeed1 / 100);
    if (abs((LONG)Y * cfg->AimSmooth) > 1)
        InputMouse.mi.dy = (LONG)Y * (AimbotSpeed1 / 100);
	//InputMouse.mi.dx = (LONG)(X - (X * cfg->AimSmooth)) / (3 + cfg->AimSleep);
	//InputMouse.mi.dy = (LONG)Y / 3;
    //InputMouse.mi.dy = ((LONG)Y - (LONG)Y * cfg->AimSmooth) * (Y / (Y - Y * cfg->AimSleep)) / 2;
    Move = true;
    return SendInput(1, &InputMouse, sizeof(INPUT));
}

float MoveX, MoveY;

FRotator CalcAngle(Vector3 LocalHeadPosition, Vector3 AimPosition)
{
    Vector3 vecDelta = Vector3((LocalHeadPosition.x - AimPosition.x), (LocalHeadPosition.y - AimPosition.y), (LocalHeadPosition.z - AimPosition.z));
    float hyp = (float)sqrt(vecDelta.x * vecDelta.x + vecDelta.y * vecDelta.y);

    FRotator ViewAngles = FRotator();
    ViewAngles.Pitch = -(float)atan(vecDelta.z / hyp) * (float)(180.0f / M_PI);
    ViewAngles.Yaw = (float)atan(vecDelta.y / vecDelta.x) * (float)(180.0f / M_PI);
    ViewAngles.Roll = (float)0.f;

    if (vecDelta.x >= 0.0f)
        ViewAngles.Yaw += 180.0f;

    return ViewAngles;
}

Vector3 ClampAngle(Vector3 angle)
{
    while (angle.y > 180) angle.y -= 360;
    while (angle.y < -180) angle.y += 360;

    if (angle.x > 89.0f) angle.x = 89.0f;
    if (angle.x < -89.0f) angle.x = -89.0f;

    angle.z = 0.0f;

    return angle;
}

Vector3 SafeCalcAngle(Vector3 src, Vector3 dst)
{
    Vector3 vAngle;
    Vector3 delta{ (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

    vAngle.x = (float)(atan((delta.z * -1) / hyp) * 57.295779513082f);
    vAngle.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
    vAngle.z = 0.0f;

    if (delta.x >= 0.0)
        vAngle.y += 180.0f;
    vAngle = ClampAngle(vAngle);
    return vAngle;
}

/*
void SetAngle(float x, float y) {
    if (!GameData::LocalPlayer_Controller)
        return;
    __int64 v3 = GameData::LocalPlayer_Controller;

    mem->WVM<float>(GameData::LocalPlayer_Controller + 0x428, x);
    mem->WVM<float>(GameData::LocalPlayer_Controller + 0x428 + 4, y);

    //*(_QWORD*)(v3 + 0x410) = *(_QWORD*)(v3 + 0x428);
    mem->WVM<_QWORD>(mem->RVM<float>(v3 + 0x410), mem->RVM<float>(v3 + 0x428));
    //*(_DWORD*)(v3 + 0x418) = *(_DWORD*)(v3 + 0x430);
    mem->WVM<_QWORD>(mem->RVM<float>(v3 + 0x418), mem->RVM<float>(v3 + 0x430));

    auto v20 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<float>(v3 + 1064) - 182490878)));
    auto v63 = v20;
    auto v21 = Decrypt_Shield(mem->RVM<float>(v20));
    LODWORD(v20) = mem->RVM<float>(182490878 - (((unsigned __int64)Decrypt_Shield(v20)) ^ 0x32B456D4) ^ __ROL4__(0, v21 & 0x3F));
    auto v22 = Decrypt_Shield(mem->RVM<float>((unsigned int)(mem->RVM<float>(v3 + 1068) - 182490878)));
    auto v64 = v22;
    auto v23 = Decrypt_Shield(mem->RVM<float>(v22));
    mem->WVM<_DWORD>((v3 + 1112), (v20 - (unsigned __int64)Decrypt_Shield(v22) ^ 0x32B456D4) + 182490878 ^ __ROL4__(v20, v23 & 0x3F));
    auto v24 = Decrypt_Shield((unsigned int)(v3 + 1044) - 182490878);
    auto v66 = v24;
    auto v25 = Decrypt_Shield(mem->RVM<float>(v24));
    LODWORD(v24) = mem->RVM<float>((182490878 - ((unsigned __int64)Decrypt_Shield(v24) ^ 0x32B456D4)) ^ __ROL4__(0, v25 & 0x3F));
    auto v26 = Decrypt_Shield(mem->RVM<float>((unsigned int)(mem->RVM<float>(v3 + 1040) - 182490878)));
    auto v67 = v26;
    auto v27 = Decrypt_Shield(mem->RVM<float>(v26));
    mem->WVM<_DWORD>((v3 + 1092), mem->RVM<float>(v24) - mem->RVM<float>(mem->RVM<float>((unsigned __int64)Decrypt_Shield(mem->RVM<float>(v26)) ^ 0x32B456D4)) + 182490878) ^ __ROL4__(v24, v27 & 0x3F);
}
*/

//void SetAngle(float x, float y) {
//    if (!GameData::LocalPlayer_Controller)
//        return;
//    __int64 v3 = GameData::LocalPlayer_Controller;
//
//    mem->WVM<float>(GameData::LocalPlayer_Controller + 0x428, x);
//    mem->WVM<float>(GameData::LocalPlayer_Controller + 0x428 + 4, y);
//
//
//    mem->WVM<_QWORD>(mem->RVM<float>(v3 + 0x410), mem->RVM<float>(v3 + 0x428));
//    //*(_DWORD*)(v3 + 0x418) = *(_DWORD*)(v3 + 0x430);
//    mem->WVM<_QWORD>(mem->RVM<float>(v3 + 0x418), mem->RVM<float>(v3 + 0x430));
//
//    auto v20 = Decrypt_Shield(mem->RVM<float>((unsigned int)(*(_DWORD*)(v3 + 1064) - 182490878)));
//    auto v63 = v20;
//    auto v21 = Decrypt_Shield(v20);
//    LODWORD(v20) = mem->RVM<float>((182490878 - ((unsigned __int64)Decrypt_Shield(v20) ^ 0x32B456D4)) ^ __ROL4__(0, v21 & 0x3F));
//    auto v22 = Decrypt_Shield(mem->RVM<float>((unsigned int)(*(_DWORD*)(v3 + 1068) - 182490878)));
//    auto v64 = v22;
//    auto v23 = Decrypt_Shield(v22);
//    mem->WVM<_DWORD>(*(_DWORD*)(v3 + 1112), mem->RVM<float>((v20 - ((unsigned __int64)Decrypt_Shield(v22) ^ 0x32B456D4) + 182490878) ^ __ROL4__(v20, v23 & 0x3F)));
//    auto v24 = Decrypt_Shield(mem->RVM<float>((unsigned int)(*(_DWORD*)(v3 + 1044) - 182490878)));
//    auto v66 = v24;
//    auto v25 = Decrypt_Shield(v24);
//    LODWORD(v24) = mem->RVM<float>((182490878 - ((unsigned __int64)Decrypt_Shield(v24) ^ 0x32B456D4)) ^ __ROL4__(0, v25 & 0x3F));
//    auto v26 = Decrypt_Shield(mem->RVM<float>((unsigned int)(*(_DWORD*)(v3 + 1040) - 182490878)));
//    auto v67 = v26;
//    auto v27 = Decrypt_Shield(v26);
//    mem->WVM<_DWORD>(*(_DWORD*)(v3 + 1092), (v24 - ((unsigned __int64)Decrypt_Shield(v26) ^ 0x32B456D4) + 182490878) ^ __ROL4__(v24, v27 & 0x3F));
//}

//void AimAtPosAngles(Vector3 TargetLocation)
//{
//    Vector3 viewAngles = SafeCalcAngle(GameData::AimCamera.POV.Location, TargetLocation);
//
//    SetAngle((float)viewAngles.x, (float)viewAngles.y);
//}

//void AimToTarget()
//{
//
//    // nosway (should be always on, not jsut when aimbotting)
//    /*if (G::localWeapon)
//    {
//        auto sway = G::localWeapon->GetSwayModifier();
//        G::localRot.Yaw -= sway.X;
//        G::localRot.Pitch -= sway.Y;
//    }*/
//
//    SetViewAngles(G::localRot + idealAngDelta * 0.4f);
//}

DWORD WINAPI AimThread(PVOID) { // 에임봇 쓰레드
    while (1)
    {
        int Index = GetAimIndex();
        if (!GetAsyncKeyState(VK_RBUTTON)) {
            CurrentShotIndex == -1;
            GameData::aimindex = -1;
        }
        if (GetAsyncKeyState(VK_RBUTTON) && cfg->AimBot && (int)GameData::CurrentWeaponIndex <= 2 && (int)GameData::CurrentWeaponIndex >= 0) {
            int dis;
            GameData::aimindex = Index;
            float AimSleep = 1;
            float bulletSpeed = GameData::BulletSpeedVal;
            float VDrag = GameData::VDragCoefficient;
            Vector3 CrossHair = Vector3(Width / 2.0f, Height / 2.0f, 0);
            int XAimSleep, YAimSleep, recoil;
            if (Index != -1 && GameData::Actors[Index].Health > 0.0 && GameData::Actors[Index].TeamNum != GameData::mynumber && GameData::Actors[Index].Distance < 600)
            {
                uint64_t MeshComp, RootComponent, ActorBase, LocalMesh;
                LocalMesh = GameData::LocalPlayer_Pawn_Mesh;      
                if (GameData::Actors[Index].MeshComponent != NULL)
                    MeshComp = GameData::Actors[Index].MeshComponent;
                if (GameData::Actors[Index].RootComponent != NULL)
                    RootComponent = GameData::Actors[Index].RootComponent;
                if (GameData::Actors[Index].AActor_Base != NULL)
                    ActorBase = GameData::Actors[Index].AActor_Base;
                Skeleton_Vec3 Skeleton;
                Vector3 Head, Neck, Spine;
                Vector3 HeadB = GameData::GetBoneWithRotation(MeshComp, Bones::forehead);
                Vector3 NeckB = GameData::GetBoneWithRotation(MeshComp, Bones::neck_01);
                Vector3 SpineB = GameData::GetBoneWithRotation(MeshComp, Bones::spine_01);
                if (&HeadB != NULL)
                    Head = HeadB;
                if (&NeckB != NULL)
                    Neck = NeckB;
                if (&SpineB != NULL)
                    Spine = SpineB;
                GameData::Actors[Index].Velocity = mem->RVM<Vector3>(RootComponent + __OFFSET__ActorVelocity);
                dis = Head.Distance(GameData::Camera.POV.Location) / 100.0f;
                Vector3 Target, Target2;
                Vector3 localhead = GameData::GetBoneWithRotation(LocalMesh, Bones::forehead);
                float dist = HeadB.Distance(localhead);
                Vector3 Velocity = GameData::Actors[Index].Velocity;
                float flytime = GameData::Actors[Index].Distance / bulletSpeed;
                Head.x = Head.x + (Velocity.x * flytime);
                Head.y = Head.y + (Velocity.y * flytime);
                Head.z = Head.z + (Velocity.z * flytime);
                Neck.x = Neck.x + (Velocity.x * flytime);
                Neck.y = Neck.y + (Velocity.y * flytime);
                Neck.z = Neck.z + (Velocity.z * flytime);
                Spine.x = Spine.x + (Velocity.x * flytime);
                Spine.y = Spine.y + (Velocity.y * flytime);
                Spine.z = Spine.z + (Velocity.z * flytime);
                //float height = GameData::Actors[Index].Location.z - GameData::Camera.POV.Location.z;
                //float DepthPlayerTarget = sqrt(pow(GameData::Actors[Index].Location.y - GameData::Camera.POV.Location.y, 2) + pow(GameData::Actors[Index].Location.x - GameData::Camera.POV.Location.x, 2));
                //Head.z += GetBulletDrop(height, DepthPlayerTarget);
                //Neck.z += GetBulletDrop(height, DepthPlayerTarget);
                //GameData::UpdateCamera();
                GameData::UpdateSway();

                FCameraCacheEntry AimbotCamera;
                if (&GameData::AimCamera != NULL)
                    AimbotCamera = GameData::AimCamera;
                Vector3 delta = WorldToScreen(HeadB, GameData::Camera);
                Vector3 delta1 = WorldToScreen(Head, AimbotCamera);
                Vector3 delta2 = WorldToScreen(Neck, AimbotCamera);
                Vector3 delta3 = WorldToScreen(Spine, AimbotCamera);
                Vector3 Screen, Angle;
                Vector3 HeadA = Vector3(localhead.x, localhead.y, localhead.z + 30);
                if (cfg->Target == 1) {
                    Angle = Head;
                    Target = delta1 - CrossHair;
                    Target2 = Head - CrossHair;
                    Screen = delta1;
                }
                else if (cfg->Target == 2) {
                    Angle = Neck;
                    Target = delta2 - CrossHair;
                    Target2 = Head - CrossHair;
                    Screen = delta2;
                }
                else if (cfg->Target == 3) {
                    Angle = Spine;
                    Target = delta3 - CrossHair;
                    Target2 = Head - CrossHair;
                    Screen = delta3;
                }

                if (GetAsyncKeyState(VK_SHIFT)) {
                    Angle = Head;
                    Target = delta1 - CrossHair;
                    Target2 = Head - CrossHair;
                    Screen = delta1;
                }

                float Health = 1.0f;
                int EnemyTime = 1, LocalTime = 1;
                if (GameData::Actors[Index].Health != NULL)
                    Health = GameData::Actors[Index].Health;
                if (GameData::Actors[Index].AActor_Base != NULL)
                    EnemyTime = GameData::getLastRenderTime(ActorBase);
                if (GameData::LocalPlayer_Pawn != NULL)
                    LocalTime = GameData::getLastRenderTime(GameData::LocalPlayer_Pawn);

                //cout << Target.x << " | " << Target.y << endl;

                if (Health > 0.0 && CrossHair.Distance(Screen) <= cfg->Fov && EnemyTime == LocalTime) {
                    MoveX = Target.x;
                    MoveY = Target.y;
                    AimBot_API(MoveX * cfg->AimSpeed.x, MoveY * cfg->AimSpeed.y);
                }
                else
                    CurrentShotIndex = -1;
            }
            Sleep(5);
        }
    }
}

//void SetAngle(float x, float y) {
//    if (!GameData::LocalPlayer_Controller)
//        return;
//    __int64 v3 = GameData::LocalPlayer_Controller;
//
//    mem->WVM<float>(mem->RVM<uint64_t>(GameData::LocalPlayer_Controller + 0x428 + 4), x);
//    mem->WVM<float>(mem->RVM<uint64_t>(GameData::LocalPlayer_Controller + 0x428), y);

    ////*(_QWORD*)(v3 + 0x410) = *(_QWORD*)(v3 + 0x428);
    //mem->WVM<_QWORD>(mem->RVM<uint64_t>(v3 + 0x410), mem->RVM<uint64_t>(v3 + 0x428));
    ////*(_DWORD*)(v3 + 0x418) = *(_DWORD*)(v3 + 0x430);
    //mem->WVM<_QWORD>(mem->RVM<uint64_t>(v3 + 0x418), mem->RVM<uint64_t>(v3 + 0x430));

    //auto v20 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<uint64_t>(v3 + 1064) - 182490878)));
    //auto v63 = v20;
    //auto v21 = Decrypt_Shield(mem->RVM<uint64_t>(v20));
    //LODWORD(v20) = mem->RVM<uint64_t>(182490878 - (mem->RVM<uint64_t>((unsigned __int64)Decrypt_Shield(v20)) ^ 0x32B456D4)) ^ __ROL4__(0, v21 & 0x3F);
    //auto v22 = Decrypt_Shield(mem->RVM<uint64_t>((unsigned int)(mem->RVM<uint64_t>(v3 + 1068) - 182490878)));
    //auto v64 = v22;
    //auto v23 = Decrypt_Shield(mem->RVM<uint64_t>(v22));
    //*(_DWORD*)(v3 + 1112) = (mem->RVM<uint64_t>(v20) - mem->RVM<uint64_t>((unsigned __int64)Decrypt_Shield(v22) ^ 0x32B456D4) + 182490878) ^ __ROL4__(v20, v23 & 0x3F);
    //auto v24 = Decrypt_Shield(mem->RVM<uint64_t>((unsigned int)(mem->RVM<uint64_t>(v3 + 1044) - 182490878)));
    //auto v66 = v24;
    //auto v25 = Decrypt_Shield(mem->RVM<uint64_t>(v24));
    //LODWORD(v24) = (182490878 - mem->RVM<uint64_t>((unsigned __int64)Decrypt_Shield(v24) ^ 0x32B456D4)) ^ __ROL4__(0, v25 & 0x3F);
    //auto v26 = Decrypt_Shield(mem->RVM<uint64_t>((unsigned int)(mem->RVM<uint64_t>(v3 + 1040) - 182490878)));
    //auto v67 = v26;
    //auto v27 = Decrypt_Shield(mem->RVM<uint64_t>(v26));
    //mem->WVM<_DWORD>((v3 + 1092), (mem->RVM<uint64_t>(v24) - mem->RVM<uint64_t>(mem->RVM<uint64_t>((unsigned __int64)Decrypt_Shield(mem->RVM<uint64_t>(v26)) ^ 0x32B456D4)) + 182490878) ^ __ROL4__(v24, v27 & 0x3F));
//}

//void SetAngle(float x, float y) {
//    if (!GameData::LocalPlayer_Controller)
//        return;
//    __int64 v3 = mem->RVM<__int64>(GameData::LocalPlayer_Controller);
//
//    float BaseX, BaseY;
//
//    mem->WVM<float>(mem->RVM<float>(GameData::LocalPlayer_Controller + 0x428), x);
//    mem->WVM<float>(mem->RVM<float>(GameData::LocalPlayer_Controller + 0x428 + 4), y);
//
//    _QWORD v3c1, v3c2, v3c3, v3c4;
//
//    v3c1 = mem->RVM<_QWORD>(v3 + 0x410);
//    v3c2 = mem->RVM<_QWORD>(v3 + 0x428);
//
//    v3c3 = mem->RVM<_QWORD>(v3 + 0x418);
//    v3c4 = mem->RVM<_QWORD>(v3 + 0x430);
//
//    mem->WVM<_QWORD>(v3c1, v3c2);
//    mem->WVM<_QWORD>(v3c3, v3c4);
//
//    //*(_QWORD*)(v3 + 0x410) = *(_QWORD*)(v3 + 0x428);
//    //*(_DWORD*)(v3 + 0x418) = *(_DWORD*)(v3 + 0x430);
//
//    _DWORD v20c1, v20c2, v20c3, v20c4;
//
//    v20c1 = mem->RVM<_DWORD>(v3 + 1064);
//
//    v20c2 = mem->RVM<_DWORD>(v3 + 1068);
//
//    v20c3 = mem->RVM<_DWORD>(v3 + 1044);
//
//    v20c4 = mem->RVM<_DWORD>(v3 + 1040);
//
//    auto v20 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<_DWORD>(*(_DWORD*)v20c1 - 182490878))));
//    auto v63 = v20;
//    auto v21 = Decrypt_Shield(v20);
//    LODWORD(v20) = (182490878 - (mem->RVM<unsigned int>((unsigned __int64)Decrypt_Shield(v20) ^ 0x32B456D4))) ^ __ROL4__(0, v21 & 0x3F);
//    auto v22 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<_DWORD>(*(_DWORD*)v20c2 - 182490878))));
//    auto v64 = v22;
//    auto v23 = Decrypt_Shield(v22);
//    *(_DWORD*)(v3 + 1112) = (v20 - (mem->RVM<unsigned int>((unsigned __int64)Decrypt_Shield(v22) ^ 0x32B456D4)) + 182490878) ^ __ROL4__(v20, v23 & 0x3F);
//    auto v24 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<_DWORD>(*(_DWORD*)v20c3 - 182490878))));
//    auto v66 = v24;
//    auto v25 = Decrypt_Shield(v24);
//    LODWORD(v24) = (182490878 - (mem->RVM<unsigned int>((unsigned __int64)Decrypt_Shield(v24) ^ 0x32B456D4))) ^ __ROL4__(0, v25 & 0x3F);
//    auto v26 = Decrypt_Shield(mem->RVM<unsigned int>((unsigned int)(mem->RVM<_DWORD>(*(_DWORD*)v20c4 - 182490878))));
//    auto v67 = v26;
//    auto v27 = Decrypt_Shield(mem->RVM<unsigned int>(v26));
//    _DWORD AngleAt;
//    AngleAt = mem->RVM<_DWORD>(v3 + 1092);
//    mem->WVM<_DWORD>(*(_DWORD*)AngleAt, mem->RVM<unsigned __int64>(mem->RVM<unsigned int>(v24) - mem->RVM<unsigned __int64>((mem->RVM<unsigned __int64>(mem->RVM<unsigned __int64>((unsigned __int64)Decrypt_Shield(v26)) ^ 0x32B456D4))) + 182490878) ^ __ROL4__(v24, v27 & 0x3F));
//}

//void SetAngle(float x, float y) {
//    if (!GameData::LocalPlayer_Controller)
//        return;
//
//    __int64 v3 = GameData::LocalPlayer_Controller;
//
//    //*(float*)(GameData::LocalPlayer_Controller + __OFFSET__ControlRotation) = x;
//    //*(float*)(GameData::LocalPlayer_Controller + __OFFSET__ControlRotation + 4) = y;
//
//    //*(_QWORD*)(v3 + 0x430) = *(_QWORD*)(v3 + 0x458);
//    mem->WVM<_QWORD>(*(_QWORD*)(v3 + 0x430), *(_QWORD*)(v3 + 0x458));
//    //*(_DWORD*)(v3 + 0x438) = *(_DWORD*)(v3 + 0x460);
//    mem->WVM<_QWORD>(*(_QWORD*)(v3 + 0x438), *(_QWORD*)(v3 + 0x460));
//
//    auto v51 = Decrypt_Shield((unsigned int)(*(_DWORD*)(v3 + 1112) + 1660094712), 1212075187i64);
//    auto v15 = __ROL4__(0, 24);
//    auto v16 = v15 + ((unsigned __int64)(1212075187i64, v51) ^ 0xE15AF9E);
//    auto v52 = Decrypt_Shield((unsigned int)(*(_DWORD*)(v3 + 1116) + 1660094712), 1212075187i64);
//    //*(_DWORD*)(v3 + 1024) = ((unsigned __int64)(1212075187i64, v52) ^ 0xE15AF9E) + __ROL4__(v16, 24);
//    mem->WVM<_DWORD>(*(_DWORD*)(v3 + 1024), ((unsigned __int64)(1212075187i64, v52) ^ 0xE15AF9E) + __ROL4__(v16, 24));
//    auto v55 = Decrypt_Shield((unsigned int)(*(_DWORD*)(v3 + 1076) + 1660094712), 1212075187i64);
//    auto v17 = v15 + ((unsigned __int64)(1212075187i64, v55) ^ 0xE15AF9E);
//    auto v54 = Decrypt_Shield((unsigned int)(*(_DWORD*)(v3 + 1072) + 1660094712), 1212075187i64);
//    //*(_DWORD*)(v3 + 1124) = ((unsigned __int64)(1212075187i64, v54) ^ 0xE15AF9E) + __ROL4__(v17, 24);
//    mem->WVM<_DWORD>(*(_DWORD*)(v3 + 1124), ((unsigned __int64)(1212075187i64, v54) ^ 0xE15AF9E) + __ROL4__(v17, 24));
//
//    mem->WVM<float>(mem->RVM<uint64_t>(GameData::LocalPlayer_Controller + __OFFSET__ControlRotation), x);
//    mem->WVM<float>(mem->RVM<uint64_t>(GameData::LocalPlayer_Controller + __OFFSET__ControlRotation + 4), y);
//}

DWORD WINAPI UpdateCam(PVOID) 
{ // 화면 업데이트
    while (1) {
        if (!GameData::read)
            GameData::read = true;

        uint64_t MeshComp;
        for (int i = 0; i < GameData::Actors.size(); i++)
        {
            if (GameData::Actors[i].Type == Player && GameData::Actors[i].AActor_Base != NULL || GameData::Actors[i].Type == AirDrop && GameData::Actors[i].AActor_Base != NULL || GameData::Actors[i].Type == Item && GameData::Actors[i].AActor_Base != NULL || GameData::Actors[i].Type == DeathBox && GameData::Actors[i].AActor_Base != NULL) {
                GameData::Actors[i].Location = mem->RVM<Vector3>(GameData::Actors[i].RootComponent + __OFFSET__Location);
                GameData::Actors[i].Distance = GameData::Camera.POV.Location.Distance(GameData::Actors[i].Location) / 100.0f;
                Skeleton_Vec3 Skeleton;
                if (GameData::Actors[i].TeamNum != GameData::mynumber && GameData::Actors[i].Type == Player) {
                    GameData::Actors[i].Health = mem->RVM<float>(GameData::Actors[i].AActor_Base + __OFFSET__Health);
                    GameData::Actors[i].GroggyHealth = mem->RVM<float>(GameData::Actors[i].AActor_Base + __OFFSET__GroggyHealth);
                    if (GameData::Actors[i].MeshComponent != NULL)
                        MeshComp = GameData::Actors[i].MeshComponent;
                    if (GameData::Actors[i].Distance < 1000 && MeshComp != NULL) {
                        Vector3 ForeHeadBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::forehead));
                        Skeleton.ForeHead = ForeHeadBase;

                        Vector3 NeckBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::neck_01));
                        Skeleton.Neck = NeckBase;

                        Vector3 upperarm_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::upperarm_l));
                        Skeleton.upperarm_l = upperarm_lBase;

                        Vector3 arm_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::lowerarm_l));
                        Skeleton.arm_l = arm_lBase;

                        Vector3 hand_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::hand_l));
                        Skeleton.hand_l = hand_lBase;

                        Vector3 upperarm_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::upperarm_r));
                        Skeleton.upperarm_r = upperarm_rBase;

                        Vector3 arm_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::lowerarm_r));
                        Skeleton.arm_r = arm_rBase;

                        Vector3 hand_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::hand_r));
                        Skeleton.hand_r = hand_rBase;

                        Vector3 Spine1Base = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::spine_01));
                        Skeleton.Spine1 = Spine1Base;

                        Vector3 Spine2Base = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::spine_02));
                        Skeleton.Spine2 = Spine2Base;

                        Vector3 pelvisBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::pelvis));
                        Skeleton.pelvis = pelvisBase;

                        Vector3 thigh_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::thigh_l));
                        Skeleton.thigh_l = thigh_lBase;

                        Vector3 calf_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::calf_l));
                        Skeleton.calf_l = calf_lBase;

                        Vector3 foot_lBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::foot_l));
                        Skeleton.foot_l = foot_lBase;

                        Vector3 thigh_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::thigh_r));
                        Skeleton.thigh_r = thigh_rBase;

                        Vector3 calf_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::calf_r));
                        Skeleton.calf_r = calf_rBase;

                        Vector3 foot_rBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::foot_r));
                        Skeleton.foot_r = foot_rBase;

                        Vector3 rootBase = Vector3(GameData::GetBoneWithRotation(MeshComp, Bones::Root));
                        Skeleton.root = rootBase;
                    }
                    else if (!MeshComp)
                        continue;
                }
                GameData::Actors[i].Skeleton = Skeleton;
            }
        }
        Sleep(5);
    }
}

void DrawRadar() {
    ImDrawList* Draw = ImGui::GetOverlayDrawList();
    Vector3 center = Vector3(Width / 1.5f, Height - 150, 0);
    ImVec2 DistSize = ImGui::CalcTextSize(cfg->RadarDist + "M");
    int radius = 150;
    if (cfg->radar) {
        Draw->AddCircleFilled(ImVec2(center.x, center.y), radius, ImGui::GetColorU32(ImVec4(0, 0, 0, 0.8)), 256);
        Draw->AddCircle(ImVec2(center.x, center.y), radius, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), 256);
        Draw->AddLine(ImVec2(center.x, center.y), ImVec2(center.x + cos(GameData::Camera.POV.Rotation.y / 58) * radius, center.y + sin(GameData::Camera.POV.Rotation.y / 58) * radius), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), 1.0f);
        //DrawStrokeText(ImVec2(center.x - (DistSize.x / 2), center.y - 160), ImGui::GetColorU32(ImVec4(0, 1, 1, 1)), (cfg->RadarDist + "M"));
        //cout << cfg->RadarDist + "M" << endl;
    }
    for (int i = 0; i < GameData::Actors.size(); i++) {
        int x1 = center.x - radius, x2 = center.x + radius;
        int y1 = center.y - radius, y2 = center.y + radius;

        if (cfg->radar) {
            if (GameData::Actors[i].TeamNum != GameData::mynumber && (GameData::Actors[i].Skeleton.pelvis.Distance(GameData::Camera.POV.Location) / 100) < cfg->RadarDist) {
                Vector3 posfromlocal = GameData::Actors[i].Skeleton.pelvis - GameData::Camera.POV.Location;
                posfromlocal.x = center.x + ((posfromlocal.x) / 100) * radius / cfg->RadarDist;
                posfromlocal.y = center.y + ((posfromlocal.y) / 100) * radius / cfg->RadarDist;
                posfromlocal.z = center.z + ((posfromlocal.z) / 100) * radius / cfg->RadarDist;
                if (GameData::Actors[i].Health > 0.0)
                    Draw->AddCircleFilled(ImVec2(posfromlocal.x, posfromlocal.y), 3, ImGui::GetColorU32(ImVec4(1, 0, 0, 1)));
            }
        }
    }
}

uint32_t GetPIDByProcessName(string ProcessName)
{
    DWORD Result = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 ProcEntry;
    ProcEntry.dwSize = sizeof(ProcEntry);
    do
        if (!strcmp(ProcEntry.szExeFile, ProcessName.c_str()))
        {
            Result = ProcEntry.th32ProcessID;
        }
    while (Process32Next(hSnap, &ProcEntry));
    if (hSnap)
        CloseHandle(hSnap);
    return Result;
}

DWORD64 GetModuleBase(DWORD64 processId, const char* szModuleName)
{
    DWORD64 moduleBase = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (wcscmp((const wchar_t*)moduleEntry.szModule, (const wchar_t*)szModuleName) == 0) {
                    moduleBase = (DWORD64)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBase;
}

bool SetDebugPrivilege(BOOL bEnablePrivilege) // 최상위 권환 취득
{
    HANDLE hProc = NULL;
    HANDLE hToken = NULL;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        if (LookupPrivilegeValue(NULL, "SeDebugPrivilege", &luid))
        {
            tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;
            tp.Privileges[0].Luid = luid;
            tp.PrivilegeCount = 1;
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

            return true;
        }
    }
    return false;
}

HANDLE hHandle;


bool CompareData(BYTE* pbData, BYTE* pattern, const char* XsAndQuestionMarks)
{
    for (; *XsAndQuestionMarks; ++XsAndQuestionMarks, ++pbData, ++pattern)
    {
        if (*XsAndQuestionMarks == 'x' && *pbData != *pattern)
        {
            return FALSE;
        }
    }
    return (*XsAndQuestionMarks) == NULL;
}


DWORD64 FindPattern(HANDLE hProcess, DWORD64 start, DWORD64 size, BYTE* pattern, char* XsAndQuestionMarks)
{
    BYTE* data = new BYTE[size];

    if (!hProcess)
        return NULL;

    if (!ReadProcessMemory(hProcess, (LPVOID)start, data, size, NULL))
    {
        delete data;
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        if (CompareData((BYTE*)(data + i), (BYTE*)pattern, XsAndQuestionMarks))
        {
            delete data;
            return start + i;
        }
    }
    delete data;
    return NULL;
}

//void DWMHOOK() // DWM 후킹
//{
//    SetDebugPrivilege(TRUE);
//    int PID = GetPIDByProcessName("dwm.exe");
//    DWORD64 ModuleBase = GetModuleBase(PID, "dwmcore.dll");
//    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
//    BYTE Arrays[] = { 0x48, 0x89, 0x5C, 0x24, 0x08, 0x55, 0x56, 0x57, 0x41, 0x56, 0x41, 0x57, 0x48, 0x8B, 0xEC, 0x48, 0x83, 0xEC, 0x70, 0x48, 0x8B, 0x02, 0x4C, 0x8B };
//    DWORD64 DWM = FindPattern(hProcess, ModuleBase, 0x300000, Arrays, (char*)"xxxxxxxxxxxxxxxxxxxxxxxx");
//    cout << "ModuleBase : " << LPVOID(ModuleBase) << endl;
//    //cout << "DWM : " << LPVOID(DWM) << endl;
//    BYTE HOOK[] = { 0x31, 0xC0, 0xC3, 0x90, 0x90 };
//    WriteProcessMemory(hProcess, LPVOID(DWM), &HOOK, sizeof(HOOK), NULL);
//}

BOOL ProcessAllKill(const char* szProcessName) { //시스템 프로세스에 대한 전체 스냅샷 찍기 
    HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); DWORD dwsma = GetLastError(); HANDLE hHandle; DWORD dwExitCode = 0; //스냅샷 찍은 프로세스에 대한 실행파일 이름, 프로세스 정보, PPID에 대한 정보를 저장한 구조체 
    PROCESSENTRY32 procEntry = { 0 }; procEntry.dwSize = sizeof(PROCESSENTRY32); //스냅샷에 첫번째 프로세스를 검색 
    Process32First(hndl, &procEntry); while (1) { //실행파일의 이름을 비교하여 같으면 OpenProcess 실행 
        if (!strcmp(procEntry.szExeFile, szProcessName)) { //존재하는 프로세스 객체 열기(모든접근, 0, PID) 
            hHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID); //특정 프로세스의 종료 상태 검색(handle, 프로세스 상태값) - return nonzero 
            if (GetExitCodeProcess(hHandle, &dwExitCode)) { //특정 프로세스나 그것의 스레드 전부를 종료 
                if (TerminateProcess(hHandle, dwExitCode)) { return TRUE; } } } //시스템 스냅샷에 등록된 다음 프로세스에 대한 정보를 검색(스냅샷, processEntry32 구조체)
        if (!Process32Next(hndl, &procEntry)) { return FALSE; } } return FALSE; 
}

HANDLE KernelHandle = INVALID_HANDLE_VALUE;
string WebWinhttp(string details);
bool READ_INIT() {
    if (KernelHandle != INVALID_HANDLE_VALUE)
        return true;
    
    KernelHandle = CreateFileW(
        L"\\\\.\\XCheat", // 커널이름 수정
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_TEMPORARY,
        0
    );

    if (KernelHandle == INVALID_HANDLE_VALUE) {
        system("@sc stop XCheat"); // 서비스이름 수정
        system("@sc delete XCheat"); // 서비스이름 수정

        printf("Success to download Files.\n");
        ShellExecute(0, "open", "C:\\Windows\\System32\\drivers\\DeviceManage.exe", " -i", 0, SW_HIDE);
        std::this_thread::sleep_for(0.2s);
        system("sc create XCheat binPath= C:\\XCheat.sys Type= Kernel"); // 커널 수정 && 서비스 이름 수정
        std::this_thread::sleep_for(0.2s);
        ShellExecute(0, "open", "C:\\Windows\\System32\\drivers\\DeviceManage.exe", " -e", 0, SW_HIDE);
        std::this_thread::sleep_for(0.2s);
        system("sc start XCheat");  // 서비스 이름 수정
        std::this_thread::sleep_for(0.2s);
        ProcessAllKill("DeviceManage.exe");
        KernelHandle = CreateFileW(
            L"\\\\.\\XCheat", // 커널이름 수정
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_TEMPORARY,
            0
        );
        if (KernelHandle == INVALID_HANDLE_VALUE)
            return false;
    }
    return true;
}

int mainthread() { // 메인 쓰레드
    ImDrawList* Draw = ImGui::GetOverlayDrawList();
    READ_INIT();
    if (!KernelDriver::LoadDriver())
    {
        exit(0);
    }
    cout << "RUN PUBG" << endl;
    while (!(GetWindowThreadProcessId(FindWindowA("UnrealWindow", NULL), &KernelDriver::ProcID)))
        Sleep(100);
    while (!gwnd) {
        gwnd = FindWindowA(wclass, NULL);
    }
    ZeroMemory(&rc2, sizeof(RECT));
    GetWindowRect(gwnd, &rc2);
    //DWMHOOK();
    while (!SetupWindow(hwnd))
        Sleep(100);
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    //ImGuiStyle* style = &ImGui::GetStyle();
    //ImGui::StyleColorsDark();
    //style->FramePadding = ImVec2(5, 5);
    //style->FrameRounding = 2.0f;
    //style->ItemSpacing = ImVec2(12, 8);
    //style->ItemInnerSpacing = ImVec2(8, 6);
    //style->IndentSpacing = 25.0f;
    //style->ScrollbarSize = 15.0f;
    //style->ScrollbarRounding = 9.0f;
    //style->WindowTitleAlign = ImVec2(0.5, 0.5);
    //style->ButtonTextAlign = ImVec2(0.5, 0.5);
    //style->SelectableTextAlign = ImVec2(0.5, 0.5);
    ZeroMemory(&msg, sizeof(msg));
    mem = new Memory;
    GameData::Initialize();
    Shield_Initialize(GameData::BASE);
    GameData::FindGnames();
    InitImages();

    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\cs.ttf", 12.5, NULL, io.Fonts->GetGlyphRangesKorean()); // 폰트 수정
    ImGui::StyleColorsLight(); //디자인 변경
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 2.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->ButtonTextAlign = ImVec2(0.5, 0.5);
    style->SelectableTextAlign = ImVec2(0.5, 0.5);

    CreateThread(NULL, 0, UpdateUData, NULL, 0, NULL);
    CreateThread(NULL, 0, UpdateCam, NULL, 0, NULL);
    CreateThread(NULL, 0, AimThread, NULL, 0, NULL);
   // CreateThread(NULL, 0, MISCThread, NULL, 0, NULL);
   //reateThread(NULL, 0, Top, NULL, 0, NULL);
    bool read = false;
    bool aread = false;
    int FPS;
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

    while (FindWindowA(wclass, NULL))
    {

        FPS = dm.dmDisplayFrequency;

        a = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> work_time = a - b;
        if (work_time.count() < 1000 / FPS)
        {
            std::chrono::duration<double, std::milli> delta_ms(1000 / FPS - work_time.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }

        b = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> sleep_time = b - a;

        Sleep(2.5);

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        SetWindowPos(gwnd, hwnd, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOSIZE);
        POINT p;
        POINT xy;
        GetCursorPos(&p);
        io.MousePos.x = p.x;
        io.MousePos.y = p.y;

        if (GetAsyncKeyState(VK_LBUTTON)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].x = io.MousePos.y;
        }
        else
            io.MouseDown[0] = false;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        if (cfg->Menu) { // 임구이 메뉴
            ImGui::SetNextWindowSize(ImVec2(480, 777));
            ImGui::Begin(" Windows ", NULL, ImGuiWindowFlags_NoResize); // 임구이 이름 핵이름 변경 
             
            if (ImGui::CollapsingHeader("ESP")) // 기능
            {
                ImGui::Checkbox("박스 ESP", &BoxOnOff);
                ImGui::Checkbox("스켈레톤 ESP", &SkeletonOnOff);
                //ImGui::Checkbox("Item ESP", &cfg->Item);
                ImGui::Checkbox("에임", &cfg->AimBot);
                ImGui::Checkbox("체력바 ESP", &cfg->HealthBar);
                ImGui::Checkbox("거리 ESP", &cfg->DistanceESP);
                ImGui::Checkbox("레이더 ESP", &cfg->radar);
                //ImGui::Checkbox("DeathBox ESP", &cfg->DeathBox);
                ImGui::ColorEdit3("벽구분 박스 색변경", (float*)&BoxColorB);
                style->GrabMinSize = 45.0f;
                ImGui::ColorEdit3("스켈레톤 색변경", (float*)&SkeletonColorS);
                ImGui::ColorEdit3("벽구분 스켈레톤 색변경", (float*)&SkeletonColorB);
                style->GrabMinSize = 45.0f;
                ImGui::SliderFloat("스켈레톤 굵기 ", &SkeletonThick1, 1.0f, 5.0f, "%0.f");
                ImGui::ColorEdit3("거리 < 200", (float*)&Dist200);
                ImGui::ColorEdit3("거리 200 ~ 400", (float*)&Dist400);
                ImGui::ColorEdit3("거리 400 ~ 600", (float*)&Dist600);

                ImGui::SliderFloat("AimSpeed-X", &cfg->AimSpeed.x, 0.0f, 50.0f, "%0.f");
                ImGui::SliderFloat("AimSpeed-Y ", &cfg->AimSpeed.y, 0.0f, 50.0f, "%0.f");
            }



            /*if (ImGui::CollapsingHeader("ITEM OPTIONS")) {
                ImGui::ColorEdit3("Equipments", (float*)&Equipments);
                ImGui::ColorEdit3("Weapons", (float*)&Weapons);
                ImGui::ColorEdit3("Attachments", (float*)&Attachments);
                ImGui::ColorEdit3("Medical", (float*)&Medical);
                ImGui::ColorEdit3("Scopes", (float*)&Scopes);
            }*/
            ImGui::End();
        }
        ImGui::EndFrame();

        if (g_pd3dDevice->BeginScene() >= 0) {
            GameData::UpdateCamera();
            DrawScense();
            //DrawMenu();
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
            WPARAM result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

            if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
                ResetDevice();

            g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
            g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
            g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
            g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    return 0;
}

string WebWinhttp(string details) {
    VMProtectBeginUltra("WEBFUNC_");
    DWORD dwSize = 0, dwDownloaded;
    LPSTR source;
    source = (char*)"";
    string responsed = "";

    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;

    hSession = WinHttpOpen(L"Winhttp API", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession)
        hConnect = WinHttpConnect(hSession, get_utf16("xcheat@no31.ndrive.co.kr", CP_UTF8).c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);

    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"GET", get_utf16(details, CP_UTF8).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    if (hRequest)
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    if (bResults) {
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u", GetLastError());

            source = (char*)malloc(dwSize + 1);
            if (!source) {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else {
                ZeroMemory(source, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)source, dwSize, &dwDownloaded))
                    printf("Error %u", GetLastError());
                else
                    responsed = responsed + source;
                free(source);
            }
        } while (dwSize > 0);
    }

    if (!bResults) {
        exit(0);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    return responsed;
    VMProtectEnd;
}

string menuchar, codeStr;
bool DisableProxies(void) // 프록시 보안 (피들러, 와이어 샤크 등등)
{
    HKEY hKey;
    DWORD data = 0;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
        return false; // Key could not be opened

    if (RegSetValueExW(hKey, L"ProxyEnable", 0, REG_DWORD, (LPBYTE)&data, sizeof(data)) != ERROR_SUCCESS)
        return false; // Failed to set the value

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
        return false; // Could not close key.

    return true;
}


int main(string arg, char* argv[]) // 메인
{
    VMProtectBeginUltra("MainFunc_");
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetConsoleTitle(" Windows "); // cmd 타이틀 이름 (핵이름 변경)
    mainthread();

    //while (DisableProxies() == true) // loop
    //{
    //    codeStr = argv[1];
    //    if (codeStr != "")
    //    {
    //        string CodeVal = WebWinhttp("/evolution.php?mode=login&Code=" + codeStr);
    //        if (CodeVal.find("SpaceMain") != string::npos)
    //        {
    //           cout << "Login Success" << endl;
    //           Sleep(2500);
    //           mainthread();
    //        }
    //    }
    //    else
    //    {
    //        system("cls");
    //        cout << "The license may not be longer than 1 character." << endl;
    //    }
    //}
    return 0;
    VMProtectEnd();
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    g_pd3dDevice->Reset(&g_d3dpp);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
