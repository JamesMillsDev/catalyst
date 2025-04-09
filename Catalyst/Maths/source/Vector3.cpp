#include "camathspch.h"
#include "Vector3.h"

#include "CatalystMath.h"

namespace Catalyst
{
    // Standard Vector3s
    Vector3 Vector3::zero = { 0.f, 0.f, 0.f };
    Vector3 Vector3::one = { 1.f, 1.f, 1.f };
    Vector3 Vector3::half = { .5f, .5f, .5f };
    Vector3 Vector3::up = { 0.f, 1.f, 0.f };
    Vector3 Vector3::down = { 0.f, -1.f, 0.f };
    Vector3 Vector3::left = { -1.f, 0.f, 0.f };
    Vector3 Vector3::right = { 1.f, 0.f, 0.f };
    Vector3 Vector3::forward = { 0.f, 0.f, 1.f };
    Vector3 Vector3::back = { 0.f, 0.f, -1.f };

    Vector3::Vector3()
        : x{ 0.f }, y{ 0.f }, z{ 0.f }
    {
        
    }

    Vector3::Vector3(const float _scalar)
        : x{ _scalar }, y{ _scalar }, z{ _scalar }
    {
    }

    Vector3::Vector3(const float _x, const float _y, const float _z)
        :x{ _x }, y{ _y }, z{ _z }
    {
    }

    Vector3::Vector3(const initializer_list<float> _values)
    {
        for (size_t i = 0; i < VEC_3_SIZE; ++i)
	        data[i] = *(_values.begin() + i);
    }

    Vector3::~Vector3() = default;

    Vector3::Vector3(const Vector3& _other)
        :x{ _other.x }, y{ _other.y }, z{ _other.z }
    {
    }

    Vector3::Vector3(Vector3&& _other) noexcept
        :x{ _other.x }, y{ _other.y }, z{ _other.z }
    {
        _other.x = 0.f;
        _other.y = 0.f;
        _other.z = 0.f;
    }

    Vector3 Vector3::Normalised(const Vector3& _vec)
    {
        const float mag = _vec.Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            return { 0.f, 0.f, 0.f };
        }
        else
        {
            return { _vec.x / mag, _vec.y / mag , _vec.z / mag };
        }
    }

    float Vector3::Distance(const Vector3& _a, const Vector3& _b)
    {
        return (_a - _b).Magnitude();
    }

    float Vector3::Dot(const Vector3& _lhs, const Vector3& _rhs)
    {
        return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z;
    }

    Vector3 Vector3::Cross(const Vector3& _lhs, const Vector3& _rhs)
    {
        return
    	{
    		_lhs.y * _rhs.z - _lhs.z * _rhs.y,
    		_lhs.z * _rhs.x - _lhs.x * _rhs.z,
    		_lhs.x * _rhs.y - _lhs.y * _rhs.x
    	};
    }

    Vector3 Vector3::Lerp(const Vector3& _a, const Vector3& _b, float _t)
    {
        _t = CatalystMath::Clamp01(_t);

        return _a * (1 - _t) + _b * _t;
    }

    float Vector3::Dot(const Vector3& _rhs) const
    {
        return x * _rhs.x + y * _rhs.y + z * _rhs.z;
    }

    Vector3 Vector3::Cross(const Vector3& _rhs) const
    {
        return
    	{
    		y * _rhs.z - z * _rhs.y,
    		z * _rhs.x - x * _rhs.z,
    		x * _rhs.y - y * _rhs.x
    	};
    }

    float Vector3::Magnitude() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    float Vector3::MagnitudeSqr() const
    {
        return x * x + y * y + z * z;
    }

    void Vector3::Normalise()
    {
        const float mag = Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            x = 0.f;
            y = 0.f;
            z = 0.f;
        }
        else
        {
            x = x / mag;
            y = y / mag;
            z = z / mag;
        }
    }

    Vector3 Vector3::Normalised() const
    {
        const float mag = Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            return { 0.f, 0.f, 0.f };
        }
        else
        {
            return { x / mag, y / mag , z / mag };
        }
    }

    string Vector3::ToString() const
    {
        return string("(") + std::to_string(x) + string(", ") + std::to_string(y) + string(", ") + std::to_string(z) + string(")");
    }

    bool Vector3::operator==(const Vector3& _rhs) const
    {
        return CatalystMath::Compare(x, _rhs.x) &&
            CatalystMath::Compare(y, _rhs.y) && 
            CatalystMath::Compare(z, _rhs.z);
    }

    bool Vector3::operator!=(const Vector3& _rhs) const
    {
        return !(*this == _rhs);
    }

    Vector3 Vector3::operator-(const Vector3& _rhs) const
    {
        return { x - _rhs.x, y - _rhs.y, z - _rhs.z };
    }

    Vector3& Vector3::operator-=(const Vector3& _rhs)
    {
        x -= _rhs.x;
        y -= _rhs.y;
        z -= _rhs.z;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& _rhs) const
    {
        return { x + _rhs.x, y + _rhs.y, z + _rhs.z };
    }

    Vector3& Vector3::operator+=(const Vector3& _rhs)
    {
        x += _rhs.x;
        y += _rhs.y;
        z += _rhs.z;
        return *this;
    }

    Vector3 Vector3::operator*(const Vector3& _rhs) const
    {
        return { x * _rhs.x, y * _rhs.y , z * _rhs.z };
    }

    Vector3& Vector3::operator*=(const Vector3& _rhs)
    {
        x *= _rhs.x;
        y *= _rhs.y;
        z *= _rhs.z;
        return *this;
    }

    Vector3 Vector3::operator*(const float _rhs) const
    {
        return { x * _rhs, y * _rhs, z * _rhs };
    }

    Vector3& Vector3::operator*=(const float _rhs)
    {
        x *= _rhs;
        y *= _rhs;
        z *= _rhs;
        return *this;
    }

    float Vector3::operator[](const int _index) const
    {
        if (_index < 0 || _index >= VEC_3_SIZE)
            return INFINITY;

        return data[_index];
    }

    Vector3& Vector3::operator=(const Vector3& _rhs)
    {
        if (*this == _rhs)
            return *this;

        x = _rhs.x;
        y = _rhs.y;
        z = _rhs.z;

        return *this;
    }

    Vector3& Vector3::operator=(Vector3&& _rhs) noexcept
    {
        if (*this == _rhs)
            return *this;

        x = _rhs.x;
        y = _rhs.y;
        z = _rhs.z;

        _rhs.x = 0.f;
        _rhs.y = 0.f;
        _rhs.z = 0.f;

        return *this;
    }
}
