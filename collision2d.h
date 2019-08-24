#ifndef COLLISION2D_H
#define COLLISION2D_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Collision2D {
	struct AABB {
		glm::vec2 min;
		glm::vec2 max;
	};
	bool test_AABB_AABB(AABB& one, AABB& two){
		if (one.min.x > two.max.x || two.min.x > one.max.x) return 0;
		if (one.min.y > two.max.y || two.min.y > one.max.y) return 0;
		return 1;
	}
};
#endif