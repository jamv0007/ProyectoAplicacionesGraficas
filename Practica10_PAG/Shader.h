//
// Created by Jose Antonio on 23/12/21.
//

#ifndef PRACTICA10_PAG_SHADER_H
#define PRACTICA10_PAG_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <iomanip>
#include <fstream>

namespace PAG {
    class Shader {
        private:
            //Identificadores de la geometria y los shaders
            GLuint idVS = 0; // Identificador del vertex shader
            GLuint idFS = 0; // Identificador del fragment shader
            GLuint idSP = 0; // Identificador del shader program

            std::string vertexShader = "";//Contenido del vertex shader
            std::string fragmentShader = "";//Contenido del fragment shader
            std::string vertexShaderFile;//fichero con el vertex shader
            std::string fragmentShaderFile;//fichero con el fragment shader

            void loadShaderProgram();
            void loadShaders();
            void shadersError();
            void compilationShaderError(GLuint idShader);

        public:
            Shader(std::string vsFile, std::string fsFile);
            virtual ~Shader();
            Shader(const Shader& orig);
            void activeShaderProgram();
            GLuint getIdSp();


    };
}


#endif //PRACTICA10_PAG_SHADER_H
