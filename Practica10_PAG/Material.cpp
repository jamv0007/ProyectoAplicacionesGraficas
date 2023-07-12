//
// Created by Jose Antonio on 23/12/21.
//

#include "Material.h"
/***
 * @brief Constructor del material
 */
PAG::Material::Material() {

}
/***
 * @brief Destructor del material
 */
PAG::Material::~Material() {

}

/***
 * @brief Constructor copia del material
 * @param orig Objeto a copiar
 */
PAG::Material::Material(const Material &orig) {
    this->Kd = orig.Kd;
    this->Ka = orig.Ka;
    this->Ks = orig.Ks;
    this->ns = orig.ns;
}

/**
 * Getters y Setters
 */
glm::vec3 PAG::Material::getKa() {
    return Ka;
}

glm::vec3 PAG::Material::getKd() {
    return Kd;
}

glm::vec3 PAG::Material::getKs() {
    return Ks;
}

float PAG::Material::getNs() {
    return ns;
}

void PAG::Material::setKa(glm::vec3 value) {
    this->Ka = value;
}

void PAG::Material::setKd(glm::vec3 value) {
    this->Kd = value;
}

void PAG::Material::setKs(glm::vec3 value) {
    this->Ks = value;
}

void PAG::Material::setNs(float value) {
    this->ns = value;
}