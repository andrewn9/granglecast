/*
 * rendering.cpp
 */

#include "rendering.h"
#include "../../game.h"
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace world;

// In your RenderingSystem class (declaration):
unsigned int shader_program;

// In your Initialize() function:
void RenderingSystem::Initialize()
{
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
        }
    )";

    unsigned int vertexShader, fragmentShader;

    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        SDL_Log("Vertex shader compilation failed: %s", infoLog);
        return;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        SDL_Log("Fragment shader compilation failed: %s", infoLog);
        return;
    }

    // Link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        SDL_Log("Shader program linking failed: %s", infoLog);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void RenderingSystem::Update() {
    glClearColor(0.271f, 0.322f, 0.424f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Entity entity : entity_manager->GetEntitiesWithComponent<Sprite>()) {
        Draw(entity);
    }

    SDL_GL_SwapWindow(game::window);
}

void RenderingSystem::Draw(Entity entity) {
    Transform2D* transform = entity_manager->GetComponent<Transform2D>(entity);
    Transform2D* camera = entity_manager->GetComponent<Transform2D>(world::camera);

    if (transform->position.x + transform->size.x/2 <= camera->position.x - camera->size.x/2 ||
        transform->position.x - transform->size.x/2 >= camera->position.x + camera->size.x/2 ||
        transform->position.y + transform->size.y/2 <= camera->position.y - camera->size.y/2 ||
        transform->position.y - transform->size.y/2 >= camera->position.y + camera->size.y/2) {
        return;
    }

    Vector2 scale = Vector2{WINDOW_WIDTH/camera->size.x, WINDOW_HEIGHT/camera->size.y};
    Vector2 position = scale * (transform->position - Vector2{round(camera->position.x),round(camera->position.y)}) - Vector2{-WINDOW_WIDTH/2,-WINDOW_HEIGHT/2};
    position.x = round(position.x);
    position.y = round(position.y);
    Vector2 size = transform->size * scale;

    int centerX = position.x;
    int centerY = position.y;

    float vertices[] = {
        centerX - size.x/2, centerY - size.y/2, 0.0f,
        centerX + size.x/2, centerY - size.y/2, 0.0f,
        centerX + size.x/2, centerY + size.y/2, 0.0f,
        centerX - size.x/2, centerY + size.y/2, 0.0f
    };
    
    for (int i = 0; i < 12; i += 3) {
        vertices[i] = (vertices[i] - (WINDOW_WIDTH / 2.0f)) / (WINDOW_WIDTH / 2.0f);
        vertices[i + 1] = (vertices[i + 1] - (WINDOW_HEIGHT / 2.0f)) / (WINDOW_HEIGHT / 2.0f);
    }


    SDL_Log("Entity: %d", entity);
    SDL_Log("Vertex 1: (%.2f, %.2f, %.2f)", vertices[0], vertices[1], vertices[2]);
    SDL_Log("Vertex 2: (%.2f, %.2f, %.2f)", vertices[3], vertices[4], vertices[5]);
    SDL_Log("Vertex 3: (%.2f, %.2f, %.2f)", vertices[6], vertices[7], vertices[8]);
    SDL_Log("Vertex 4: (%.2f, %.2f, %.2f)\n", vertices[9], vertices[10], vertices[11]);

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Create VAO, VBO, and EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Use shader program and draw rectangle
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


