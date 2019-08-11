#include <shader.h>
#include <GL/glew.h>
#include <helper.h> //loadFile

unsigned int load_shaders (const char* vertex_shader_path, const char* fragment_shader_path) {
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
	if(!success){
    	glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);
	if(!success)
	{
   		 glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
   		 std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//link shaders to shader program
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}		
	//cleanup
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}