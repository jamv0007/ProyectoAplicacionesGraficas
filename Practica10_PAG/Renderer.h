//
// Created by Jose Antonio on 23/12/21.
//

#ifndef PRACTICA10_PAG_RENDERER_H
#define PRACTICA10_PAG_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <iomanip>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Camera.h"
#include "Light.h"

namespace PAG {

    enum USE_SHADER{
        COMPLETE,
        SHADOW
    };

    class Renderer {
        private:
            //Colores RGB fondo
            double r;
            double g;
            double b;

            //Vectores de Shaders, Modelos,y luces
            std::vector<Shader*> shaders;
            std::vector<Model*> models;
            std::vector<Light*> lights;

            //Camara
            Camera* camara;

            //FBO para los mapas de sombras
            GLuint shadowFbo[2];

            //Instancia para el patrón singleton
            static Renderer* instance;
            Renderer();

            //Metodos privados
            void drawModel(int inxModel);
            void applyConfiguration();
            void subImplementation(GLuint* subroutines, int tamS, std::string* sName, std::string* iName, GLuint sp, GLenum shader);
            void callSubroutines(GLuint sp, int model, int light);
            void sendLightUniforms(GLuint sp, int light);
            void sendModelUniforms(GLuint sp, int model);
            void shadowMap();
            void shadowRender(int width,int height);

        public:
            static Renderer* getInstance();
            virtual ~Renderer();
            void refresh_Window(int width,int height);
            void size_Window(int width, int height);
            void colorApply();
            void loadParameterGL();
            void addModel(MODEL_TYPE model);
            void removeModel(MODEL_TYPE model);
            void cameraMovement(float amount, bool horizontal);
            void setCameraMovement(MOVEMENT_TYPE movement);
            void wireMode();
            void textureMode();
            void normalMappingMode();
            void setR(double value);
            void setG(double value);
            void setB(double value);
            double getR();
            double getG();
            double getB();
    };
}


#endif //PRACTICA10_PAG_RENDERER_H
