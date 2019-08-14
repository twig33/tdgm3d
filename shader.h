#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
class Shader {
	public:
		void use();//
		void set_transform(const float* transform);
		void set_projection(const float* projection);
		void set_view(const float* view);
	protected:
		int shader_program;
		void init(const char* vertex_shader_path, const char* fragment_shader_path);
		const char* transform_uniform_name = "transform";
		const char* view_uniform_name = "view";
		const char* projection_uniform_name = "proj";
		int transform_uniform_location;
		int view_uniform_location;
		int projection_uniform_location;
		static void set_mat4(int location, const float* in);
		void load_common_uniforms();
		virtual void load_specific_uniforms();
};
class ShaderColorSolid : public Shader {
	public:
		ShaderColorSolid()
		{init("shader_color_solid.vs", "shader_color_solid.frag");};
		void set_color(float r, float g, float b, float a = 1.0);
	protected:
		const char* color_uniform_name = "ourColor";
		int color_uniform_location;
		void load_specific_uniforms();
};
class ShaderColorVertex : public Shader {
	public:
		ShaderColorVertex()
		{init("shader_color_vertex.vs", "shader_color_vertex.frag");};
	protected:
		void load_specific_uniforms(){
			std::cout << "\n" << "vertexcolorhsader specific uniforms loaded\n";
		}
};
#endif