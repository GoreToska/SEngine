//
// Created by gorev on 17.09.2025.
//

#ifndef DEBUGLINE_H
#define DEBUGLINE_H
#include "GameObject.h"
#include "Component/MeshRender.h"


class DebugLine : public GameObject
{
    typedef GameObject Super;

public:
    DebugLine(const Vector3D position, const Vector3D direction, const Color color, const float length = 0.2f);

private:
    std::shared_ptr<MeshRender> mesh;

    Vector3D position;
    Vector3D direction;
    Color color;
    float length;
};


#endif //DEBUGLINE_H
