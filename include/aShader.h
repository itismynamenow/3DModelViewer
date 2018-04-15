#ifndef ASHADER_H
#define ASHADER_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#define GLEW_STATIC
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class aShader
{
    public:
        aShader();
        virtual ~aShader();

        unordered_map <string, GLint> programs;
        unordered_map <string, GLint> uniformMap;
        GLuint currentProgram;

        void addProgram(string vs,string fs, string name);
        GLuint getProgram(string name);
        void useProgram(string name);

        const string loadShader(string filename);

        void setUniform(string program, string uniform, int value);
        void setUniform(string program, string uniform, float value);
        void setUniform(string program, string uniform, glm::vec3 value);
        void setUniform(string program, string uniform, glm::mat4x4 value);
        void setUniformMap();

    protected:

    private:
};

#endif // ASHADER_H
