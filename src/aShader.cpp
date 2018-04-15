#include "aShader.h"

aShader::aShader()
{
    //ctor
}

aShader::~aShader()
{
    //dtor
}

void aShader::addProgram(string vs,string fs, string name)
{
    GLuint program;
    program =glCreateProgram();

//    cout<<"Is Program ="<<(bool)glIsProgram(program)<<endl;
    GLint status;
    const char *temp;
    std::string tmpString;

    //create vertex shader

    GLuint VertexShader=glCreateShader(GL_VERTEX_SHADER);
    tmpString=loadShader(vs+".vs");
    temp = tmpString.c_str();
    glShaderSource(VertexShader, 1, &temp, NULL);
    glCompileShader(VertexShader);

//    std::cout<<vs+".vs"<<std::endl;
//    std::cout<<temp<<std::endl;

    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &status);

    if (status!= GL_TRUE)
    {
        std::cout<<"Failed  to load vertex shader"<<std::endl;
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(VertexShader, 1024,&log_length, message);
        std::cout<<message<<std::endl;
    }

    glAttachShader(program, VertexShader);

    //create fragment shader

    GLuint FragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    tmpString=loadShader(fs+".fs");
    temp = tmpString.c_str();
    glShaderSource(FragmentShader, 1, &temp, NULL);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &status);

    if (status!= GL_TRUE)
    {
        std::cout<<"Failed  to load fragment shader"<<std::endl;
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(FragmentShader, 1024,&log_length, message);
        std::cout<<message<<std::endl;
    }

    glAttachShader(program, FragmentShader);

    //link program

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        std::cout<<"Failed to link program"<<std::endl;
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024,&log_length, message);
        std::cout<<message<<std::endl;
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status != GL_TRUE)
    {
        std::cout<<"Failed to validate program"<<std::endl;
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024,&log_length, message);
        std::cout<<message<<std::endl;
    }
    //add program to map

    programs[name]=program;

    glUseProgram(program);
//    cout<<"Program ="<<program<<endl;
//    cout<<"Is Program ="<<(bool)glIsProgram(program)<<endl;

}

const string aShader::loadShader(string filename)
{
    string line;
    string text;
    const char * filename_cstr = filename.c_str();
    ifstream myfile (filename_cstr);
    if(myfile.fail()){
        cout<<"Failed to load file named as "<<filename<<endl;
        return "";
    }
    else
    {
        while( getline(myfile, line) )
        {
            text+="\n"+line;
        }
    }


    return text;
}

void aShader::useProgram(string name)
{
    glUseProgram(programs[name]);
    currentProgram=programs[name];
    if(name=="basicProgram")
        setUniformMap();
}

GLuint aShader::getProgram(string name)
{
    return programs[name];
}

void aShader::setUniform(string program, string uniform, int value)
{
    const char *uniform_c_str = uniform.c_str();
    glUniform1i(glGetUniformLocation(programs[program], uniform_c_str),value );
}

void aShader::setUniform(string program, string uniform, float value)
{
    const char *uniform_c_str = uniform.c_str();
    glUniform1f(glGetUniformLocation(programs[program], uniform_c_str),value );
}

void aShader::setUniform(string program, string uniform, glm::vec3 value)
{
    const char *uniform_c_str = uniform.c_str();
    glUniform3f(glGetUniformLocation(programs[program], uniform_c_str),value.x,value.y,value.z );
}

void aShader::setUniform(string program, string uniform, glm::mat4x4 value)
{
    const char *uniform_c_str = uniform.c_str();
    glUniformMatrix4fv(glGetUniformLocation(programs[program], uniform_c_str),1, GL_TRUE,glm::value_ptr(value));
}

void aShader::setUniformMap()
{
    uniformMap["textureSampler"]=glGetUniformLocation(currentProgram, "textureSampler");
    uniformMap["transform"]=glGetUniformLocation(currentProgram, "transform");
    uniformMap["displayTexture"]=glGetUniformLocation(currentProgram, "displayTexture");
    uniformMap["playAnimation"]=glGetUniformLocation(currentProgram, "playAnimation");
    uniformMap["bones"]=glGetUniformLocation(currentProgram, "bones");
}
