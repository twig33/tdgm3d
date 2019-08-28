#include <graphics.h>
#include <rendergroup.h>
#include <fstream>
#include <sstream>
#include <iostream>

GraphicsManager* Graphics;

GraphicsManager::GraphicsManager(){
	//glfw init
	glfwInit();
	//window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(640, 480, "tdgm3d", nullptr,
	nullptr);
	glfwMakeContextCurrent(window);
	//viewport
	glViewport(0, 0, 640, 480);
	//glew init 
	glewExperimental = GL_TRUE; //ya uzhe забылdlya chevo eto
	glewInit();
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	//create shaders
	proj = glm::perspective(glm::radians(90.0f), (float)640/(float)480, 0.1f, 100.0f);
	//camera = glm::mat4(1.0f);
	//set screen clear color
	glClearColor(0.9, 0.9, 1, 1);
	Shaders = new ShaderManager();
	RenderGroups = new RenderGroupManager();
	RenderGroups->set_projection_mat_pointer(&proj);
	const glm::mat4& temp = camera.get_transform_mat();
	RenderGroups->set_view_mat_pointer(&temp);
	//resources init
	float* vertices_temp = NULL;
	unsigned int* indices_temp = NULL;
	std::size_t size_vertices_temp = 0;
	std::size_t size_indices_temp = 0;
	process_obj("sphere.obj", &size_vertices_temp, &vertices_temp,
								&size_indices_temp, &indices_temp);
	resources_vertex_data[GRAPHICS_RESOURCE_SPHERE] = new VertexData  (GRAPHICS_SHADER_COLOR_SOLID, size_vertices_temp, vertices_temp,
																	 								size_indices_temp, indices_temp);
	process_obj("cube.obj", &size_vertices_temp, &vertices_temp,
								&size_indices_temp, &indices_temp);
	resources_vertex_data[GRAPHICS_RESOURCE_CUBE] = new VertexData  (GRAPHICS_SHADER_COLOR_SOLID, size_vertices_temp, vertices_temp,
																	 								size_indices_temp, indices_temp);
	//finish();
}

void GraphicsManager::finish(){
	this->quit = 1;
	glfwTerminate();	
}
void GraphicsManager::push(RenderObject* object){
	RenderGroups->push(object);	
}
void GraphicsManager::remove(RenderObject* object){
	RenderGroups->remove(object, false);	
}
void GraphicsManager::update(){
	if (glfwWindowShouldClose(window)){
		this->finish();
		return;	
	}
	RenderGroups->draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
}
VertexData* GraphicsManager::get_vertex_data(unsigned int id){
	if (id < 0 || id > GRAPHICS_RESOURCE_SIZE){
		std::cout << "Invalid resource id\n";	
		return NULL;
	}
	return resources_vertex_data[id];
}
void GraphicsManager::process_obj(const char* path,
								  std::size_t* size_vertices, float** vertices,
								  std::size_t* size_indices, unsigned int** indices)
{
	*size_vertices = 0;
	*size_indices = 0;
	std::fstream file(path);
	std::string line;
	while(std::getline(file, line)){ //calculate sizes
		std::istringstream iss(line);
		std::string header;
		iss >> header;
		if (header == "v"){
			*size_vertices += 3 * sizeof(float);
		}
		else if (header == "f"){
			*size_indices += 6 * sizeof(unsigned int);
		}
	}
	line = "";
	*vertices = (float*)calloc(*size_vertices, sizeof(float) * (*size_vertices));
	*indices = (unsigned int*)calloc(*size_indices, sizeof(unsigned int) * (*size_indices));
	std::cout << "Loading " << *size_vertices / 3 * sizeof(float) << " vertices and " << *size_indices / 6 * sizeof(unsigned int) << " faces from " << path << "\n";
	unsigned int vertices_i = 0, indices_i = 0;
	file.close();           
	file.clear();
	file.open(path);
	bool temp_latch = false;
	std::stringstream iss("");
	while(std::getline(file, line)){ //populate
		iss.str(line);
		std::string header;
		iss >> header;
		if (header == "v"){
			iss >> (*vertices)[vertices_i] >> (*vertices)[vertices_i + 1] >> (*vertices)[vertices_i + 2];
			std::cout << (*vertices)[vertices_i] << " " << (*vertices)[vertices_i + 1] << " " << (*vertices)[vertices_i + 2] << "\n";
			vertices_i += 3;
		}
		else if (header == "f"){ // 0 1 2 0 2 3
			std::string index;
			std::istringstream iss_index;
			std::string token;
			for (int i = 0; i < 4; ++i){
				iss >> index;
				iss_index.str(index);
				std::getline(iss_index, token, '/'); //only take the first indice in a x/x/x formation
				std::stringstream token_to_int;
				token_to_int << token;
				unsigned int temp;
				token_to_int >> temp;
				--temp;
				//std::cout << temp << "\n";
				switch(i){
					case 0:
						(*indices)[indices_i] = temp;
						(*indices)[indices_i + 3] = temp;
						break;
					case 1:
						(*indices)[indices_i + 1] = temp;
						break;
					case 2:
						(*indices)[indices_i + 2] = temp;
						(*indices)[indices_i + 4] = temp;
						break;
					case 3:
						(*indices)[indices_i + 5] = temp;
						break;
				}
				token_to_int.clear();
				token_to_int.flush();
				token_to_int.str("");
				iss_index.clear();
				iss_index.str("");
				index = "";
			}
			std::cout << (*indices)[indices_i] << " ";
			std::cout << (*indices)[indices_i+1] << " ";
			std::cout << (*indices)[indices_i+2] << " \n";
			std::cout << (*indices)[indices_i+3] << " ";
			std::cout << (*indices)[indices_i+4] << " ";
			std::cout << (*indices)[indices_i+5] << " \n";
			indices_i += 6;
		}
		iss.clear();
		iss.flush();
		iss.str("");
	}
	//delete &file;
}