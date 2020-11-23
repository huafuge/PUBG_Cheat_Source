#pragma once
#include <iostream> //Standard IO
#include <Windows.h> //Windows Functions
#include "Memory.h"
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <string>
#include <cstdarg>
#include <Psapi.h>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <list>
#include "offsets.h"
#pragma comment(lib, "d3dx9.lib") 
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "Dwmapi.lib") 

#define M_PI	3.14159265358979323846264338327950288419716939937510

using namespace std;

struct FVector2D
{
    FLOAT X, Y;

    FVector2D() {}

    FVector2D(FLOAT X, FLOAT Y)
    {
        this->X = X;
        this->Y = Y;
    }

    FVector2D operator - () { return FVector2D(-X, -Y); }

    FVector2D operator + (FVector2D v) { return FVector2D(X + v.X, Y + v.Y); }
    FVector2D operator - (FVector2D v) { return FVector2D(X - v.X, Y - v.Y); }
    FVector2D operator * (FVector2D v) { return FVector2D(X * v.X, Y * v.Y); }
    FVector2D operator / (FVector2D v) { return FVector2D(X / v.X, Y / v.Y); }

    FVector2D operator = (FVector2D v)
    {
        this->X = v.X;
        this->Y = v.Y;

        return *this;
    }

    FVector2D operator += (FVector2D v)
    {
        this->X += v.X;
        this->Y += v.Y;

        return *this;
    }

    FVector2D operator -= (FVector2D v)
    {
        this->X -= v.X;
        this->Y -= v.Y;

        return *this;
    }

    FVector2D operator /= (FVector2D v)
    {
        this->X /= v.X;
        this->Y /= v.Y;

        return *this;
    }

    FVector2D operator *= (FVector2D v)
    {
        this->X *= v.X;
        this->Y *= v.Y;

        return *this;
    }

    FVector2D operator + (FLOAT f) { return FVector2D(X + f, Y + f); }
    FVector2D operator - (FLOAT f) { return FVector2D(X - f, Y - f); }
    FVector2D operator * (FLOAT f) { return FVector2D(X * f, Y * f); }
    FVector2D operator / (FLOAT f) { return FVector2D(X / f, Y / f); }

    FVector2D operator = (FLOAT f)
    {
        this->X = f;
        this->Y = f;

        return *this;
    }

    FVector2D operator += (FLOAT f)
    {
        this->X += f;
        this->Y += f;

        return *this;
    }

    FVector2D operator -= (FLOAT f)
    {
        this->X -= f;
        this->Y -= f;

        return *this;
    }

    FVector2D operator /= (FLOAT f)
    {
        this->X /= f;
        this->Y /= f;

        return *this;
    }

    FVector2D operator *= (FLOAT f)
    {
        this->X *= f;
        this->Y *= f;

        return *this;
    }

    BOOL operator == (FVector2D v) { return ((X == v.X) && (Y == v.Y)); }
    BOOL operator != (FVector2D v) { return ((X != v.X) || (Y != v.Y)); }

    FLOAT operator [] (INT i)
    {
        if (i == 0)
            return X;
        else
            return Y;
    }

    FLOAT Length() { return sqrtf(X * X + Y * Y); }
};

struct FVector2
{
    FLOAT X, Y, Z;

    FVector2() {}

    FVector2(FLOAT X, FLOAT Y, FLOAT Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
    }

    FVector2 operator - () { return FVector2(-X, -Y, -Z); }

    FVector2 operator + (FVector2 v) { return FVector2(X + v.X, Y + v.Y, Z + v.Z); }
    FVector2 operator - (FVector2 v) { return FVector2(X - v.X, Y - v.Y, Z - v.Z); }
    FVector2 operator * (FVector2 v) { return FVector2(X * v.X, Y * v.Y, Z * v.Z); }
    FVector2 operator / (FVector2 v) { return FVector2(X / v.X, Y / v.Y, Z / v.Z); }

    FVector2 operator = (FVector2 v)
    {
        this->X = v.X;
        this->Y = v.Y;
        this->Z = v.Z;

        return *this;
    }

    FVector2 operator += (FVector2 v)
    {
        this->X += v.X;
        this->Y += v.Y;
        this->Z += v.Z;

        return *this;
    }

    FVector2 operator -= (FVector2 v)
    {
        this->X -= v.X;
        this->Y -= v.Y;
        this->Z -= v.Z;

        return *this;
    }

    FVector2 operator /= (FVector2 v)
    {
        this->X /= v.X;
        this->Y /= v.Y;
        this->Z /= v.Z;

        return *this;
    }

    FVector2 operator *= (FVector2 v)
    {
        this->X *= v.X;
        this->Y *= v.Y;
        this->Z *= v.Z;

        return *this;
    }

    FVector2 operator + (FLOAT f) { return FVector2(X + f, Y + f, Z + f); }
    FVector2 operator - (FLOAT f) { return FVector2(X - f, Y - f, Z - f); }
    FVector2 operator * (FLOAT f) { return FVector2(X * f, Y * f, Z * f); }
    FVector2 operator / (FLOAT f) { return FVector2(X / f, Y / f, Z / f); }

    FVector2 operator = (FLOAT f)
    {
        this->X = f;
        this->Y = f;
        this->Z = f;

        return *this;
    }

    FVector2 operator += (FLOAT f)
    {
        this->X += f;
        this->Y += f;
        this->Z += f;

        return *this;
    }

    FVector2 operator -= (FLOAT f)
    {
        this->X -= f;
        this->Y -= f;
        this->Z -= f;

        return *this;
    }

    FVector2 operator /= (FLOAT f)
    {
        this->X /= f;
        this->Y /= f;
        this->Z /= f;

        return *this;
    }

    FVector2 operator *= (FLOAT f)
    {
        this->X *= f;
        this->Y *= f;
        this->Z *= f;

        return *this;
    }

    BOOL operator == (FVector2 v) { return ((X == v.X) && (Y == v.Y) && (Z == v.Z)); }
    BOOL operator != (FVector2 v) { return ((X != v.X) || (Y != v.Y) || (Z != v.Z)); }

    FLOAT operator [] (INT i)
    {
        if (i == 0)
            return X;
        else if (i == 1)
            return Y;
        else
            return Z;
    }

    FLOAT Length() { return sqrtf(X * X + Y * Y + Z * Z); }
};

struct FRotator
{
    INT Pitch, Yaw, Roll;

    FRotator() {}

    FRotator(INT Pitch, INT Yaw, INT Roll)
    {
        this->Pitch = Pitch;
        this->Yaw = Yaw;
        this->Roll = Roll;
    }

    FRotator operator - () { return FRotator(-Pitch, -Yaw, -Roll); }

    FRotator operator + (FRotator r) { return FRotator(Pitch + r.Pitch, Yaw + r.Yaw, Roll + r.Roll); }
    FRotator operator - (FRotator r) { return FRotator(Pitch - r.Pitch, Yaw - r.Yaw, Roll - r.Roll); }
    FRotator operator * (FRotator r) { return FRotator(Pitch * r.Pitch, Yaw * r.Yaw, Roll * r.Roll); }
    FRotator operator / (FRotator r) { return FRotator(Pitch / r.Pitch, Yaw / r.Yaw, Roll / r.Roll); }

    FRotator operator = (FRotator r)
    {
        this->Pitch = r.Pitch;
        this->Yaw = r.Yaw;
        this->Roll = r.Roll;

        return *this;
    }

    FRotator operator += (FRotator r)
    {
        this->Pitch += r.Pitch;
        this->Yaw += r.Yaw;
        this->Roll += r.Roll;

        return *this;
    }

    FRotator operator -= (FRotator r)
    {
        this->Pitch -= r.Pitch;
        this->Yaw -= r.Yaw;
        this->Roll -= r.Roll;

        return *this;
    }

    FRotator operator /= (FRotator r)
    {
        this->Pitch /= r.Pitch;
        this->Yaw /= r.Yaw;
        this->Roll /= r.Roll;

        return *this;
    }

    FRotator operator *= (FRotator r)
    {
        this->Pitch *= r.Pitch;
        this->Yaw *= r.Yaw;
        this->Roll *= r.Roll;

        return *this;
    }

    FRotator operator + (FLOAT f) { return FRotator(Pitch + f, Yaw + f, Roll + f); }
    FRotator operator - (FLOAT f) { return FRotator(Pitch - f, Yaw - f, Roll - f); }
    FRotator operator * (FLOAT f) { return FRotator(Pitch * f, Yaw * f, Roll * f); }
    FRotator operator / (FLOAT f) { return FRotator(Pitch / f, Yaw / f, Roll / f); }

    FRotator operator = (FLOAT f)
    {
        this->Pitch = f;
        this->Yaw = f;
        this->Roll = f;

        return *this;
    }

    FRotator operator += (FLOAT f)
    {
        this->Pitch += f;
        this->Yaw += f;
        this->Roll += f;

        return *this;
    }

    FRotator operator -= (FLOAT f)
    {
        this->Pitch -= f;
        this->Yaw -= f;
        this->Roll -= f;

        return *this;
    }

    FRotator operator /= (FLOAT f)
    {
        this->Pitch /= f;
        this->Yaw /= f;
        this->Roll /= f;

        return *this;
    }

    FRotator operator *= (FLOAT f)
    {
        this->Pitch *= f;
        this->Yaw *= f;
        this->Roll *= f;

        return *this;
    }

    BOOL operator == (FRotator r) { return ((Pitch == r.Pitch) && (Yaw == r.Yaw) && (Roll == r.Roll)); }
    BOOL operator != (FRotator r) { return ((Pitch != r.Pitch) || (Yaw != r.Yaw) || (Roll != r.Roll)); }

    FLOAT operator [] (INT i)
    {
        if (i == 0)
            return Pitch;
        else if (i == 1)
            return Yaw;
        else
            return Roll;
    }
};

class Vector2
{
public:
    float x;
    float y;

    Vector2() : x(0.f), y(0.f)
    { }

    Vector2(float _x, float _y) : x(_x), y(_y)
    {

    }
    ~Vector2() {

    }

    inline float Distance(Vector2 vector)
    {
        float dx = vector.x - x;
        float dy = vector.y - y;
        return (float)sqrt(dx * dx + dy * dy);
    }


    inline float XDistance(Vector2 vector)
    {
        float dx = vector.x - x;
        return (float)sqrt(dx * dx);
    }

    inline float YDistance(Vector2 vector)
    {
        float dy = vector.y - y;
        return (float)sqrt(dy * dy);
    }

    inline float Length()
    {
        return (float)sqrt((double)(x * x + y * y));
    }
};

class Vector3
{
public:
    float x, y, z;
    Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
    //Vector(const FVector& v) :x(v.X), y(v.Y), z(v.Z) {}

    float InnerProduct(const Vector3& v) const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    Vector3 OuterProduct(const Vector3& v) const {
        Vector3 output;
        output.x = (y * v.z) - (z * v.y);
        output.y = (z * v.x) - (x * v.z);
        output.z = (x * v.y) - (y * v.x);
        return output;
    }

    Vector3 Min(const Vector3& v) const {
        Vector3 output;
        output.x = x < v.x ? x : v.x;
        output.y = y < v.y ? y : v.y;
        output.z = z < v.z ? z : v.z;
        return output;
    }

    Vector3 Max(const Vector3& v) const {
        Vector3 output;
        output.x = x > v.x ? x : v.x;
        output.y = y > v.y ? y : v.y;
        output.z = z > v.z ? z : v.z;
        return output;
    }

    bool operator == (const Vector3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator != (const Vector3& v) const {
        return !(*this == v);
    }

    Vector3 operator - () const {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator + (const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator - (const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator * (const float fValue) const {
        return Vector3(x * fValue, y * fValue, z * fValue);
    }

    void Normalize() {
        float factor = 1.0f / sqrtf(x * x + y * y + z * z);
        (*this) = (*this) * factor;
    }

    float Length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    float Distance(const Vector3& v) const {
        return (v - *this).Length();
    }

    Vector3 operator ^ (const Vector3& v) const {
        return OuterProduct(v);
    }

    float operator * (const Vector3& v) const {
        return InnerProduct(v);
    }
};

/*
struct FVector
{
    FLOAT X, Y, Z;

    FVector() {}

    FVector(FLOAT X, FLOAT Y, FLOAT Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
    }

    FVector operator - () { return FVector(-X, -Y, -Z); }

    FVector operator + (FVector v) { return FVector(X + v.X, Y + v.Y, Z + v.Z); }
    FVector operator - (FVector v) { return FVector(X - v.X, Y - v.Y, Z - v.Z); }
    FVector operator * (FVector v) { return FVector(X * v.X, Y * v.Y, Z * v.Z); }
    FVector operator / (FVector v) { return FVector(X / v.X, Y / v.Y, Z / v.Z); }

    FVector operator = (FVector v)
    {
        this->X = v.X;
        this->Y = v.Y;
        this->Z = v.Z;

        return *this;
    }

    FVector operator += (FVector v)
    {
        this->X += v.X;
        this->Y += v.Y;
        this->Z += v.Z;

        return *this;
    }

    FVector operator -= (FVector v)
    {
        this->X -= v.X;
        this->Y -= v.Y;
        this->Z -= v.Z;

        return *this;
    }

    FVector operator /= (FVector v)
    {
        this->X /= v.X;
        this->Y /= v.Y;
        this->Z /= v.Z;

        return *this;
    }

    FVector operator *= (FVector v)
    {
        this->X *= v.X;
        this->Y *= v.Y;
        this->Z *= v.Z;

        return *this;
    }

    FVector operator + (FLOAT f) { return FVector(X + f, Y + f, Z + f); }
    FVector operator - (FLOAT f) { return FVector(X - f, Y - f, Z - f); }
    FVector operator * (FLOAT f) { return FVector(X * f, Y * f, Z * f); }
    FVector operator / (FLOAT f) { return FVector(X / f, Y / f, Z / f); }

    FVector operator = (FLOAT f)
    {
        this->X = f;
        this->Y = f;
        this->Z = f;

        return *this;
    }

    FVector operator += (FLOAT f)
    {
        this->X += f;
        this->Y += f;
        this->Z += f;

        return *this;
    }

    FVector operator -= (FLOAT f)
    {
        this->X -= f;
        this->Y -= f;
        this->Z -= f;

        return *this;
    }

    FVector operator /= (FLOAT f)
    {
        this->X /= f;
        this->Y /= f;
        this->Z /= f;

        return *this;
    }

    FVector operator *= (FLOAT f)
    {
        this->X *= f;
        this->Y *= f;
        this->Z *= f;

        return *this;
    }

    BOOL operator == (FVector v) { return ((X == v.X) && (Y == v.Y) && (Z == v.Z)); }
    BOOL operator != (FVector v) { return ((X != v.X) || (Y != v.Y) || (Z != v.Z)); }

    FLOAT operator [] (INT i)
    {
        if (i == 0)
            return X;
        else if (i == 1)
            return Y;
        else
            return Z;
    }

    FLOAT Length() { return sqrtf(X * X + Y * Y + Z * Z); }
};
*/

// ScriptStruct CoreUObject.Vector
// 0x000C
struct FVector
{
    float                                              X;                                                        // 0x0000(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)
    float                                              Y;                                                        // 0x0004(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)
    float                                              Z;                                                        // 0x0008(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_SaveGame, CPF_IsPlainOldData)
    operator Vector3() { return Vector3(X, Y, Z); }
};

struct FMinimalViewInfo
{
    class Vector3                                      Location;                                                 // 0x0000(0x000C) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    class Vector3                                      Rotation;                                                 // 0x000C(0x000C) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    float                                              FOV;                                                      // 0x0018(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    float                                              OrthoWidth;                                               // 0x001C(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    float                                              OrthoNearClipPlane;                                       // 0x0020(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    float                                              OrthoFarClipPlane;                                        // 0x0024(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
    float                                              AspectRatio;                                              // 0x0028(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_ZeroConstructor, CPF_IsPlainOldData)
};

struct FCameraCacheEntry
{
    float                                                   TimeStamp;                                                // 0x0000(0x0004) (CPF_ZeroConstructor, CPF_IsPlainOldData)
    unsigned char                                           UnknownData00[0xC];                                       // 0x0004(0x000C) MISSED OFFSET
    struct FMinimalViewInfo                                 POV;
};

struct FQuat
{
    float x;
    float y;
    float z;
    float w;
};

struct FTransform
{
    FQuat rot;
    Vector3 translation;
    char pad[4];
    Vector3 scale;
    char pad1[4];
    D3DMATRIX ToMatrixWithScale()
    {
        D3DMATRIX m;
        m._41 = translation.x;
        m._42 = translation.y;
        m._43 = translation.z;

        float x2 = rot.x + rot.x;
        float y2 = rot.y + rot.y;
        float z2 = rot.z + rot.z;

        float xx2 = rot.x * x2;
        float yy2 = rot.y * y2;
        float zz2 = rot.z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * scale.x;
        m._22 = (1.0f - (xx2 + zz2)) * scale.y;
        m._33 = (1.0f - (xx2 + yy2)) * scale.z;

        float yz2 = rot.y * z2;
        float wx2 = rot.w * x2;
        m._32 = (yz2 - wx2) * scale.z;
        m._23 = (yz2 + wx2) * scale.y;

        float xy2 = rot.x * y2;
        float wz2 = rot.w * z2;
        m._21 = (xy2 - wz2) * scale.y;
        m._12 = (xy2 + wz2) * scale.x;

        float xz2 = rot.x * z2;
        float wy2 = rot.w * y2;
        m._31 = (xz2 + wy2) * scale.z;
        m._13 = (xz2 - wy2) * scale.x;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};

enum Bones : int
{
    Root = 0,
    pelvis = 1,
    spine_01 = 2,
    spine_02 = 3,
    spine_03 = 4,
    neck_01 = 5,
    Head = 6,
    face_root = 7,
    eyebrows_pos_root = 8,
    eyebrows_root = 9,
    eyebrows_r = 10,
    eyebrows_l = 11,
    eyebrow_l = 12,
    eyebrow_r = 13,
    forehead_root = 14,
    forehead = 15,
    jaw_pos_root = 16,
    jaw_root = 17,
    jaw = 18,
    mouth_down_pos_root = 19,
    mouth_down_root = 20,
    lip_bm_01 = 21,
    lip_bm_02 = 22,
    lip_br = 23,
    lip_bl = 24,
    jaw_01 = 25,
    jaw_02 = 26,
    cheek_pos_root = 27,
    cheek_root = 28,
    cheek_r = 29,
    cheek_l = 30,
    nose_side_root = 31,
    nose_side_r_01 = 32,
    nose_side_r_02 = 33,
    nose_side_l_01 = 34,
    nose_side_l_02 = 35,
    eye_pos_r_root = 36,
    eye_r_root = 37,
    eye_rot_r_root = 38,
    eye_lid_u_r = 39,
    eye_r = 40,
    eye_lid_b_r = 41,
    eye_pos_l_root = 42,
    eye_l_root = 43,
    eye_rot_l_root = 44,
    eye_lid_u_l = 45,
    eye_l = 46,
    eye_lid_b_l = 47,
    nose_pos_root = 48,
    nose = 49,
    mouth_up_pos_root = 50,
    mouth_up_root = 51,
    lip_ul = 52,
    lip_um_01 = 53,
    lip_um_02 = 54,
    lip_ur = 55,
    lip_l = 56,
    lip_r = 57,
    hair_root = 58,
    hair_b_01 = 59,
    hair_b_02 = 60,
    hair_l_01 = 61,
    hair_l_02 = 62,
    hair_r_01 = 63,
    hair_r_02 = 64,
    hair_f_02 = 65,
    hair_f_01 = 66,
    hair_b_pt_01 = 67,
    hair_b_pt_02 = 68,
    hair_b_pt_03 = 69,
    hair_b_pt_04 = 70,
    hair_b_pt_05 = 71,
    camera_fpp = 72,
    GunReferencePoint = 73,
    GunRef = 74,
    breast_l = 75,
    breast_r = 76,
    clavicle_l = 77,
    upperarm_l = 113,
    lowerarm_l = 89,
    hand_l = 95,
    thumb_01_l = 81,
    thumb_02_l = 82,
    thumb_03_l = 83,
    thumb_04_l_MBONLY = 84,
    index_01_l = 85,
    index_02_l = 86,
    index_03_l = 87,
    index_04_l_MBONLY = 88,
    middle_01_l = 89,
    middle_02_l = 90,
    middle_03_l = 91,
    middle_04_l_MBONLY = 92,
    ring_01_l = 93,
    ring_02_l = 94,
    ring_03_l = 95,
    ring_04_l_MBONLY = 96,
    pinky_01_l = 97,
    pinky_02_l = 98,
    pinky_03_l = 99,
    pinky_04_l_MBONLY = 100,
    item_l = 101,
    lowerarm_twist_01_l = 102,
    upperarm_twist_01_l = 103,
    clavicle_r = 104,
    upperarm_r = 140,
    lowerarm_r = 116,
    hand_r = 126,
    thumb_01_r = 108,
    thumb_02_r = 109,
    thumb_03_r = 110,
    thumb_04_r_MBONLY = 111,
    index_01_r = 112,
    index_02_r = 113,
    index_03_r = 114,
    index_04_r_MBONLY = 115,
    middle_01_r = 116,
    middle_02_r = 117,
    middle_03_r = 118,
    middle_04_r_MBONLY = 119,
    ring_01_r = 120,
    ring_02_r = 121,
    ring_03_r = 122,
    ring_04_r_MBONLY = 123,
    pinky_01_r = 124,
    pinky_02_r = 125,
    pinky_03_r = 126,
    pinky_04_r_MBONLY = 127,
    item_r = 128,
    lowerarm_twist_01_r = 129,
    upperarm_twist_01_r = 130,
    BackPack = 131,
    backpack_01 = 132,
    backpack_02 = 133,
    Slot_Primary = 134,
    Slot_Secondary = 135,
    Slot_Melee = 136,
    slot_throwable = 137,
    coat_l_01 = 138,
    coat_l_02 = 139,
    coat_l_03 = 140,
    coat_l_04 = 141,
    coat_fl_01 = 142,
    coat_fl_02 = 143,
    coat_fl_03 = 144,
    coat_fl_04 = 145,
    coat_b_01 = 146,
    coat_b_02 = 147,
    coat_b_03 = 148,
    coat_b_04 = 149,
    coat_r_01 = 150,
    coat_r_02 = 151,
    coat_r_03 = 152,
    coat_r_04 = 153,
    coat_fr_01 = 154,
    coat_fr_02 = 155,
    coat_fr_03 = 156,
    coat_fr_04 = 157,
    thigh_l = 168,
    calf_l = 169,
    foot_l = 170,
    ball_l = 161,
    calf_twist_01_l = 162,
    thigh_twist_01_l = 163,
    thigh_r = 174,
    calf_r = 175,
    foot_r = 176,
    ball_r = 167,
    calf_twist_01_r = 168,
    thigh_twist_01_r = 169,
    Slot_SideArm = 170,
    skirt_l_01 = 171,
    skirt_l_02 = 172,
    skirt_l_03 = 173,
    skirt_f_01 = 174,
    skirt_f_02 = 175,
    skirt_f_03 = 176,
    skirt_b_01 = 177,
    skirt_b_02 = 178,
    skirt_b_03 = 179,
    skirt_r_01 = 180,
    skirt_r_02 = 181,
    skirt_r_03 = 182,
    ik_hand_root = 183,
    ik_hand_gun = 184,
    ik_hand_r = 185,
    ik_hand_l = 186,
    ik_aim_root = 187,
    ik_aim_l = 188,
    ik_aim_r = 189,
    ik_foot_root = 190,
    ik_foot_l = 191,
    ik_foot_r = 192,
    camera_tpp = 193,
    ik_target_root = 194,
    ik_target_l = 195,
    ik_target_r = 196,
    VB_spine_03_spine_03 = 197,
    VB_upperarm_r_lowerarm_r = 198
};




//namespace state {
//	extern bool menu;
//	extern bool VehicleESP;
//	extern bool PlayerESP;
//	extern bool Crosshair;
//	extern bool PlayerName;
//	extern bool WeaponESP;
//	extern bool Attachments;
//	extern bool drugs;
//	extern bool Aimbot;
//	extern int HB;
//	extern bool InstantHit;
//	extern int AimKey;
//	extern bool PlayerBoxEsp;
//	extern bool PlayerHeadEsp;
//
//	extern bool DevMode;
//}



extern int ActorIds[4];
extern int uaz[3];
extern int dacia[4];
extern int motorbike[5];
extern int buggy[3];
extern int boat;
extern int itemtype[2];
