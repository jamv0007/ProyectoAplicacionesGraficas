//
// Created by Jose Antonio on 24/12/21.
//

#include "Camera.h"
/***
 * @brief Constructor de camara
 * @param position Posicion de la camara
 * @param cameraTarget A donde mira la camara
 * @param up Vector hacia arriba
 */
PAG::Camera::Camera(glm::vec3 position, glm::vec3 cameraTarget, glm::vec3 up) {
    this->position = position;
    this->cameraTarget = cameraTarget;
    this->up = up;

    calculateCameraVector();
}

/***
 * @bief Destructor de camara
 */
PAG::Camera::~Camera() {

}

/***
 * @brief Constructor copia de camara
 * @param orig Objeto a copiar
 */
PAG::Camera::Camera(const Camera &orig) {
    this->activeMove = orig.activeMove;
    //Posicion camara
    this->position = orig.position;
    this->cameraTarget = orig.cameraTarget;
    this->up = orig.up;

    //Vectores normalizados
    this->cameraDirection = orig.cameraDirection;
    this->cameraRight = orig.cameraRight;
    this->cameraUp = orig.cameraUp;

    //perspectiva
    this->fovY = orig.fovY;
    this->aspecto = orig.aspecto;
    this->zNear = orig.zNear;
    this->zFar = orig.zFar;

    //Matrices
    this->view = orig.view;
    this->proyection = orig.proyection;
}

/***
 * @brief Función que permite calcular los vectores de la camara como direccion, el vector hacia la derecha y hacia arriba
 */
void PAG::Camera::calculateCameraVector() {
    this->cameraDirection = glm::normalize(position - cameraTarget);

    //Se calcula el vector derecho si la direccion y arriba son paralelos
    if(glm::all(glm::equal(glm::cross(glm::normalize(up),cameraDirection),glm::vec3(0,0,0)))){
        if(glm::all(glm::equal(cameraDirection,glm::vec3(0,-1,0)))){
            this->cameraRight = glm::normalize(glm::cross(glm::vec3(0,0,-1),cameraDirection));
        }else if(glm::all(glm::equal(cameraDirection,glm::vec3(0,1,0)))){
            this->cameraRight = glm::normalize(glm::cross(glm::vec3(0,0,1),cameraDirection));
        }
    }else{
        this->cameraRight = glm::normalize(glm::cross(up,cameraDirection));
    }


    this->cameraUp = glm::normalize(glm::cross(cameraDirection,cameraRight));

    updateCamera();
}

/***
 * @brief Funcion que actualiza la camara perpectiva
 */
void PAG::Camera::updateCamera() {

    this->view = glm::lookAt(position, cameraTarget, cameraUp);

    this->proyection = glm::perspective(glm::radians(fovY), aspecto, zNear, zFar);
}

/***
 * @brief Funcion que aplica la rotacion Pan a la camara
 * @param amount Cantidad de rotacion (segun signo dirección)
 */
void PAG::Camera::applyPanRotation(float amount) {

    glm::mat4 traslacion = glm::translate(glm::mat4(1.f),position);
    traslacion = glm::rotate(traslacion, glm::radians(amount),cameraUp);
    traslacion = glm::translate(traslacion,-position);
    glm::vec4 trasLacionAplicada = traslacion * glm::vec4(cameraTarget,1);
    cameraTarget.x = trasLacionAplicada.x;
    cameraTarget.y = trasLacionAplicada.y;
    cameraTarget.z = trasLacionAplicada.z;

    calculateCameraVector();
}

/***
 * @brief Funcion que aplica la rotación Tilt a la camara
 * @param amount Cantidad de rotacion
 */
void PAG::Camera::applyTiltRotation(float amount) {

    glm::mat4 traslacion = glm::translate(glm::mat4(1.f),position);
    traslacion = glm::rotate(traslacion, glm::radians(amount),cameraRight);
    traslacion = glm::translate(traslacion,-position);
    glm::vec4 trasLacionAplicada = traslacion * glm::vec4(cameraTarget,1);
    cameraTarget.x = trasLacionAplicada.x;
    cameraTarget.y = trasLacionAplicada.y;
    cameraTarget.z = trasLacionAplicada.z;
    calculateCameraVector();
}

/***
 * @brief Funcion que realiza el movimiento Dolly de la camara
 * @param sideAmount Cantidad para movimiento de lado
 * @param advanceAmount Cantidad de movimiento de avance
 */
void PAG::Camera::applyDollyCamera(float sideAmount, float advanceAmount) {
    position.x = position.x + sideAmount;
    position.z = position.z + advanceAmount;
    cameraTarget.x = cameraTarget.x + sideAmount;
    cameraTarget.z = cameraTarget.z + advanceAmount;
    calculateCameraVector();
}

/***
 * @brief Funcion que aplica movimieto Crane a la camara
 * @param amount Cantidad de movimiento
 */
void PAG::Camera::applyCraneCamera(float amount) {
    position.y = position.y + amount;
    cameraTarget.y = cameraTarget.y + amount;
    calculateCameraVector();
}

/***
 * @brief Funcion que aplica movimiento Orbit a la camara
 * @param amountX Cantidad en el eje X
 * @param amountY Cantidad en el eje Y
 */
void PAG::Camera::applyOrbitRotation(float amountX, float amountY) {
    glm::mat4 rot = glm::rotate(glm::mat4(1.f), glm::radians(amountX), cameraUp);
    rot = glm::rotate(rot, glm::radians(amountY), cameraRight);

    glm::vec4 rotacionAplicada = rot * glm::vec4(position, 1);

    position.x = rotacionAplicada.x;
    position.y = rotacionAplicada.y;
    position.z = rotacionAplicada.z;

    calculateCameraVector();
}

/***
 * @brief Funcion que aplica el zoom a la cámara
 * @param amount Cantidad de desplazamiento
 */
void PAG::Camera::applyZoom(float amount) {
    fovY += amount;
    this->proyection = glm::perspective(glm::radians(fovY), aspecto, zNear, zFar);
}

/**
 * Getters y Setters
 */
glm::mat4 PAG::Camera::getViewMatrix() {
    return view;
}

glm::mat4 PAG::Camera::getProjectionMatrix() {
    return proyection;
}

PAG::MOVEMENT_TYPE PAG::Camera::getActiveMove() {
    return activeMove;
}

void PAG::Camera::setActiveMove(MOVEMENT_TYPE mov) {
    activeMove = mov;
}
