//
// Created by Jose Antonio on 25/12/21.
//

#include "Light.h"
/***
 * @brief Constructor de luz
 */
PAG::Light::Light() {

}
/***
 * @brief Destructor de la luz
 */
PAG::Light::~Light() {
    if(light == LIGHT_TYPE::DIRECTIONAL || light == LIGHT_TYPE::SPOTLIGHT){
        glDeleteTextures(1,&shadowTexture);
    }
}
/***
 * @brief Constructor copia de la luz
 * @param orig Objeto a copiar
 */
PAG::Light::Light(const Light &orig) {

    this->light = orig.light;
    this->Ia = orig.Ia;
    this->Id = orig.Id;
    this->Is = orig.Is;
    this->gamma = orig.gamma;
    this->softEdges = orig.softEdges;
    this->position = orig.position;
    this->direction = orig.direction;
}

/***
 * @brief Funcion que activa la luz como ambiente
 * @param Ia Intensidad ambiental de la luz
 */
void PAG::Light::setAmbientLight(glm::vec3 Ia) {

    this->light = LIGHT_TYPE::AMBIENT;
    this->Ia = Ia;
}

/***
 * @brief Función que activa la luz Puntual
 * @param position Posición de la luz
 * @param Id Intensidad difusa
 * @param Is Intensidad Especular
 */
void PAG::Light::setPointLight(glm::vec3 position, glm::vec3 Id, glm::vec3 Is) {

    this->light = LIGHT_TYPE::POINT;
    this->position = position;
    this->Id = Id;
    this->Is = Is;
}

/***
 * @brief Funcióon que activa la luz direccional
 * @param direction Direción de la luz
 * @param Id Intensidad difusa
 * @param Is Intensidad Especular
 */
void PAG::Light::setDirectionalLight(glm::vec3 direction, glm::vec3 Id, glm::vec3 Is) {

    this->light = LIGHT_TYPE::DIRECTIONAL;
    this->direction = direction;
    this->Id = Id;
    this->Is = Is;

    //Se calculan las matrices de vista y proyeccion
    vM = glm::lookAt ( glm::vec3 (5.0,5.0,5.0) * (-direction), glm::vec3 ( 0, 0, 0 ),
                       glm::vec3 ( 0, 1, 0 ) );
    pM = glm::ortho ( -3.0, 3.0, -3.0, 3.0, 0.1, 10.0 );
    //Se crea la textura
    glGenTextures(1,&shadowTexture);


}
/***
 * @brief Función que activa la luz foco
 * @param position Posicón de la luz
 * @param direction Dirección de la luz
 * @param Id Intensidad difusa
 * @param Is Intensidad especular
 * @param gamma Ángulo del foco
 * @param softEdges Exponente de bordes del foco
 */
void PAG::Light::setSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 Id, glm::vec3 Is, float gamma,
                              float softEdges) {

    this->light = LIGHT_TYPE::SPOTLIGHT;
    this->Id = Id;
    this->Is = Is;
    this->position = position;
    this->direction = direction;
    this->gamma = gamma;
    this->softEdges = softEdges;

    //Se crean las matrices y se habilita la textura
    vM = glm::lookAt(position,position+direction,glm::vec3(0,1,0));
    pM = glm::perspective(2*glm::radians(gamma),(float)1024/(float)1024,0.1f,10.0f);

    glGenTextures(1,&shadowTexture);
}


/**
 * Getters y Setters
 */
glm::vec3 PAG::Light::getIa() {
    return Ia;
}

glm::vec3 PAG::Light::getId() {
    return Id;
}

glm::vec3 PAG::Light::getIs() {
    return Is;
}

float PAG::Light::getGamma() {
    return gamma;
}

PAG::LIGHT_TYPE PAG::Light::getLight() {
    return light;
}

glm::vec3 PAG::Light::getPosition() {
    return position;
}

glm::vec3 PAG::Light::getDirection() {
    return direction;
}

float PAG::Light::getSoftEdges() {
    return softEdges;
}

glm::mat4 PAG::Light::getPm() {
    return pM;
}

glm::mat4 PAG::Light::getVm() {
    return vM;
}

GLuint PAG::Light::getShadowTexture() {
    return shadowTexture;
}

