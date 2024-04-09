#pragma once

namespace Dog {

	struct Rect
	{
		float left, right, top, bottom;
		float width, height;

		Rect() = default;
		Rect(float l, float t, float w, float h);

		bool contains(const glm::vec2& p) const;
		bool contains(const Rect& r) const;

		bool overlaps(const Rect& r) const;
		bool bordersOverlap(const Rect& r) const;

		void SetScale(const glm::vec2& scale);

		void UpdateBoundsFromTopLeft();

		void PrintBounds() const;
	};

}