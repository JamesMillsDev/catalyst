#include "camathspch.h"
#include "Vector2.h"

#include "CatalystMath.h"

namespace Catalyst
{
    // Standard Vector2s
    Vector2 Vector2::zero = { 0.f, 0.f };
    Vector2 Vector2::one = { 1.f, 1.f };
    Vector2 Vector2::half = { .5f, .5f };
    Vector2 Vector2::up = { 0.f, 1.f };
    Vector2 Vector2::down = { 0.f, -1.f };
    Vector2 Vector2::left = { -1.f, 0.f };
    Vector2 Vector2::right = { 1.f, 0.f };

	Vector2 Vector2::Normalised(const Vector2& _vec)
	{
        const float mag = _vec.Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            return { 0.f, 0.f };
        }
        else
        {
            return { _vec.x / mag, _vec.y / mag };
        }
	}

	float Vector2::Distance(const Vector2& _a, const Vector2& _b)
	{
        return (_a - _b).Magnitude();
	}

	float Vector2::Dot(const Vector2& _lhs, const Vector2& _rhs)
	{
        return _lhs.x * _rhs.x + _lhs.y * _rhs.y;
	}

	Vector2 Vector2::Lerp(const Vector2& _a, const Vector2& _b, float _t)
	{
        _t = CatalystMath::Clamp01(_t);

        return _a * (1 - _t) + _b * _t;
	}

    Vector2::Vector2()
	    : x{ 0 }, y{ 0 }
    {
        
    }

    Vector2::Vector2(const float _scalar)
	    : x{ _scalar }, y{ _scalar }
    {
    }

    Vector2::Vector2(const float _x, const float _y)
        : x{ _x }, y{ _y }
    {
    }

    Vector2::Vector2(const initializer_list<float> _values)
    {
        for (size_t i = 0; i < VEC_2_SIZE; ++i)
	        data[i] = *(_values.begin() + i);
    }

    Vector2::~Vector2() = default;

    Vector2::Vector2(const Vector2& _other)
	    : x{ _other.x }, y{ _other.y }
    {
    }

    Vector2::Vector2(Vector2&& _other) noexcept
        :x{ _other.x }, y{ _other.y }
    {
        _other.x = 0.f;
        _other.y = 0.f;
    }

    float Vector2::Magnitude() const
    {
        return sqrtf(x * x + y * y);
    }

    float Vector2::MagnitudeSqr() const
    {
        return x * x + y * y;
    }

    void Vector2::Normalise()
    {
        const float mag = Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            x = 0.f;
            y = 0.f;
        }
        else
        {
            x = x / mag;
            y = y / mag;
        }
    }

    Vector2 Vector2::Normalised() const
    {
        const float mag = Magnitude();

        if (CatalystMath::Compare(mag, 0.f))
        {
            return { 0.f, 0.f };
        }
        else
        {
            return { x / mag, y / mag };
        }
    }

    string Vector2::ToString() const
    {
        return string("(") + std::to_string(x) + string(", ") + std::to_string(y) + string(")");
    }

    bool Vector2::operator==(const Vector2& _rhs) const
    {
        return CatalystMath::Compare(x, _rhs.x) && CatalystMath::Compare(y, _rhs.y);
    }

    bool Vector2::operator!=(const Vector2& _rhs) const
    {
        return !(*this == _rhs);
    }

    Vector2 Vector2::operator-(const Vector2& _rhs) const
    {
        return { x - _rhs.x, y - _rhs.y };
    }

    Vector2& Vector2::operator-=(const Vector2& _rhs)
    {
        x -= _rhs.x;
        y -= _rhs.y;
        return *this;
    }

    Vector2 Vector2::operator+(const Vector2& _rhs) const
    {
        return { x + _rhs.x, y + _rhs.y };
    }

    Vector2& Vector2::operator+=(const Vector2& _rhs)
    {
        x += _rhs.x;
        y += _rhs.y;
        return *this;
    }

    Vector2 Vector2::operator*(const Vector2& _rhs) const
    {
        return { x * _rhs.x, y * _rhs.y };
    }

    Vector2& Vector2::operator*=(const Vector2& _rhs)
    {
        x *= _rhs.x;
        y *= _rhs.y;
        return *this;
    }

    Vector2 Vector2::operator*(const float _rhs) const
    {
        return { x * _rhs, y * _rhs };
    }

    Vector2& Vector2::operator*=(const float _rhs)
    {
        x *= _rhs;
        y *= _rhs;
        return *this;
    }

    Vector2& Vector2::operator=(const Vector2& _other)
    {
        if (*this == _other)
            return *this;

        x = _other.x;
        y = _other.y;

        return *this;
    }

    Vector2& Vector2::operator=(Vector2&& _rhs) noexcept
    {
        if (*this == _rhs)
            return *this;

        x = _rhs.x;
        y = _rhs.y;

        _rhs.x = 0.f;
        _rhs.y = 0.f;

        return *this;
    }
}
