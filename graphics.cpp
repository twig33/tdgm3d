#include <graphics.h>
#include <shader.h>
#include <helper.h> //loadFile

GraphicsManager Graphics;

RenderObject* GraphicsManager::new_empty_object_push_back(){
	RenderObject* object = new RenderObject;
	if (this->tail == NULL){
		this->tail = object;	
	}else if (this->head == NULL){
		this->head = object;	
	}else{
		this->head->next = object;
		this->head = object;
	}
	return object;
}
RenderObject* GraphicsManager::new_triangles_object(int color_type, 
													std::size_t size_vertices, float vertices[],
					  								std::size_t size_indices, unsigned int indices[]){
	if (size_indices % 3){
		std::cout << "Warning: indices size not a multiply of 3\n";	
	}
	RenderObject* object = this->new_empty_object_push_back();
	object->vertices_count = size_indices;
	glGenBuffers(1, &(object->VBO));
	glGenBuffers(1, &(object->EBO));
	glGenVertexArrays(1, &(object->VAO));
	glBindVertexArray(object->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return object;
}
void GraphicsManager::init(){
	//glfw init
	glfwInit();
	//window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(640, 480, "pong", nullptr,
	nullptr);
	glfwMakeContextCurrent(window);
	//viewport
	glViewport(0, 0, 640, 480);
	//glew init 
	glewExperimental = GL_TRUE; //ya uzhe забылdlya chevo eto
	glewInit();
	//shaders
	unsigned int vertex_shader, fragment_shader;
	//load files
	std::string stringvs = loadFile("shader.vs");
	std::string stringfrag = loadFile("shader.frag");
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
   		 std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//link shaders to shader program
	this->shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}		
	//cleanup
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	//use
	glUseProgram(shader_program);
	glClearColor(0.9, 0.9, 1, 1);
}

void GraphicsManager::finish(){
	this->quit = 1;
	glfwTerminate();	
}

void GraphicsManager::update(){
	if (glfwWindowShouldClose(window)){
		this->finish();
		return;	
	}
	glClear(GL_COLOR_BUFFER_BIT);
	RenderObject* curr = this->tail;
	while(curr != NULL){
		glUseProgram(shader_program);
		glBindVertexArray(curr->VAO);
		glDrawElements(GL_TRIANGLES, curr->vertices_count, GL_UNSIGNED_INT, 0);
		curr = curr->next;
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}