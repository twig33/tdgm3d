#ifndef COLLISION2D_H
#define COLLISION2D_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderobject.h>

namespace Collision2D {
	struct AABB {
		glm::vec2 min;
		glm::vec2 max;
	};
	struct Circle {
		const glm::vec2 center;
		const float r;
	};
	class Collider2DCircle{
		public:
			Circle circle;
			Transform* parent_transform;
	};
	bool test_AABB_AABB(AABB& one, AABB& two);
	Circle circle_from_AABB(AABB& in);
	Circle transform_circle(Transform* transform, Circle& circle);
	bool test_circle_circle(Circle& one, Circle& two);
	bool test_c2dc_c2dc(Collider2DCircle& one, Collider2DCircle& two);
};
#endif