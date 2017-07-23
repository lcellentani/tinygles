#pragma once

#include <limits>
#include <cmath>

namespace mathlib
{

class Angle {
public:
	enum AngleType {
		Degree,
		Radian
	};

	static constexpr const float DEG2RAD = 3.14159265f / 180.0f;
	static constexpr const float RAD2DEG = 180.0f / 3.14159265f;

public:
	Angle() : mDegree(0.0), mRadian(0.0) {}

	Angle(float value, AngleType type = Degree) {
		mDegree = type == Degree ? value : value * Angle::RAD2DEG;
		mRadian = type == Radian ? value : value * Angle::DEG2RAD;
	}

	float degrees() const {
		return mDegree;
	}

	float radians() const {
		return mRadian;
	}

	bool operator==(const Angle& other) const {
		float e = std::numeric_limits<float>::epsilon() * std::max(std::abs(mDegree), std::abs(other.mRadian));
		return (std::abs(mDegree - other.mDegree) <= e);
	}

	bool operator!=(const Angle& other) const {
		return !(*this == other);
	}

	bool operator<(const Angle& other) const {
		return mDegree < other.mDegree;
	}

	bool operator>(const Angle& other) const {
		return mDegree > other.mDegree;
	}

	const Angle operator-() const {
		return Angle(-mDegree, Angle::Degree);
	}

	const Angle operator+(const Angle& other) const {
		return Angle(mDegree + other.mDegree, Angle::Degree);
	}

	const Angle operator-(const Angle& other) const {
		return Angle(mDegree - other.mDegree, Angle::Degree);
	}

	static const Angle zero() {
		return Angle(0.0f, Angle::Degree);
	}

private:
	float mDegree;
	float mRadian;
};

} // namespace mathlibs