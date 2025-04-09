#include "camathspch.h"
#include "Mathematics/Color.h"

namespace Catalyst
{
	Color::Color()
		: r{ 1.f }, g{ 1.f }, b{ 1.f }, a{ 1.f }
	{
	}

	Color::Color(const float _r, const float _g, const float _b, const float _a)
		: r{ _r }, g{ _g }, b{ _b }, a{ _a }
	{
	}

	Color::operator vec4() const
	{
		return { r, g, b, a };
	}

	Color::operator glm::vec<3, float>() const
	{
		return { r, g, b };
	}

	Color& Color::operator*(const float _multi)
	{
		r *= _multi;
		g *= _multi;
		b *= _multi;
		a *= _multi;

		return *this;
	}

	Color& Color::operator*(const Color& _other)
	{
		r *= _other.r;
		g *= _other.g;
		b *= _other.b;
		a *= _other.a;

		return *this;
	}

	bool Color::operator==(Color& _other) const
	{
		vec4 thisCol = *this;
		vec4 otherCol = _other;

		return all(equal(glm::abs(otherCol), glm::abs(thisCol)));
	}

	bool Color::operator!=(Color& _other) const
	{
		return !(*this == _other);
	}
}