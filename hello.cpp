#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

GLuint shaderProgram;
GLuint vao;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

// Compile shader and check for errors
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        cout << "Shader compile error:\n" << log << endl;
    }
    return shader;
}

// Setup shaders and program
void initShaders() {
    GLuint vShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    // Setup camera matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::ortho(-5.0, 5.0, -5.0, 5.0, 5.0, 15.0);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glutWireTeapot(2.0);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glutSolidTeapot(1.0);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glutSolidTeapot(1.0);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // Ensure compatibility profile
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Shader-Based Teapot");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW Init Failed!" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    initShaders();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
