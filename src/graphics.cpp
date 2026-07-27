
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics.h"
#include "chunk.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

camera->GetProjection(width, height);
glViewport(0, 0, width, height);

}

bool MiddleMousePressed = false;
bool LeftMousePressed = false;
bool RPressed = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
		camera->mouse_direction = 0;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
		camera->mouse_direction = 1;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
		camera->mouse_direction = 2;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
		camera->mouse_direction = 3;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
		RPressed = true;
		camera->RemArrow(xpos, ypos);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        camera->mouse_type = 1;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        camera->mouse_type = 2;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        camera->mouse_type = 3;
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        camera->mouse_type = 4;
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        camera->mouse_type = 5;
    }
    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
    {
        camera->mouse_type = 0;
    }

    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
    	RPressed = false;
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	
	if(action == GLFW_PRESS)
	{
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
        	LeftMousePressed = true;
            camera->SetArrow(xpos, ypos);
		}

		if(button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			
		}

        if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
         	MiddleMousePressed = true;
            camera->mouse_position = {xpos, ypos};
            camera->old_position = camera->position;
		}
    }

	if(action == GLFW_RELEASE)
	{
        if (button == GLFW_MOUSE_BUTTON_LEFT)
 			LeftMousePressed = false;
        if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			MiddleMousePressed = false;
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (MiddleMousePressed)
    {
    	camera->UpdateWindowSize(window);
		camera->MoveCamera(xpos, ypos);
    }
    if(LeftMousePressed)
    {
    	camera->SetArrow(xpos, ypos);
    }
    if(RPressed)
    {
    	camera->RemArrow(xpos, ypos);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	
    if (yoffset > 0)
        camera->zoom-=0.1f;

    if (yoffset < 0)
        camera->zoom+=0.1f;

    camera->UpdateWindowSize(window);

}

ChunkMesh::ChunkMesh()
	: VAO(0), VBO(0), EBO(0)
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 256 * 4 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 256 * 6 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(offsetof(Point, u)));
	glEnableVertexAttribArray(1);

	int index = 0;

	for (int i = 0; i < 256; i++)
	{
	    int offset = i * 4;
	
	    indices[index++] = 0 + offset;
	    indices[index++] = 1 + offset;
	    indices[index++] = 3 + offset;
	
	    indices[index++] = 0 + offset;
	    indices[index++] = 2 + offset;
	    indices[index++] = 3 + offset;
	}

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,256 * 6 * sizeof(unsigned int),indices);
}

ChunkMesh::~ChunkMesh()
{
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);

    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
}

void ChunkMesh::ChangeState(Chunk* chunk, uint8_t position, uint8_t state)
{

	if(chunk->type[position] != 0)
	{
		float state_offset = (1 / 8.0f) * 4;
		if(state == 0 && vertices[position*4].v <= 0.5 && vertices[position*4+3].v <= 0.5)
		{
    		vertices[position * 4].v += state_offset;
    		vertices[position * 4 + 1].v += state_offset;
    		vertices[position * 4 + 2].v += state_offset;
    		vertices[position * 4 + 3].v += state_offset;
    	}
    	if(state >= 1 && vertices[position*4].v >= 0.5 && vertices[position*4+3].v >= 0.5)
		{
    		vertices[position * 4].v -= state_offset;
    		vertices[position * 4 + 1].v -= state_offset;
    		vertices[position * 4 + 2].v -= state_offset;
    		vertices[position * 4 + 3].v -= state_offset;
    	}
    }
}

void ChunkMesh::CalculateCellMesh(Chunk* chunk, uint8_t position, bool _flip)
{

	if(chunk->type[position] > 0)
	{
		int x = position % 16;
		int y = position / 16;
		uint8_t _direction = (chunk->direction[position/4] >> (3-position%4)*2)&3;
		int uvx = (chunk->type[position] -1) % 8;
		int uvy = (chunk->type[position] -1) / 8;
		float px = chunk->x * 16 + x;
   		float py = chunk->y * 16 - y;
		float u0 = uvx * (1 / 8.0f);
		float v0 = (1 - (1 / 8.0f) * uvy) - (1 / 8.0f);
		float u1 = (uvx * (1 / 8.0f)) + (1 / 8.0f);
		float v1 = 1 - (1 / 8.0f) * uvy;
		float U[4] = {u0,u1,u0,u1};
		float V[4] = {v1,v1,v0,v0};

		float state_offset = 0;
		if(((chunk->state[position/4] >> (3-position%4)*2)&3) != 0)
		state_offset = (1 / 8.0f) * 4;

		constexpr uint8_t uv[2][4][4] =
		{
			{
				{0,1,2,3},
				{2,0,3,1},
				{3,2,1,0},
				{1,3,0,2},
			},
			{
				{1,0,3,2},
				{3,1,2,0},
				{3,2,1,0},
				{1,3,0,2},
			}
		};

		vertices[position*4] =   Point(px,py + 1, 0, U[uv[_flip][_direction][0]], V[uv[_flip][_direction][0]] - state_offset);
    	vertices[position*4+1] = Point(px+1,py+1, 0, U[uv[_flip][_direction][1]], V[uv[_flip][_direction][1]] - state_offset);
    	vertices[position*4+2] = Point(px, py,    0, U[uv[_flip][_direction][2]], V[uv[_flip][_direction][2]] - state_offset);
    	vertices[position*4+3] = Point(px+1,py,   0, U[uv[_flip][_direction][3]], V[uv[_flip][_direction][3]] - state_offset);

	}
	else
	{
		vertices[position*4]= Point();
		vertices[position*4+1]= Point();
		vertices[position*4+2]= Point();
		vertices[position*4+3]= Point();
	}
	

	this->Update();
	
}
void ChunkMesh::Update()
{
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 256 * 4 * sizeof(Point), vertices);
	
	glBindVertexArray(0);
}
void ChunkMesh::Render(glm::mat4 ortho, GraphicsHandler& gh)
{
	glUseProgram(gh.shaderProgram);

	glUniform1i(glGetUniformLocation(gh.shaderProgram, "tex"), 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gh.texture);
	
	glUniformMatrix4fv(glGetUniformLocation(gh.shaderProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 256 * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "\033[31m" << "Shader compile error:\n" << "\033[0m" << infoLog << std::endl;
        }
        else
        {
        //	std::cout << "\033[0m" << "Shader compiled successfully " << "\033[32m" << "[OK]" << "\033[0m" << std::endl;
        }


    return shader;
}

GLuint CreateShaderProgram(const char* vshader, const char* fshader) 
{
    GLuint vertex = CompileShader(GL_VERTEX_SHADER, vshader);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fshader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);


    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}
GLuint CreateTexture(const char* filename)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    return texture;
}
