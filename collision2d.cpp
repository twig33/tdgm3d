#include <collision2d.h>

namespace Collision2D {
	bool test_AABB_AABB(AABB& one, AABB& two){
		if (one.min.x > two.max.x || two.min.x > one.max.x) return 0;
		if (one.min.y > two.max.y || two.min.y > one.max.y) return 0;
		return 1;
	}
	Circle circle_from_AABB(AABB& in){
		glm::vec2 d = in.max - in.min;
		float distance = sqrt(glm::dot(d, d));
		Circle out = {
			glm::vec2 (	in.min.x + (in.max.x - in.min.x) / 2,   //center
						in.min.y + (in.max.y - in.min.y) / 2),  
			distance / 2   //radius
		};
		return out;
	}
	Circle transform_circle(Transform* transform, Circle& circle){
		const glm::mat4& mat = transform->get_transform_mat();
		float scale = transform->get_scale();
		glm::vec4 pos = glm::vec4(circle.center.x, circle.center.y, 0.0f, 0.0f); //for multiplying with mat4 to get absolute pos
		pos = mat * pos;
		return {glm::vec2(pos.x, pos.y),
				circle.r * scale};
	}
	bool test_circle_circle(Circle& one, Circle& two){
		glm::vec2 d = one.center - two.center;
		float distance_squared = glm::dot(d, d);
		float radius_sum = one.r + two.r;
		return distance_squared <= radius_sum * radius_sum;
	}
	bool test_c2dc_c2dc(Collider2DCircle& one, Collider2DCircle& two){ //need to transform before testing
		Circle one_t = Collision2D::transform_circle(one.parent_transform, one.circle);
		Circle two_t = Collision2D::transform_circle(two.parent_transform, two.circle);
		return Collision2D::test_circle_circle(one_t, two_t);
	}
};