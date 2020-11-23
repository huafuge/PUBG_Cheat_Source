#pragma once
#include <Windows.h>
#include <vector>

#include "defines.h"

#include "Shield.h"

#include "defs.h"



enum ActorType : int
{
    Player,
    Item,
    AirDrop,
    DeathBox,
    Vehicles,
    Grenades,
};

list<int> upper_part = { Bones::neck_01, Bones::Head, Bones::forehead };
list<int> right_arm = { Bones::neck_01, Bones::upperarm_r, Bones::lowerarm_r, Bones::hand_r };
list<int> left_arm = { Bones::neck_01, Bones::upperarm_l, Bones::lowerarm_l, Bones::hand_l };
list<int> spine = { Bones::neck_01, Bones::spine_01, Bones::spine_02, Bones::pelvis };

list<int> lower_right = { Bones::pelvis, Bones::thigh_r, Bones::calf_r, Bones::foot_r };
list<int> lower_left = { Bones::pelvis, Bones::thigh_l, Bones::calf_l, Bones::foot_l };

list<list<int>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };

using namespace std;

bool namebool = true;

D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
    float radPitch = (rot.x * float(M_PI) / 180.f);
    float radYaw = (rot.y * float(M_PI) / 180.f);
    float radRoll = (rot.z * float(M_PI) / 180.f);

    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);

    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;

    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;

    return matrix;
}

Vector3 WorldToScreen(Vector3 WorldLocation, FCameraCacheEntry CameraCacheL)
{
    Vector3 Screenlocation = Vector3(0, 0, 0);

    auto POV = CameraCacheL.POV;
    Vector3 Rotation = POV.Rotation; // FRotator

    D3DMATRIX tempMatrix = Matrix(Rotation); // Matrix

    Vector3 vAxisX, vAxisY, vAxisZ;

    vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    Vector3 vDelta = WorldLocation - POV.Location;
    Vector3 vTransformed = Vector3(vDelta * vAxisY, vDelta * vAxisZ, vDelta * vAxisX);

    if (vTransformed.z < 1.f)
        vTransformed.z = 1.f;

    float FovAngle = POV.FOV;
    float ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
    float ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;

    Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
    Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

    return Screenlocation;
}

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return pOut;
}

class Skeleton_Vec3
{
public:
    Vector3 ForeHead, Head, Neck,
        upperarm_l, arm_l, hand_l, upperarm_r, arm_r, hand_r,
        Spine1, Spine2, pelvis,
        thigh_l, calf_l, foot_l,
        thigh_r, calf_r, foot_r, root;
};

class LootCategory {
public:
    uint64_t Loot_Base;
    uint64_t ActorID;
    Vector3 Location;
    string ObjectName;
};

class AActor
{
public:
    uint64_t AActor_Base;
    uint64_t MeshComponent;
    uint64_t RootComponent;
    uint64_t ActorID;
    Vector3 Location;
    Vector3 LastLoc;
    Vector3 NowLoc;
    Vector3 Velocity;
    Skeleton_Vec3 Skeleton;
    string PlayerName;
    string ObjectName;
    float Distance;
    float Health;
    float GroggyHealth;
    int RenderTime;
    int TeamNum;
    ActorType Type;
};

namespace Loot {
    int DroppedItemGroup, Carapackage_RedBox, Carapackage_RedBox1, DeathDropItemPackage;
};

namespace Vehicle {
    int PlayerMale, PlayerFemale, BotPlayerMale, BotPlayerFemale, RegistedPlayer, AIPawnBase;
    int Dacia1, Dacia2, Dacia3, Dacia4;
    int Scooter1, Scooter2, Scooter3, Scooter4;
    int TukTukTuk1, TukTukTuk2, TukTukTuk3, TukTukTuk4;
    int MiniBus;
    int Uaz1, Uaz2, Uaz3, Uaz4, Uaz5, Uaz6, Uaz7, Uaz8;
    int Pickup1, Pickup2, Pickup3, Pickup4, Pickup5, Pickup6, Pickup7, Pickup8, Pickup9, Pickup10;
    int Rony1, Rony2, Rony3, Rony4;
    int Niva1, Niva2, Niva3, Niva4;
    int Boat1, Boat2, Boat3;
    int Buggy1, Buggy2, Buggy3, Buggy4, Buggy5, Buggy6;
    int Bike1, Bike2, Bike3, Bike4, Bike5, Bike6, Bike7, Bike8, Bike9, Bike10, Bike11, Bike12;
    int TBike1, TBike2, TBike3, TBike4, TBike5, TBike6;
    int Mirado1, Mirado2, Mirado3, Mirado4, Mirado5, Mirado6, Mirado7, Mirado8;
    int AquaRail1, AquaRail2, AquaRail3, AquaRail4, AquaRail5;
    int BRDM;
    int Motorglider;
};

namespace ItemGroup
{
    int AKM, M416, M16A4, SCAL, G36C, UMP, AUG, Groza, AWM, Mini14, SKS, QBZ, M249, MK47;//AR
    int Beryl, QBU, VectorGun;
    int Kar98K, M24, VSS;//Sniper
    int Armor2, Armor3, Head2_1, Head2_2, Head3;
    int Back2, Back2_2, Back3, Back3_2;
    int FirstAid, MedKit, EnergyDrink, PainKiller, Adrenaline;
    int Scope3x, Scope4x, Scope6x, Scope8x, ExtendedQuickDraw, SniperExtendedQuickDraw;
    int Ammo_556, Ammo_762, Ammo_300;
    int Suppressor, Stock, Compensator, SniperSuppressor, RedDot, HoloGram;
    int Grenade, SmokeBomb, FlareGun, ProjGrenade;
};

namespace GameData
{
    UINT64 GNames = 0;
    uint64_t BASE;
    uint64_t CameraManager;
    uint64_t UWorld;
    uint64_t UGameInst;
    uint64_t ULevel;
    uint64_t LocalPlayer;
    uint64_t LocalPlayer_Controller;
    uint64_t LocalPlayer_Pawn;
    uint64_t LocalPlayer_Pawn_Mesh;
    uint64_t LocalPlayer_RootComponent;
    uint64_t LocalPlayer_MESH;
    uint64_t LocalPlayer_WeaponProcessor;
    uint64_t LocalPlayer_EquippedWeapon;
    uint64_t LocalPlayer_Index;
    uint64_t LocalPlayer_WeaponConfig;
    uint64_t LocalPlayer_WeaponData;
    uint64_t SwayPTR;
    uint64_t LocalNamePTR;
    uint64_t ObjectArray;
    uint64_t ObjectIdAddress;
    uint64_t ItemObject;
    uint64_t PID = 0;
    Vector3 LocalSway;
    Vector3 Prediction;
    string LocalName;
    string MainName;
    string NearPlayerName;
    string ItemName;
    vector<string> GNamesStr;
    BYTE CurrentWeaponIndex;
    uint64_t LocalPlayer_CurrentWeapon;
    char name[64];
    char myname[64];
    int SC, mynumber;
    int MinDist;
    float BulletSpeedVal;
    float VDragCoefficient;
    bool read;
    int aRootComponent;
    int ItemCount;
    int ItemID;
    int aimindex;
    int SpectatedCount;
    Vector3 LocalPlayer_Location;
    FCameraCacheEntry Camera;
    FCameraCacheEntry AimCamera;
    FCameraCacheEntry Recoil;

    uint64_t ActorTArray;
    WORD ActorCount;
    uint64_t ActorPTR;

    vector<AActor> Actors;
    vector<LootCategory> Loots;

    void Initialize()
    {
        BASE = (uint64_t)mem->GetProcBase();
    }

    FTransform GetBoneIndex(uint64_t mesh, int index)
    {
        uint64_t bonearray = mem->RVM<uint64_t>(mesh + __OFFSET__BoneArray);

        return mem->RVM<FTransform>(bonearray + (index * 0x30));
    }

    Vector3 GetBoneWithRotation(uint64_t mesh, int id)
    {
        FTransform bone = GetBoneIndex(mesh, id);

        FTransform ComponentToWorld = mem->RVM<FTransform>(mesh + __OFFSET__ComponentToWorld);
        D3DMATRIX Matrix;
        Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
        return Vector3(Matrix._41, Matrix._42, Matrix._43);
    }

    int getLastRenderTime(UINT64 actor) {
        return mem->RVM<int>(mem->RVM<uint64_t>(actor + __OFFSET__ActorMesh) + __OFFSET__LastRenderTime);
    }

    string GetNameFromIdUsingGName(uint64_t GN, int ID)
    {
        UINT64 fNamePtr = mem->RVM<uint64_t>(GN + ((int(ID / __OFFSET__ChunkSize)) * 8));
        UINT64 fName = mem->RVM<uint64_t>(fNamePtr + ((int(ID % __OFFSET__ChunkSize)) * 8));
        char names_c[64];
        mem->readSTR(fName + 0x10, names_c, sizeof(names_c));
        std::string names = std::string(names_c);
        if (names == "")
            names = "fail";
        return names;
    }

    std::string GetItemNameByID(int ObjectID)
    {
        if (ItemGroup::AKM == ObjectID)
            return "AKM";
        else if (ItemGroup::M416 == ObjectID)
            return "M416";
        else if (ItemGroup::M16A4 == ObjectID)
            return "M16A4";
        else if (ItemGroup::SCAL == ObjectID)
            return "SCAL-L";
        else if (ItemGroup::G36C == ObjectID)
            return "G36-C";
        else if (ItemGroup::UMP == ObjectID)
            return "UMP";
        else if (ItemGroup::Kar98K == ObjectID)
            return "Kar98K";
        else if (ItemGroup::M24 == ObjectID)
            return "M24";
        else if (ItemGroup::VSS == ObjectID)
            return "VSS";
        else if (ItemGroup::G36C == ObjectID)
            return "G36-C";
        else if (ItemGroup::Armor2 == ObjectID)
            return u8"Lv.2 Vest";
        else if (ItemGroup::Armor3 == ObjectID)
            return u8"Lv.3 Vest";
        else if (ItemGroup::Head2_1 == ObjectID || ItemGroup::Head2_2 == ObjectID)
            return u8"Lv.2 Helmat";
        else if (ItemGroup::Head3 == ObjectID)
            return u8"Lv.3 Helmat";
        else if (ItemGroup::AWM == ObjectID)
            return "AWM";
        else if (ItemGroup::AUG == ObjectID)
            return "AUG";
        else if (ItemGroup::Groza == ObjectID)
            return "Groza";
        else if (ItemGroup::QBZ == ObjectID)
            return "QBZ";
        else if (ItemGroup::Mini14 == ObjectID)
            return "Mini14";
        else if (ItemGroup::SKS == ObjectID)
            return "SKS";
        else if (ItemGroup::M249 == ObjectID)
            return "M249";
        else if (ItemGroup::MK47 == ObjectID)
            return "MK47";
        else if (ItemGroup::Beryl == ObjectID)
            return "BerylM762";
        else if (ItemGroup::VectorGun == ObjectID)
            return "Vector";
        else if (ItemGroup::QBU == ObjectID)
            return "QBU";
        else if (ItemGroup::Back2 == ObjectID || ItemGroup::Back2_2 == ObjectID)
            return u8"Lv.2 BackPack";
        else if (ItemGroup::Back3 == ObjectID || ItemGroup::Back3_2 == ObjectID)
            return u8"Lv.3 BackPack";
        else if (ItemGroup::FirstAid == ObjectID)
            return u8"FirstAid";
        else if (ItemGroup::MedKit == ObjectID)
            return u8"MediKit";
        else if (ItemGroup::EnergyDrink == ObjectID)
            return u8"EnergyDrink";
        else if (ItemGroup::PainKiller == ObjectID)
            return u8"PainKiller";
        else if (ItemGroup::Adrenaline == ObjectID)
            return u8"Adrenaline";
        else if (ItemGroup::Scope3x == ObjectID)
            return u8"x3 Scope";
        else if (ItemGroup::Scope4x == ObjectID)
            return u8"x4 Scope";
        else if (ItemGroup::Scope6x == ObjectID)
            return u8"x6 Scope";
        else if (ItemGroup::Scope8x == ObjectID)
            return u8"x8 Scope";
        else if (ItemGroup::ExtendedQuickDraw == ObjectID)
            return u8"ExtendedQuickDraw";
        else if (ItemGroup::SniperExtendedQuickDraw == ObjectID)
            return u8"ExtendedQuickDraw(SR)";
        else if (ItemGroup::Suppressor == ObjectID)
            return u8"Suppressor";
        else if (ItemGroup::SniperSuppressor == ObjectID)
            return u8"Suppressor(SR)";
        else if (ItemGroup::Stock == ObjectID)
            return u8"Chock";
        else if (ItemGroup::Compensator == ObjectID)
            return u8"Compensator";
        else if (ItemGroup::Ammo_556 == ObjectID)
            return "5.56mm";
        else if (ItemGroup::Ammo_762 == ObjectID)
            return "7.62mm";
        else if (ItemGroup::Ammo_300 == ObjectID)
            return u8"300 Mag";
        else if (ItemGroup::RedDot == ObjectID)
            return u8"RedDot";
        else if (ItemGroup::HoloGram == ObjectID)
            return u8"HoloSite";
        else if (ItemGroup::Grenade == ObjectID)
            return u8"Grenade";
        else if (ItemGroup::SmokeBomb == ObjectID)
            return u8"SmokeBomb";
        else if (ItemGroup::FlareGun == ObjectID)
            return u8"FlareGun";
        else if (ItemGroup::ProjGrenade == ObjectID)
            return "Grenade";

        return "NULL";
    }
    uint64_t temp;

    //void DumpGNames() {
    //    int ia = 0;
    //    string GNames;
    //    ofstream file("C:\\Gnames.txt", std::ofstream::binary);
    //    while (1) {
    //        string res = GetNameFromIdUsingGName(temp, ia);

    //        file << res << " | " << ia << endl;
    //        if (ia > 500000) {
    //            file.close();
    //            break;
    //        }
    //        ia++;
    //    }
    //}

    uint64_t FindGnames() {
        if (BASE == 0) {
            BASE = (uint64_t)mem->GetProcBase();
        }
        //cout << hex << BASE << endl;
        UINT64 gnames = 0;
        UINT64 tmp = mem->RVM<uint64_t>(BASE + __OFFSET__GNames);
        //cout << "tmp: " << hex << tmp << endl;
        DWORD64 v11 = Decrypt_Shield(tmp);
        //cout << "v11: " << hex << v11 << endl;
        v11 = mem->RVM<uint64_t>(v11);

        DWORD64 v12 = Decrypt_Shield(v11);
        //cout << "v12: " << hex << v12 << endl;
        v12 = mem->RVM<uint64_t>(v12);

        DWORD64 v13 = Decrypt_Shield(v12);
        //cout << "v13: " << hex << v13 << endl;

        gnames = mem->RVM<uint64_t>(v13);
        gnames = Decrypt_Shield(gnames);
        tmp = gnames;
        gnames = 0;
#pragma region NameDumper

        gnames = tmp;
        temp = gnames;
        //ofstream file("C:\\Windows\\Names.txt", std::ofstream::binary);
        if (GetNameFromIdUsingGName(gnames, 1) == "ByteProperty" && GetNameFromIdUsingGName(gnames, 2) == "IntProperty" && GetNameFromIdUsingGName(gnames, 3) == "BoolProperty")
        {
#pragma region NameDumper
            int ia = 0;
            int fails = 0;
            while (1) {

                string Gres = GetNameFromIdUsingGName(gnames, ia);

                if (Gres == "PlayerFemale_A_C") {
                    Vehicle::PlayerFemale = ia;
                    printf("PlayerFemale_A_C: %i\n", ia);
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "PlayerMale_A_C") {
                    Vehicle::PlayerMale = ia;
                    printf("PlayerMale_A_C: %i\n", ia);
                   // file << Gres << " | " << ia << endl;
                }
                if (Gres == "RegistedPlayer") {
                    Vehicle::RegistedPlayer = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "AIPawn_Base_C") {
                    Vehicle::AIPawnBase = ia;
                    // file << Gres << " | " << ia << endl;
                }
                if (Gres == "AIPawn_Base_Female_C") {
                    Vehicle::BotPlayerFemale = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "AIPawn_Base_Male_C") {
                    Vehicle::BotPlayerMale = ia;
                    // file << Gres << " | " << ia << endl;
                }

                if (Gres == "DroppedItemGroup") {
                    Loot::DroppedItemGroup = ia;
                   // file << Gres << " | " << ia << endl;
                }
                if (Gres == "Carapackage_RedBox_C") {
                    Loot::Carapackage_RedBox = ia;
                   // file << Gres << " | " << ia << endl;
                }
                if (Gres == "Carapackage_RedBox_FBR_01_C") {
                    Loot::Carapackage_RedBox1 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "DeathDropItemPackage_C") {
                    Loot::DeathDropItemPackage = ia;
                    //file << Gres << " | " << ia << endl;
                }

                if (Gres == "Item_Weapon_AK47_C") {
                    ItemGroup::AKM = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_HK416_C") {
                    ItemGroup::M416 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_M16A4_C") {
                    ItemGroup::M16A4 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_SCAR-L_C") {
                    ItemGroup::SCAL = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_G36C_C") {
                    ItemGroup::G36C = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_UMP_C") {
                    ItemGroup::UMP = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Kar98k_C") {
                    ItemGroup::Kar98K = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_M24_C") {

                    //file << Gres << " | " << ia << endl;
                    ItemGroup::M24 = ia;
                }
                if (Gres == "Item_Weapon_VSS_C") {

                    //file << Gres << " | " << ia << endl;
                    ItemGroup::VSS = ia;
                }
                if (Gres == "Item_Weapon_AWM_C") {

                    //file << Gres << " | " << ia << endl;
                    ItemGroup::AWM = ia;
                }
                if (Gres == "Item_Weapon_AUG_C") {
                    ItemGroup::AUG = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Groza_C") {
                    ItemGroup::Groza = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Mini14_C") {
                    ItemGroup::Mini14 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_QBZ95_C") {
                    ItemGroup::QBZ = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_SKS_C") {
                    ItemGroup::SKS = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_M249_C") {
                    ItemGroup::M249 = ia;
                   // file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Mk47Mutant_C") {
                    ItemGroup::MK47 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_BerylM762_C") {
                    ItemGroup::Beryl = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Vector_C") {
                    ItemGroup::VectorGun = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_QBU88_C") {
                    ItemGroup::QBU = ia;
                    //file << Gres << " | " << ia << endl;
                }

                if (Gres == "Item_Armor_D_01_Lv2_C") {
                    ItemGroup::Armor2 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Armor_C_01_Lv3_C") {
                    ItemGroup::Armor3 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Head_F_01_Lv2_C") {
                    ItemGroup::Head2_1 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Head_F_02_Lv2_C") {
                    ItemGroup::Head2_2 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Head_G_01_Lv3_C") {
                    ItemGroup::Head3 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Back_F_01_Lv2_C") {
                    ItemGroup::Back2 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Back_F_02_Lv2_C") {
                    ItemGroup::Back2_2 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Back_C_01_Lv3_C") {
                    ItemGroup::Back3 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Back_C_02_Lv3_C") {
                    ItemGroup::Back3_2 = ia;
                    //file << Gres << " | " << ia << endl;
                }

                if (Gres == "Item_Heal_FirstAid_C") {
                    ItemGroup::FirstAid = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Heal_MedKit_C") {
                    ItemGroup::MedKit = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Boost_EnergyDrink_C") {
                    ItemGroup::EnergyDrink = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Boost_PainKiller_C") {
                    ItemGroup::PainKiller = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Boost_AdrenalineSyringe_C") {
                    ItemGroup::Adrenaline = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_Scope3x_C") {
                    ItemGroup::Scope3x = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_ACOG_01_C") {
                    ItemGroup::Scope4x = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_Scope6x_C") {
                    ItemGroup::Scope6x = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_CQBSS_C") {
                    ItemGroup::Scope8x = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_Large_C") {
                    ItemGroup::ExtendedQuickDraw = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Muzzle_Suppressor_Large_C") {
                    ItemGroup::Suppressor = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Stock_AR_Composite_C") {
                    ItemGroup::Stock = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_SniperRifle_C") {
                    ItemGroup::SniperExtendedQuickDraw = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Muzzle_Suppressor_SniperRifle_C") {
                    ItemGroup::SniperSuppressor = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Muzzle_Compensator_Large_C") {
                    ItemGroup::Compensator = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_DotSight_01_C") {
                    ItemGroup::RedDot = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Attach_Weapon_Upper_Holosight_C") {
                    ItemGroup::HoloGram = ia;
                    //file << Gres << " | " << ia << endl;
                }

                if (Gres == "Item_Ammo_556mm_C") {
                    ItemGroup::Ammo_556 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Ammo_762mm_C") {
                    ItemGroup::Ammo_762 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Ammo_300Magnum_C") {
                    ItemGroup::Ammo_300 = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_Grenade_C") {
                    ItemGroup::Grenade = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_SmokeBomb_C") {
                    ItemGroup::SmokeBomb = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "Item_Weapon_FlareGun_C") {
                    ItemGroup::FlareGun = ia;
                    //file << Gres << " | " << ia << endl;
                }
                if (Gres == "ProjGrenade_C") {
                    ItemGroup::ProjGrenade = ia;
                    //file << Gres << " | " << ia << endl;
                    //cout << "Grenade : " << ia << endl;
                }

                if (Gres == "PlayerFemale_A_C") {
                    Vehicle::PlayerFemale = ia;
                    printf("PlayerFemale_A_C: %i\n", ia);
                    //cout << "Female : " << ia << endl;
                }
                if (Gres == "PlayerMale_A_C") {
                    printf("PlayerMale_A_C: %i\n", ia);
                    Vehicle::PlayerMale = ia;
                    //cout << "Male : " << ia << endl;
                }

                if (Gres == "DeathDropItemPackage_C") {
                    Loot::DeathDropItemPackage = ia;
                    //cout << "DeathBox : " << ia << endl;
                }


                if (Gres == "Dacia_A_01_v2_C") {
                    Vehicle::Dacia1 = ia;
                }
                if (Gres == "Dacia_A_02_v2_C") {
                    Vehicle::Dacia2 = ia;
                }
                if (Gres == "Dacia_A_03_v2_C") {
                    Vehicle::Dacia3 = ia;
                }
                if (Gres == "Dacia_A_04_v2_C") {
                    Vehicle::Dacia4 = ia;
                }

                if (Gres == "BP_Scooter_01_A_C") {
                    Vehicle::Scooter1 = ia;
                }
                if (Gres == "BP_Scooter_02_A_C") {
                    Vehicle::Scooter2 = ia;
                }
                if (Gres == "BP_Scooter_03_A_C") {
                    Vehicle::Scooter3 = ia;
                }
                if (Gres == "BP_Scooter_04_A_C") {
                    Vehicle::Scooter4 = ia;
                }

                if (Gres == "BP_TukTukTuk_A_01_C") {
                    Vehicle::TukTukTuk1 = ia;
                }
                if (Gres == "BP_TukTukTuk_A_02_C") {
                    Vehicle::TukTukTuk2 = ia;
                }
                if (Gres == "BP_TukTukTuk_A_03_C") {
                    Vehicle::TukTukTuk3 = ia;
                }
                if (Gres == "BP_TukTukTuk_B_01_C") {
                    Vehicle::TukTukTuk4 = ia;
                }

                if (Gres == "BP_MiniBus_C") {
                    Vehicle::MiniBus = ia;
                }
                    
                if (Gres == "Uaz_A_01_C") {
                    Vehicle::Uaz1 = ia;
                }
                if (Gres == "Uaz_A_02_C") {
                    Vehicle::Uaz2 = ia;
                }
                if (Gres == "Uaz_A_03_C") {
                    Vehicle::Uaz3 = ia;
                }
                if (Gres == "Uaz_A_04_C") {
                    Vehicle::Uaz4 = ia;
                }
                if (Gres == "Uaz_B_01_C") {
                    Vehicle::Uaz5 = ia;
                }
                if (Gres == "Uaz_B_02_C") {
                    Vehicle::Uaz6 = ia;
                }
                if (Gres == "Uaz_B_03_C") {
                    Vehicle::Uaz7 = ia;
                }
                if (Gres == "Uaz_B_04_C") {
                    Vehicle::Uaz8 = ia;
                }

                if (Gres == "BP_PickupTruck_A_01_C") {
                    Vehicle::Pickup1 = ia;
                }
                if (Gres == "BP_PickupTruck_A_02_C") {
                    Vehicle::Pickup2 = ia;
                }
                if (Gres == "BP_PickupTruck_A_03_C") {
                    Vehicle::Pickup3 = ia;
                }
                if (Gres == "BP_PickupTruck_A_04_C") {
                    Vehicle::Pickup4 = ia;
                }
                if (Gres == "BP_PickupTruck_A_05_C") {
                    Vehicle::Pickup5 = ia;
                }
                if (Gres == "BP_PickupTruck_B_01_C") {
                    Vehicle::Pickup6 = ia;
                }
                if (Gres == "BP_PickupTruck_B_02_C") {
                    Vehicle::Pickup7 = ia;
                }
                if (Gres == "BP_PickupTruck_B_03_C") {
                    Vehicle::Pickup8 = ia;
                }
                if (Gres == "BP_PickupTruck_B_04_C") {
                    Vehicle::Pickup9 = ia;
                }
                if (Gres == "ABP_PickupTruck_C") {
                    Vehicle::Pickup10 = ia;
                }

                if (Gres == "BP_M_Rony_A_01_C") {
                    Vehicle::Rony1 = ia;
                }
                if (Gres == "BP_M_Rony_A_02_C") {
                    Vehicle::Rony2 = ia;
                }
                if (Gres == "BP_M_Rony_A_03_C") {
                    Vehicle::Rony3 = ia;
                }
                if (Gres == "BP_M_Rony_A_04_C") {
                    Vehicle::Rony4 = ia;
                }

                if (Gres == "BP_Niva_01_C") {
                    Vehicle::Niva1 = ia;
                }
                if (Gres == "BP_Niva_02_C") {
                    Vehicle::Niva2 = ia;
                }
                if (Gres == "BP_Niva_03_C") {
                    Vehicle::Niva3 = ia;
                }
                if (Gres == "BP_Niva_04_C") {
                    Vehicle::Niva4 = ia;
                }

                if (Gres == "Boat_PG117_C") {
                    Vehicle::Boat1 = ia;
                }
                if (Gres == "PG117_A_01_C") {
                    Vehicle::Boat2 = ia;
                }
                if (Gres == "PG117_A_02_C") {
                    Vehicle::Boat3 = ia;
                }

                if (Gres == "Buggy_A_01_C") {
                    Vehicle::Buggy1 = ia;
                }
                if (Gres == "Buggy_A_02_C") {
                    Vehicle::Buggy2 = ia;
                }
                if (Gres == "Buggy_A_03_C") {
                    Vehicle::Buggy3 = ia;
                }
                if (Gres == "Buggy_A_04_C") {
                    Vehicle::Buggy4 = ia;
                }
                if (Gres == "Buggy_A_05_C") {
                    Vehicle::Buggy5 = ia;
                }
                if (Gres == "Buggy_A_06_C") {
                    Vehicle::Buggy6 = ia;
                }

                if (Gres == "BP_Motorbike_01_C") {
                    Vehicle::Bike1 = ia;
                }
                if (Gres == "BP_Motorbike_02_C") {
                    Vehicle::Bike2 = ia;
                }
                if (Gres == "BP_Motorbike_03_C") {
                    Vehicle::Bike3 = ia;
                }
                if (Gres == "BP_Motorbike_04_C") {
                    Vehicle::Bike4 = ia;
                }
                if (Gres == "BP_Motorbike_01_Desert_C") {
                    Vehicle::Bike5 = ia;
                }
                if (Gres == "BP_Motorbike_02_Desert_C") {
                    Vehicle::Bike6 = ia;
                }
                if (Gres == "BP_Motorbike_03_Desert_C") {
                    Vehicle::Bike7 = ia;
                }
                if (Gres == "BP_Motorbike_04_Desert_C") {
                    Vehicle::Bike8 = ia;
                }
                if (Gres == "ABP_Motorbike_04_C") {
                    Vehicle::Bike9 = ia;
                }
                if (Gres == "ABP_Motorbike_03_C") {
                    Vehicle::Bike10 = ia;
                }
                if (Gres == "ABP_Motorbike_02_C") {
                    Vehicle::Bike11 = ia;
                }
                if (Gres == "ABP_Motorbike_01_C") {
                    Vehicle::Bike12 = ia;
                }

                if (Gres == "BP_Motorbike_04_SideCar") {
                    Vehicle::TBike1 = ia;
                }
                if (Gres == "BP_Motorbike_04_SideCar_C") {
                    Vehicle::TBike2 = ia;
                }
                if (Gres == "BP_Motorbike_04_SideCar_Desert") {
                    Vehicle::TBike3 = ia;
                }
                if (Gres == "BP_Motorbike_04_SideCar_Desert_C") {
                    Vehicle::TBike4 = ia;
                }
                if (Gres == "ABP_Motorbike_04_Sidecar") {
                    Vehicle::TBike5 = ia;
                }
                if (Gres == "ABP_Motorbike_04_Sidecar_C") {
                    Vehicle::TBike6 = ia;
                }

                if (Gres == "BP_Mirado_A_01_C") {
                    Vehicle::Mirado1 = ia;
                }
                if (Gres == "BP_Mirado_A_02_C") {
                    Vehicle::Mirado2 = ia;
                }
                if (Gres == "BP_Mirado_A_03_C") {
                    Vehicle::Mirado3 = ia;
                }
                if (Gres == "BP_Mirado_A_04_C") {
                    Vehicle::Mirado4 = ia;
                }
                if (Gres == "BP_Mirado_Open_01_C") {
                    Vehicle::Mirado5 = ia;
                }
                if (Gres == "BP_Mirado_Open_02_C") {
                    Vehicle::Mirado6 = ia;
                }
                if (Gres == "BP_Mirado_Open_03_C") {
                    Vehicle::Mirado7 = ia;
                }
                if (Gres == "BP_Mirado_Open_04_C") {
                    Vehicle::Mirado8 = ia;
                }

                if (Gres == "AquaRail_A_01_C") {
                    Vehicle::AquaRail1 = ia;
                }
                if (Gres == "AquaRail_A_02_C") {
                    Vehicle::AquaRail2 = ia;
                }
                if (Gres == "AquaRail_A_03_C") {
                    Vehicle::AquaRail3 = ia;
                }
                if (Gres == "AquaRail_A_01") {
                    Vehicle::AquaRail4 = ia;
                }
                if (Gres == "AquaRail_C") {
                    Vehicle::AquaRail5 = ia;
                }

                if (Gres == "BP_BRDM_C") {
                    Vehicle::BRDM = ia;
                }
                if (Gres == "BP_Motorglider_C") {
                    Vehicle::Motorglider = ia;
                }


                if (Gres == "fail") {
                    GNamesStr.push_back(Gres);
                    fails++;
                }
                else {
                    fails = 0;
                    GNamesStr.push_back(Gres);
                }

                if (ia > 300000) {
                    //file.close();
                    break;
                }
                ia++;
            }
#pragma endregion
            GNames = gnames;
            return gnames;
        }
        exit(1);
        return 0;
    }

    DWORD DecryptCIndex(DWORD value)
    {
        DWORD v10 = _rotl(value ^ 0xCB56D1FB, 0x0A);
        DWORD result = v10 ^ (v10 << 0x10) ^ 0x56DDD05C;
        return result;
    }

    void UpdateUData()
    {
        if (read) {
            LocalPlayer = mem->RVM<uint64_t>(BASE + __OFFSET__LocalPlayer);
            // cout << "LocalPlayer : " << (LPVOID)LocalPlayer << endl;
            LocalPlayer_Controller = Decrypt_Shield(mem->RVM<uint64_t>(LocalPlayer + __OFFSET__PlayerController));
            //cout << "LocalPlayer Controller : " << (LPVOID)LocalPlayer_Controller << endl;
            LocalPlayer_Pawn = Decrypt_Shield(mem->RVM<uint64_t>(LocalPlayer_Controller + __OFFSET__Pawn));
            LocalPlayer_WeaponProcessor = mem->RVM<uint64_t>(LocalPlayer_Pawn + __OFFSET__WeaponProcessor);

            LocalPlayer_EquippedWeapon = mem->RVM<uint64_t>(LocalPlayer_WeaponProcessor + __OFFSET_WEAPONPROCESSER_EQUIPPEDWEAPONS);
            //cout << "CurrentWeaponIndex : " << (int)CurrentWeaponIndex << endl;
            CurrentWeaponIndex = mem->RVM<BYTE>(LocalPlayer_WeaponProcessor + __OFFSET_WEAPONPROCESSER_EQUIPPEDWEAPONS + __OFFSET_WEAPONPROCESSER_WEAPONINDEX);
            LocalPlayer_CurrentWeapon = mem->RVM<uint64_t>(LocalPlayer_EquippedWeapon + (uint64_t(CurrentWeaponIndex) * 0x8));

            LocalPlayer_WeaponData = mem->RVM<uint64_t>(LocalPlayer_CurrentWeapon + __OFFSET_WEAPON_TRAJECTORYDATA);
            BulletSpeedVal = mem->RVM<float>(LocalPlayer_WeaponData + __OFFSET_TRAJECTORYDATA_CONFIG);
            //cout << "BulletSpeedVal : " << hex << BulletSpeedVal << endl;
            //cout << (LPVOID)LocalPlayer_WeaponData << endl;
            VDragCoefficient = mem->RVM<float>(LocalPlayer_WeaponData + __OFFSET_TRAJECTORYDATA_CONFIG + __OFFSET_TRAJECTORYCONFIG_VDRAG_COEFICIENT);
            //cout << "VDragCoefficient : " << VDragCoefficient << endl;

            ActorTArray = Decrypt_Shield(mem->RVM<uint64_t>(ULevel + __OFFSET__Actors));
            //cout << "ActorTArray : " << (LPVOID)ActorTArray << endl;
            ActorPTR = mem->RVM<uint64_t>(ActorTArray + 0x0);
            //cout << "ActorPTR : " << (LPVOID)ActorPTR << endl;
            ActorCount = mem->RVM<int>(ActorTArray + 0x8);
            //cout << "ActorCount : " << ActorCount << endl;
            LocalPlayer_Pawn_Mesh = mem->RVM<uint64_t>(LocalPlayer_Pawn + __OFFSET__ActorMesh);
            CameraManager = mem->RVM<uint64_t>(LocalPlayer_Controller + __OFFSET__CameraManager);
            SC = mem->RVM<int>(LocalPlayer_Pawn + __OFFSET__SPECTATED_COUNT);
            if (SC >= 1 && SC <= 100)
                SpectatedCount = SC;
            else
                SpectatedCount = 0;

            int getnumber = mem->RVM<int>(LocalPlayer_Pawn + __OFFSET__ActorTeam);
            if (getnumber <= 100)
                mynumber = getnumber;
            else if (getnumber > 100 && getnumber < 100100)
                mynumber = getnumber - 100000;
        }
    }


    void UpdateSway() {
        uint64_t LocalPlayerMesh = mem->RVM<uint64_t>(LocalPlayer_Pawn + __OFFSET__ActorMesh);

        uint64_t LocalAnimInst_PTR = mem->RVM<uint64_t>(LocalPlayerMesh + __OFFSET__AnimInst);

        Vector3 Recoil = mem->RVM<Vector3>(LocalAnimInst_PTR + __OFFSET__RecoilAdsRotation_CP);

        Vector3 Sway = mem->RVM<Vector3>(LocalAnimInst_PTR + __OFFSET__ControlRotation_CP);
        AimCamera.POV.FOV = Camera.POV.FOV;
        AimCamera.POV.Location = Camera.POV.Location;
        AimCamera.POV.Rotation = Sway + Recoil;
    }

    void UpdateCamera()
    {
        float Camera_FOV = mem->RVM<float>(CameraManager + __OFFSET__CameraCache_FOV);
        //printf("FOV : %f\n", Camera_FOV);
        Vector3 Camera_Location = mem->RVM<Vector3>(CameraManager + __OFFSET__CameraCache_Loc);
        //printf("Loc X : %f Y : %f Z : %f\n", Camera_Location.x, Camera_Location.y, Camera_Location.z);
        Vector3 Camera_Rotation = mem->RVM<Vector3>(CameraManager + __OFFSET__CameraCache_Rot);
        //printf("Rot X : %f Y : %f Z : %f\n", Camera_Rotation.x, Camera_Rotation.y, Camera_Rotation.z);
        Camera.POV.FOV = Camera_FOV;
        Camera.POV.Location = Camera_Location;
        Camera.POV.Rotation = Camera_Rotation;
    }

    uint64_t LocationPTR;

    string getName(int id) {
        string returnv = string(to_string(GNamesStr.size()));
        if (GNamesStr.size() > id)
            returnv = GNamesStr[id];

        return returnv;
    }

    void UpdateActors()
    {
        vector<AActor> tempActors;
        for (int i = 0; i < ActorCount; i++)
        {
            AActor tempActor;

            uint64_t CurrentActorPTR = mem->RVM<uint64_t>(ActorPTR + i * 8);

            uint64_t ActorID = DecryptCIndex(mem->RVM<int>(CurrentActorPTR + __OFFSET__ActorID));
            if (CurrentActorPTR != NULL) {
                if (!cfg->DebugMode && ActorID != Vehicle::PlayerFemale && 
                    ActorID != Vehicle::PlayerMale && ActorID != Vehicle::BotPlayerFemale && ActorID != Vehicle::BotPlayerMale && ActorID != Vehicle::RegistedPlayer && ActorID != Vehicle::AIPawnBase &&
                    ActorID != Loot::DeathDropItemPackage && ActorID != Loot::Carapackage_RedBox && ActorID != Loot::Carapackage_RedBox1 && ActorID != Loot::DroppedItemGroup &&
                    Vehicle::Dacia1&& ActorID != Vehicle::Dacia2&& ActorID != Vehicle::Dacia3&& ActorID != Vehicle::Dacia4 &&
                    ActorID != Vehicle::Scooter1&& ActorID != Vehicle::Scooter2&& ActorID != Vehicle::Scooter3&& ActorID != Vehicle::Scooter4 &&
                    ActorID != Vehicle::TukTukTuk1&& ActorID != Vehicle::TukTukTuk2&& ActorID != Vehicle::TukTukTuk3&& ActorID != Vehicle::TukTukTuk4 &&
                    ActorID != Vehicle::MiniBus &&
                    ActorID != Vehicle::Uaz1&& ActorID != Vehicle::Uaz2&& ActorID != Vehicle::Uaz3&& ActorID != Vehicle::Uaz4&& ActorID != Vehicle::Uaz5&& ActorID != Vehicle::Uaz6&& ActorID != Vehicle::Uaz7&& ActorID != Vehicle::Uaz8&&
                    ActorID != Vehicle::Pickup1&& ActorID != Vehicle::Pickup2&& ActorID != Vehicle::Pickup3&& ActorID != Vehicle::Pickup4&& ActorID != Vehicle::Pickup5&& ActorID != Vehicle::Pickup6&& ActorID != Vehicle::Pickup7&& ActorID != Vehicle::Pickup8&& ActorID != Vehicle::Pickup9&& ActorID != Vehicle::Pickup10&&
                    ActorID != Vehicle::Rony1&& ActorID != Vehicle::Rony2&& ActorID != Vehicle::Rony3&& ActorID != Vehicle::Rony4&&
                    ActorID != Vehicle::Niva1&& ActorID != Vehicle::Niva2&& ActorID != Vehicle::Niva3&& ActorID != Vehicle::Niva4&&
                    ActorID != Vehicle::Boat1&& ActorID != Vehicle::Boat2&& ActorID != Vehicle::Boat3&&
                    ActorID != Vehicle::Buggy1&& ActorID != Vehicle::Buggy2&& ActorID != Vehicle::Buggy3&& ActorID != Vehicle::Buggy4&& ActorID != Vehicle::Buggy5&& ActorID != Vehicle::Buggy6&&
                    ActorID != Vehicle::Bike1&& ActorID != Vehicle::Bike2&& ActorID != Vehicle::Bike3&& ActorID != Vehicle::Bike4&& ActorID != Vehicle::Bike5&& ActorID != Vehicle::Bike6&& ActorID != Vehicle::Bike7&& ActorID != Vehicle::Bike8&& ActorID != Vehicle::Bike9&& ActorID != Vehicle::Bike10&& ActorID != Vehicle::Bike11&& ActorID != Vehicle::Bike12&&
                    ActorID != Vehicle::TBike1&& ActorID != Vehicle::TBike2&& ActorID != Vehicle::TBike3&& ActorID != Vehicle::TBike4&& ActorID != Vehicle::TBike5&& ActorID != Vehicle::TBike6&&
                    ActorID != Vehicle::Mirado1&& ActorID != Vehicle::Mirado2&& ActorID != Vehicle::Mirado3&& ActorID != Vehicle::Mirado4&& ActorID != Vehicle::Mirado5&& ActorID != Vehicle::Mirado6&& ActorID != Vehicle::Mirado7&& ActorID != Vehicle::Mirado8&&
                    ActorID != Vehicle::AquaRail1&& ActorID != Vehicle::AquaRail2&& ActorID != Vehicle::AquaRail3&& ActorID != Vehicle::AquaRail4&& ActorID != Vehicle::AquaRail5&&
                    ActorID != Vehicle::BRDM&&
                    ActorID != Vehicle::Motorglider)
                    continue;

                if (ActorID == Vehicle::PlayerFemale || ActorID == Vehicle::PlayerMale)
                    tempActor.Type = Player;
                if (ActorID == Vehicle::RegistedPlayer || ActorID == Vehicle::AIPawnBase || ActorID == Vehicle::BotPlayerFemale || ActorID == Vehicle::BotPlayerMale)
                    tempActor.Type = Player;
                if (ActorID == Loot::DeathDropItemPackage)
                    tempActor.Type = DeathBox;
                if (ActorID == Loot::DroppedItemGroup)
                    tempActor.Type = Item;
                if (ActorID == Loot::Carapackage_RedBox)
                    tempActor.Type = AirDrop;
                if (ActorID == ItemGroup::ProjGrenade)
                    tempActor.Type = Grenades;
                if (ActorID == Vehicle::Dacia1 || ActorID == Vehicle::Dacia2 || ActorID == Vehicle::Dacia3 || ActorID == Vehicle::Dacia4 ||
                    ActorID == Vehicle::Scooter1 || ActorID == Vehicle::Scooter2 || ActorID == Vehicle::Scooter3 || ActorID == Vehicle::Scooter4 ||
                    ActorID == Vehicle::TukTukTuk1 || ActorID == Vehicle::TukTukTuk2 || ActorID == Vehicle::TukTukTuk3 || ActorID == Vehicle::TukTukTuk4 ||
                    ActorID == Vehicle::MiniBus ||
                    ActorID == Vehicle::Uaz1 || ActorID == Vehicle::Uaz2 || ActorID == Vehicle::Uaz3 || ActorID == Vehicle::Uaz4 || ActorID == Vehicle::Uaz5 || ActorID == Vehicle::Uaz6 || ActorID == Vehicle::Uaz7 || ActorID == Vehicle::Uaz8 ||
                    ActorID == Vehicle::Pickup1 || ActorID == Vehicle::Pickup2 || ActorID == Vehicle::Pickup3 || ActorID == Vehicle::Pickup4 || ActorID == Vehicle::Pickup5 || ActorID == Vehicle::Pickup6 || ActorID == Vehicle::Pickup7 || ActorID == Vehicle::Pickup8 || ActorID == Vehicle::Pickup9 || ActorID == Vehicle::Pickup10 ||
                    ActorID == Vehicle::Rony1 || ActorID == Vehicle::Rony2 || ActorID == Vehicle::Rony3 || ActorID == Vehicle::Rony4 ||
                    ActorID == Vehicle::Niva1 || ActorID == Vehicle::Niva2 || ActorID == Vehicle::Niva3 || ActorID == Vehicle::Niva4 ||
                    ActorID == Vehicle::Boat1 || ActorID == Vehicle::Boat2 || ActorID == Vehicle::Boat3 ||
                    ActorID == Vehicle::Buggy1 || ActorID == Vehicle::Buggy2 || ActorID == Vehicle::Buggy3 || ActorID == Vehicle::Buggy4 || ActorID == Vehicle::Buggy5 || ActorID == Vehicle::Buggy6 ||
                    ActorID == Vehicle::Bike1 || ActorID == Vehicle::Bike2 || ActorID == Vehicle::Bike3 || ActorID == Vehicle::Bike4 || ActorID == Vehicle::Bike5 || ActorID == Vehicle::Bike6 || ActorID == Vehicle::Bike7 || ActorID == Vehicle::Bike8 || ActorID == Vehicle::Bike9 || ActorID == Vehicle::Bike10 || ActorID == Vehicle::Bike11 || ActorID == Vehicle::Bike12 ||
                    ActorID == Vehicle::TBike1 || ActorID == Vehicle::TBike2 || ActorID == Vehicle::TBike3 || ActorID == Vehicle::TBike4 || ActorID == Vehicle::TBike5 || ActorID == Vehicle::TBike6 ||
                    ActorID == Vehicle::Mirado1 || ActorID == Vehicle::Mirado2 || ActorID == Vehicle::Mirado3 || ActorID == Vehicle::Mirado4 || ActorID == Vehicle::Mirado5 || ActorID == Vehicle::Mirado6 || ActorID == Vehicle::Mirado7 || ActorID == Vehicle::Mirado8 ||
                    ActorID == Vehicle::AquaRail1 || ActorID == Vehicle::AquaRail2 || ActorID == Vehicle::AquaRail3 || ActorID == Vehicle::AquaRail4 || ActorID == Vehicle::AquaRail5 ||
                    ActorID == Vehicle::BRDM ||
                    ActorID == Vehicle::Motorglider)
                    tempActor.Type = Vehicles;
                uint64_t RootComponent = Decrypt_Shield(mem->RVM<uint64_t>(CurrentActorPTR + __OFFSET__RootComponent));
                uint64_t MeshComponent = mem->RVM<uint64_t>(CurrentActorPTR + __OFFSET__ActorMesh);
                int ReadTeamNumber;
                int TeamNumber = mem->RVM<int>(CurrentActorPTR + __OFFSET__ActorTeam);
                if (TeamNumber >= 0) {
                    if (TeamNumber <= 100)
                        ReadTeamNumber = TeamNumber;
                    else if (TeamNumber > 100 && TeamNumber < 100100)
                        ReadTeamNumber = TeamNumber - 100000;
                }
                string objectname = getName(ActorID);
                tempActor.AActor_Base = CurrentActorPTR;
                tempActor.RootComponent = RootComponent;
                tempActor.MeshComponent = MeshComponent;
                tempActor.ActorID = ActorID;
                tempActor.TeamNum = ReadTeamNumber;
                tempActor.ObjectName = objectname;
                tempActors.push_back(tempActor);
            }
        }
        Actors = tempActors;
    }
}
