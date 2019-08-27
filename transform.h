#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
	public:
		Transform();
		void set_position(const glm::vec3& pos);
		glm::vec3 get_position ();
		const float* get_transform_mat_value_ptr() const;
		const glm::mat4& get_transform_mat();
		void translate(const glm::vec3& tr);
		void rotate(const glm::vec3& rot);
		void set_rotation(const glm::vec3& rot);
		float get_scale();
		void set_scale(const float scalein);
	private:
		float scale = 1.0f;
		glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
		glm::mat4 position_mat;
		glm::mat4 transform;
		glm::mat4 rotation_mat;
		glm::mat4 scale_mat;
		void update_transform_matrix();
};
#endif