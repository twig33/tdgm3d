#include <transform.h>

glm::mat4 identity = glm::mat4(1.0f);

Transform::Transform(){
	transform = identity;
	position_mat = identity;
	rotation_mat = identity;
	scale_mat = identity;
}

glm::vec3 Transform::get_position(){
	return position;	
}
const float* Transform::get_transform_mat_value_ptr() const {
	return glm::value_ptr(transform);
}
const glm::mat4& Transform::get_transform_mat(){
	return transform;	
}
void Transform::set_position(const glm::vec3& pos){
	position_mat = glm::translate(identity, pos);
	position = pos;
	update_transform_matrix();
}
void Transform::translate(const glm::vec3& tr){
	position_mat = glm::translate(position_mat, tr);
	transform = glm::translate(transform, tr);
	position += tr;
	//update_transform_matrix(); can get away with not updating the transform matrix when translating 
}
void Transform::set_rotation(const glm::vec3& rot){
	rotation_mat = glm::rotate(identity, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	update_transform_matrix();
}
void Transform::rotate(const glm::vec3& rot){
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	update_transform_matrix();	
}
void Transform::set_scale(const float scalein){
	scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(scalein,scalein,scalein));
	scale = scalein;
	update_transform_matrix();
}
float Transform::get_scale(){
	return scale;	
}
void Transform::update_transform_matrix(){
	transform = position_mat * rotation_mat * scale_mat;
}