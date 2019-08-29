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
	load_resources();
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
void GraphicsManager::load_resources(){
	float* vertices_temp = NULL;
	unsigned int* indices_temp = NULL;
	std::size_t size_vertices_temp = 0;
	std::size_t size_indices_temp = 0;
	for (int i = 0; i < GRAPHICS_RESOURCE_SIZE; ++i){
		process_obj(resource_paths[i], &size_vertices_temp, &vertices_temp,
									&size_indices_temp, &indices_temp);
		resources_vertex_data[i] = new VertexData  (GRAPHICS_SHADER_COLOR_SOLID, size_vertices_temp, vertices_temp,
													size_indices_temp, indices_temp);
	}
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
		else if (header == "f"){ //can be a quad or a triangle (1/2/3 1/2/3 1/2/3 1/2/3 or 1/2/3 1/2/3 1/2/3)
			std::string fourth_indice = "";
			for (int i = 0; i < 4; ++i){
				fourth_indice = "";
				iss >> fourth_indice; //push 4 times
			}
			if (fourth_indice != ""){
				*size_indices += 6 * sizeof(unsigned int); //make 2 triangles from 1 quad so need 6 unsigned ints
			}
			else {
				*size_indices += 3 * sizeof(unsigned int);	//1 triangle so 3 unsigned ints
			}
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
		else if (header == "f"){
			std::string indice[4] = {""};
			iss >> indice[0] >> indice[1] >> indice[2] >> indice[3];
			std::istringstream iss_indice;
			std::string token;
			if (indice[3] != ""){ //if it's a quad
				for (int i = 0; i < 4; ++i){
					iss_indice.str(indice[i]);
					std::getline(iss_indice, token, '/'); //only take the first indice in a x/x/x formation
					std::stringstream token_to_int;
					token_to_int << token;
					unsigned int temp;
					token_to_int >> temp;
					--temp;
					switch(i){ // 0 1 2 0 2 3
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
					iss_indice.clear();
					iss_indice.str("");
				}
				indices_i += 6;
			}
			else {// if it's a triangle
				for (int i = 0; i < 3; ++i){
					iss_indice.str(indice[i]);
					std::getline(iss_indice, token, '/'); //only take the first indice in a x/x/x formation
					std::stringstream token_to_int;
					token_to_int << token;
					unsigned int temp;
					token_to_int >> temp;
					--temp;
					(*indices)[indices_i + i] = temp; 
				}
				indices_i += 3;
			}
		}
		iss.clear();
		iss.flush();
		iss.str("");
	}
	//delete &file;
}