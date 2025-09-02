//
// Created by gorev on 29.08.2025.
//

#ifndef PRIMITIVEOBJECTS_H
#define PRIMITIVEOBJECTS_H
#include <vector>

#include "Engine/EngineTypes.h"
#include "Render/ShaderManager.h"

namespace volume
{
    inline void GetBoxVertices(std::vector<Vector3D>& vector)
    {
        vector = {
            Vector3D(-0.5f, 0.5f, -0.5f),
            Vector3D(0.5f, 0.5f, -0.5f),
            Vector3D(-0.5f, -0.5f, -0.5f),
            Vector3D(0.5f, -0.5f, -0.5f),

            Vector3D(-0.5f, 0.5f, 0.5f),
            Vector3D(0.5f, 0.5f, 0.5f),
            Vector3D(-0.5f, -0.5f, 0.5f),
            Vector3D(0.5f, -0.5f, 0.5f)
        };
    }

    inline void GetBoxIndices(std::vector<DWORD>& vector)
    {
        vector = {
            0, 1, 2, 2, 1, 3,

            5, 4, 7, 7, 4, 6,

            4, 5, 0, 0, 5, 1,

            2, 3, 6, 6, 3, 7,

            4, 0, 6, 6, 0, 2,

            1, 5, 3, 3, 5, 7
        };
    }
}

#endif //PRIMITIVEOBJECTS_H
