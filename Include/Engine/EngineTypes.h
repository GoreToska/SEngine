//
// Created by gorev on 13.08.2025.
//

#ifndef ENGINETYPES_H
#define ENGINETYPES_H
#include <DirectXCollision.h>
#include <SimpleMath.h>

typedef DirectX::SimpleMath::Vector2 Vector2D;
typedef DirectX::SimpleMath::Vector3 Vector3D;
typedef DirectX::SimpleMath::Vector4 Vector4D;
typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::SimpleMath::Quaternion Quaternion;
typedef DirectX::SimpleMath::Color Color;
inline constexpr float PI = 3.141593f;


#define ERROR_COLOR Color{255, 0, 144, 255}
#define TRANSPARENT_COLOR Color{0, 0, 0, 0}


struct Vertex
{
    Vector3D position;
    Vector3D normal;
    Vector3D tangent;
    Vector3D bitangent;
    Vector2D texCoord;
};

#endif //ENGINETYPES_H
