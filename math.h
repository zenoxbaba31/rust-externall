#pragma once
#include <math.h>
#include <vector>
#include <memory>

#define pow(n) (n)*(n)
class Vector2
{
public:
    float x, y;

    __forceinline Vector2()
    {
        x = y = 0.0f;
    }

    __forceinline Vector2(float X, float Y)
    {
        x = X; y = Y;
    }


    float distance(Vector2 b)
    {
        const float dx = b.x - x;
        const float dy = b.y - y;
        return sqrt(dx * dx + dy * dy);
    }

    __forceinline Vector2 operator+(float v) const
    {
        return Vector2(x + v, y + v);
    }

    __forceinline Vector2 operator-(float v) const
    {
        return Vector2(x - v, y - v);
    }

    __forceinline Vector2& operator+=(float v)
    {
        x += v; y += v; return *this;
    }

    __forceinline Vector2& operator*=(float v)
    {
        x *= v; y *= v; return *this;
    }

    __forceinline Vector2& operator/=(float v)
    {
        x /= v; y /= v; return *this;
    }

    __forceinline Vector2 operator-(const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }

    __forceinline Vector2 operator+(const Vector2& v) const
    {
        return Vector2(x + v.x, y + v.y);
    }

    __forceinline Vector2& operator+=(const Vector2& v)
    {
        x += v.x; y += v.y; return *this;
    }

    __forceinline Vector2& operator-=(const Vector2& v) {
        x -= v.x; y -= v.y; return *this;
    }

    __forceinline Vector2 operator/(float v) const {
        return Vector2(x / v, y / v);
    }

    __forceinline Vector2 operator*(float v) const {
        return Vector2(x * v, y * v);
    }

    __forceinline Vector2 operator/(const Vector2& v) const {
        return Vector2(x / v.x, y / v.y);
    }

    __forceinline bool Zero() const {
        return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
    }


};

class Vector3
{
public:

    static float sqrtf(float number)
    {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long*)&y;
        i = 0x5f3759df - (i >> 1);
        y = *(float*)&i;
        y = y * (threehalfs - (x2 * y * y));
        y = y * (threehalfs - (x2 * y * y));

        return 1 / y;
    }

    Vector3()
    {
        x = y = z = 0.f;
    }

    Vector3(float fx, float fy, float fz)
    {
        x = fx;
        y = fy;
        z = fz;
    }

    float operator[](int i) const {
        return ((float*)this)[i];
    }

    float Length() const
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    float  x, y, z;

    Vector3 operator+(const Vector3& input) const
    {
        return Vector3{ x + input.x, y + input.y, z + input.z };
    }

    Vector3 operator-(const Vector3& input) const
    {
        return Vector3{ x - input.x, y - input.y, z - input.z };
    }

    Vector3 operator/(float input) const
    {
        return Vector3{ x / input, y / input, z / input };
    }

    Vector3 operator*(float input) const
    {
        return Vector3{ x * input, y * input, z * input };
    }

    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    Vector3& operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    Vector3& operator/=(float input)
    {
        x /= input;
        y /= input;
        z /= input;
        return *this;
    }

    Vector3& operator*=(float input)
    {
        x *= input;
        y *= input;
        z *= input;
        return *this;
    }

    Vector3 midPoint(Vector3 v2)
    {
        return Vector3((x + v2.x) / 2, (y + v2.y) / 2, (z + v2.z) / 2);
    }

    bool operator==(const Vector3& input) const
    {
        return x == input.x && y == input.y && z == input.z;
    }


    float distance(Vector3 vec)
    {
        return Vector3::sqrtf(pow(vec.x - x) + pow(vec.y - y) + pow(vec.z - z));
    }

    float clamp0to1(float value)
    {
        float result;
        if (value < 0)
        {
            result = 0;
        }
        else if (value > 1.f)
        {
            result = 1.f;
        }
        else
        {
            result = value;
        }
        return result;
    }

    float Lerp()
    {
        return x + (y - x) * clamp0to1(z);
    }

    float length_sqr() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    float length() const
    {
        return (float)sqrt(length_sqr());
    }

    float length_2d() const
    {
        return (float)sqrt((x * x) + (y * y));
    }

    float length() { return sqrtf((x * x) + (y * y) + (z * z)); }

    Vector3 normalized()
    {
        float len = length();
        return Vector3(x / len, y / len, z / len);
    }

    __forceinline bool Zero() const {
        return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f && z > -0.1f && z < 0.1f);
    }

    Vector3 cross(Vector3 rhs)
    {
        return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }

    float Dot(const Vector3& Vec2) {
        return x * Vec2[0] + y * Vec2[1] + z * Vec2[2];
    }

    float dot(Vector3 input) const
    {
        return (x * input.x) + (y * input.y) + (z * input.z);
    }


    bool empty() { return x == 0.000000 && y == 0.000000 && z == 0.000000; }

    bool is_valid() const
    {
        return !(x == 0.f && y == 0.f && z == 0.f) || (x == -1.f && y == -1.f && z == -1.f);
    }

};

class Vector4
{
public:
    Vector4()
    {
        x = y = z = w = 0.f;
    }

    Vector4(float fx, float fy, float fz, float fw)
    {
        x = fx;
        y = fy;
        z = fz;
        w = fw;
    }

    float x, y, z, w;

    bool empty() { return x == 0.000000 && y == 0.000000 && z == 0.000000 && w == 0.000000; }
};

Vector2 cos_tan_horizontal(float angle, float range, float x, float y, int length) {
    float our_angle = (angle + 45.f);

    float yaw = our_angle * (3.14159265358979323846 / 180.0);

    float view_cosinus = cos(yaw);
    float view_sinus = sin(yaw);

    float x2 = range * (-view_cosinus) + range * view_sinus;
    float y2 = range * (-view_cosinus) - range * view_sinus;

    int screen_x = x + static_cast<int>(x2 / range * length);
    int screen_y = y + static_cast<int>(y2 / range * length);

    return Vector2(screen_x, screen_y);
}


 
static bool Normalize(float& Yaw, float& Pitch) {
    if (Pitch < -89)
        return false;

    else if (Pitch > 89)
        return false;

    if (Yaw < -360)
        return false;

    else if (Yaw > 360)
        return false;

    return true;
}
static float to_degree(float radian)
{
    return radian * 180.f / 3.141592f;
}
const float Length(Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
static Vector3 CalcAngle(Vector3 LocalPos, Vector3 EnemyPos)
{
    Vector3 dir;

    dir.x = LocalPos.x - EnemyPos.x;
    dir.y = LocalPos.y - EnemyPos.y;
    dir.z = LocalPos.z - EnemyPos.z;

    float Pitch = to_degree(asin(dir.y / Length(dir)));
    float Yaw = to_degree(-atan2(dir.x, -dir.z));


    Vector3 Return;
    Return.x = Pitch;
    Return.y = Yaw;


    return Return;
}
const double ToRad(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
const double ToDeg(double radians) {
    return radians * (180.0 / 3.14);
}
const Vector4 ToQuat(Vector3 Euler) {
    float c1 = cos(ToRad(Euler.x) / 2);
    float s1 = sin(ToRad(Euler.x) / 2);
    float c2 = cos(ToRad(Euler.y) / 2);
    float s2 = sin(ToRad(Euler.y) / 2);
    float c3 = cos(ToRad(Euler.z) / 2);
    float s3 = sin(ToRad(Euler.z) / 2);
    float c1c2 = c1 * c2;
    float s1s2 = s1 * s2;
    float c1s2 = c1 * s2;
    float s1c2 = s1 * c2;
    Vector4 Quat = {
        s1c2 * c3 + c1s2 * s3,
        c1s2 * c3 - s1c2 * s3,
        c1c2 * s3 - s1s2 * c3,
        c1c2 * c3 + s1s2 * s3
    };
    return Quat;
}

struct Matrix4x4 {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        }; float m[4][4];
    };
}; float pViewMatrix[16];

std::string FilterDroppedItem(std::string input)
{
    size_t start = input.find(_memdup(skCrypt(" (")));
    size_t end = input.find(_memdup(skCrypt(")")));
    if (start != std::string::npos && end != std::string::npos)
        input.erase(start, end - start + 1);
    bool skip_item = false;
    std::string skip_keywords[] = { _memdup(skCrypt("torch")), _memdup(skCrypt("rock")), _memdup(skCrypt("asset")), _memdup(skCrypt("fire")), _memdup(skCrypt("dung")), _memdup(skCrypt("arrow")), _memdup(skCrypt("nail")), _memdup(skCrypt("movepoint")), _memdup(skCrypt("-")), _memdup(skCrypt("_")) };
    for (const std::string& keyword : skip_keywords) {
        if (input.find(keyword) != std::string::npos) {
            skip_item = true;
            break;
        }
    }
    if (skip_item)
        return (std::string)skCrypt("");

    return input;
}

float AimFovv(bool WORLD, Vector3 ScreenPos)
{
    if (WORLD) {
        Vector2 center(globals::ScreenMidSize.right, globals::ScreenMidSize.bottom);
        return center.distance({ ScreenPos.x, ScreenPos.y });
    }
    else {
        return 1000.f;
    }
}
