//
// Created by Jose Antonio on 23/12/21.
//

#ifndef PRACTICA10_PAG_MATERIAL_H
#define PRACTICA10_PAG_MATERIAL_H
#include <glm/glm.hpp>

namespace PAG {
    class Material {
        private:
            //Componentes del material
            glm::vec3 Kd = glm::vec3 (1.0f,1.0f,1.0f);
            glm::vec3 Ks = glm::vec3 (1.0f,1.0f,1.0f);
            glm::vec3 Ka = glm::vec3 (0.0f,0.0f,0.5f);
            //Exponente de phong
            float ns = 120.0;

        public:
            Material();
            virtual ~Material();
            Material(const Material& orig);
            glm::vec3 getKd();
            glm::vec3 getKs();
            glm::vec3 getKa();
            float getNs();
            void setKd(glm::vec3 value);
            void setKs(glm::vec3 value);
            void setKa(glm::vec3 value);
            void setNs(float value);
    };
}
#endif //PRACTICA10_PAG_MATERIAL_H
