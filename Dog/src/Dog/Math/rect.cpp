#include <PCH/dogpch.h>
#include "rect.h"

namespace Dog {



	inline Rect::Rect(float l, float t, float w, float h)
		: left(l), right(l + w), top(t), bottom(t + h), width(w), height(h) {}

	inline bool Rect::contains(const glm::vec2& p) const
	{
		return p.x >= left && p.x <= right && p.y >= top && p.y <= bottom;
	}

	inline bool Rect::contains(const Rect& r) const
	{
		return r.left >= left && r.right <= right && r.top >= top && r.bottom <= bottom;
	}

	inline bool Rect::overlaps(const Rect& r) const
	{
		return left < r.right && right > r.left && top < r.bottom && bottom > r.top;
	}

	inline bool Rect::bordersOverlap(const Rect& r) const
	{
		return left <= r.right && right >= r.left && top <= r.bottom && bottom >= r.top;
	}

	inline void Rect::SetScale(const glm::vec2& scale) {
		width = scale.x;
		height = scale.y;
		right = left + width;
		bottom = top + height;
	}

	inline void Rect::UpdateBoundsFromTopLeft() {
		right = left + width;
		bottom = top + height;
	}

	inline void Rect::PrintBounds() const {
		printf("Left: %f, Top: %f, Right: %f, Bottom: %f\n", left, top, right, bottom);
	}

} // namespace Dog
