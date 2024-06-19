#include "camathspch.h"
#include "Matrix3.h"

#include "Quaternion.h"

namespace Catalyst
{
    Matrix3::Matrix3()
    {
        
    }

    Matrix3::Matrix3(const Quaternion& _quat)
    {
        const float sqrQx = _quat.x * _quat.x;
        const float sqrQy = _quat.y * _quat.y;
        const float sqrQz = _quat.z * _quat.z;

        xAxis =
        {
            1.f - 2.f * sqrQy - 2.f * sqrQz,
            2.f * _quat.x * _quat.y + 2.f * _quat.z * _quat.w,
            2.f * _quat.x * _quat.z - 2.f * _quat.y * _quat.w
        };

        yAxis =
        {
            2.f * _quat.x * _quat.y - 2.f * _quat.z * _quat.w,
            1.f - 2.f * sqrQx - 2.f * sqrQz,
            2.f * _quat.y * _quat.z + 2.f * _quat.x * _quat.w
        };

        zAxis =
        {
            2.f * _quat.x * _quat.z + 2.f * _quat.y * _quat.w,
            2.f * _quat.y * _quat.z - 2.f * _quat.x * _quat.w,
            1.f - 2.f * sqrQx - 2.f * sqrQy
        };
    }

    Matrix3::~Matrix3()
    {
    }

    Matrix3::Matrix3(const Matrix3& _other)
    {
    }

    Matrix3& Matrix3::operator=(const Matrix3& _rhs)
    {
        return *this;
    }

    Vector3& Matrix3::operator[](const int _index)
    {
	    switch (_index)
	    {
	    case 0:
            return xAxis;
	    case 1:
            return yAxis;
	    case 2:
            return zAxis;
	    default:
            return Vector3::zero;

	    }
    }
}
