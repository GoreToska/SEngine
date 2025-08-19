//
// Created by gorev on 18.08.2025.
//

#ifndef SMATH_H
#define SMATH_H
#include <DirectXMath.h>

namespace smath
{
    inline float ToRadians(const float degrees)
    {
        return degrees * (DirectX::XM_PI / 180.0f);
    }

    struct WeakPtrComparator
    {
        bool operator()(const std::weak_ptr<CameraComponent>& lhs, const std::weak_ptr<CameraComponent>& rhs) const
        {
            return lhs.lock() < rhs.lock();
        }
    };
}

#endif //SMATH_H
