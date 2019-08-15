#include <shader.h>
#include <GL/glew.h>
#include <helper.h> //loadFile
#include <iostream>

glm::mat4 temp = glm::mat4(1.0f);
void Shader::init (const char* vertex_shader_path, const char* fragment_shader_path) {
	//shaders
	unsigned int vertex_shader, fragment_shader;
	//load files
	std::string stringvs = loadFile(vertex_shader_path);
	std::string stringfrag = loadFile(fragment_shader_path);
	const char* vertexShaderSource = stringvs.c_str();
	const char* fragmentShaderSource = stringfrag.c_str();
	//create
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	int  success;
	char infoLog[512];
	//compile
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv (vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success){
    	glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv (fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
   		 glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
   		 std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//link shaders to shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}		
	//set uniform locations
	load_common_uniforms();
	load_specific_uniforms();
	//cleanup
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use(){
	glUseProgram(this->shader_program);	
}
void Shader::set_mat4(int location, const float* in){
	glUniformMatrix4fv(location, 1, GL_FALSE, in);
	/*std::cout << "Setting mat4 \n";
	for (int i = 0; i < 16; ++i){
		std::cout << in[i] << " ";
		if ((i+1)%4 == 0){std::cout << '\n';}
	}std::cout << " at location " << location << "\n";*/
}
void Shader::set_transform(const float* transform){
	set_mat4(transform_uniform_location, transform);	
}
void Shader::set_projection(const float* projection){
	set_mat4(projection_uniform_location, projection);	
}
void Shader::set_view(const float* view){
	set_mat4(view_uniform_location, view);	
}
void Shader::load_common_uniforms(){
	transform_uniform_location = glGetUniformLocation(shader_program, transform_uniform_name);
	projection_uniform_location = glGetUniformLocation(shader_program, projection_uniform_name);
	view_uniform_location = glGetUniformLocation(shader_program, view_uniform_name);
}
void Shader::load_specific_uniforms(){
	std::cout << "Shader class does not have specific uniforms";	
}
void ShaderColorSolid::load_specific_uniforms(){
	color_uniform_location = glGetUniformLocation(shader_program, color_uniform_name); 	
	std::cout << "\n" << "color solid shader specific uniforms loaded\n";
}
void ShaderColorSolid::set_color(float r, float g, float b, float a){
	glUniform4f(color_uniform_location, r, g, b, a);	
}
void ShaderColorSolid::bind_attributes(){
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
void ShaderColorVertex::bind_attributes(){
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}