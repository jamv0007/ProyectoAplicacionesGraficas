//
// Created by Jose Antonio on 25/12/21.
//

#ifndef PRACTICA10_PAG_LIGHT_H
#define PRACTICA10_PAG_LIGHT_H

#include <glm/glm.hpp>
#include "Camera.h"

namespace PAG {

    enum class LIGHT_TYPE{
        AMBIENT,
        POINT,
        DIRECTIONAL,
        SPOTLIGHT
    };

    class Light {
        private:
            LIGHT_TYPE light = LIGHT_TYPE::AMBIENT;//Tipo de la luz

            //Intensidades de la luz
            glm::vec3 Ia = glm::vec3(0.5,0.5,0.5);
            glm::vec3 Id = glm::vec3(1.0,1.0,1.0);
            glm::vec3 Is = glm::vec3(0.5,0.5,0.5);

            //Posición y  dirección de la luz
            glm::vec3 position = glm::vec3(0.0,0.0,6.0);
            glm::vec3 direction = glm::vec3(0.0,-1.5,0.0);

            //Ángulo y exponente de suavización del angulo
            float gamma = 3.0;
            float softEdges = 256.0; //Exponente Bordes suaves cono luz

            //Matrices de visión y proyeccion para el mapa de sombras
            glm::mat4 pM = glm::mat4(1.0f);
            glm::mat4 vM = glm::mat4(1.0f);

            //Id del mapa de sombras
            GLuint shadowTexture = -1;

        public:

            Light();
            virtual ~Light();
            Light(const Light& orig);

            void setAmbientLight(glm::vec3 Ia);
            void setPointLight(glm::vec3 position, glm::vec3 Id, glm::vec3 Is);
            void setDirectionalLight(glm::vec3 direction,glm::vec3 Id, glm::vec3 Is );
            void setSpotLight(glm::vec3 position,glm::vec3 direction,glm::vec3 Id, glm::vec3 Is,float gamma,float softEdges);

            glm::vec3 getIa();
            glm::vec3 getId();
            glm::vec3 getIs();
            glm::vec3 getPosition();
            glm::vec3 getDirection();
            float getGamma();
            float getSoftEdges();
            LIGHT_TYPE getLight();

            glm::mat4 getPm();
            glm::mat4 getVm();
            GLuint getShadowTexture();

    };
}

#endif //PRACTICA10_PAG_LIGHT_H
