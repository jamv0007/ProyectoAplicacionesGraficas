//
// Created by Jose Antonio on 24/12/21.
//

#ifndef PRACTICA10_PAG_CAMERA_H
#define PRACTICA10_PAG_CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

namespace PAG {

    enum class MOVEMENT_TYPE{
        NONE,
        PAN,
        TILT,
        DOLLY,
        CRANE,
        ORBIT,
        ZOOM
    };

    class Camera {
        private:
            MOVEMENT_TYPE activeMove = MOVEMENT_TYPE::NONE;
            //Posicion camara
            glm::vec3 position;
            glm::vec3 cameraTarget;
            glm::vec3 up;

            //Vectores normalizados
            glm::vec3 cameraDirection;
            glm::vec3 cameraRight;
            glm::vec3 cameraUp;

            //perspectiva
            float fovY = 60.0;
            float aspecto = 1024.0/820.0;
            float zNear = 1.0f;
            float zFar = 100.0f;

            //Matrices
            glm::mat4 view;
            glm::mat4 proyection;

            void calculateCameraVector();

        public:
            Camera(glm::vec3 position,glm::vec3 cameraTarget, glm::vec3 up);
            virtual ~Camera();
            Camera(const Camera& orig);
            void updateCamera();
            void applyPanRotation(float amount);
            void applyTiltRotation(float amount);
            void applyDollyCamera(float sideAmount,float advanceAmount);
            void applyCraneCamera(float amount);
            void applyOrbitRotation(float amountX,float amountY);
            void applyZoom(float amount);
            glm::mat4 getViewMatrix();
            glm::mat4 getProjectionMatrix();
            MOVEMENT_TYPE getActiveMove();
            void setActiveMove(MOVEMENT_TYPE mov);

    };
}

#endif //PRACTICA10_PAG_CAMERA_H
