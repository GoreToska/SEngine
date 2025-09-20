//
// Created by gorev on 17.09.2025.
//

#include "GameObjects/DebugLine.h"


DebugLine::DebugLine(const Vector3D position, const Vector3D direction,
                     const Color color, const float length) : Super(),
                                                                 position(position),
                                                                 direction(direction),
                                                                 color(color),
                                                                 length(length)
{
    std::vector<Vertex> vertices;
    Vertex vertex;
    vertex.position = position;
    vertices.emplace_back(vertex);
    vertex.position = position + direction * length;
    vertices.emplace_back(vertex);
    mesh = AddComponent<MeshRender>(transform, vertices, color,
                                    "Debug_Lines_Deferred_VS",
                                    "Debug_Lines_Deferred_PS",
                                    "", D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
}
