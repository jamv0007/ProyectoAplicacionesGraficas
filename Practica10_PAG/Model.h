//
// Created by Jose Antonio on 23/12/21.
//

#ifndef PRACTICA10_PAG_MODEL_H
#define PRACTICA10_PAG_MODEL_H

#include "Shader.h"
#include "Material.h"
#include "lodepng.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

namespace PAG {

    enum class MODEL_TYPE{
        TRIANGLE,
        TETRAHEDRON,
        COW,
        DICE,
        CYLINDER
    };

    class Model {
        private:
            //Tipo de modelo y dibujo
            MODEL_TYPE model; //Modelo correspondiente
            bool wire = false; //Modo alambre activo
            bool texture = false; //Textura activa
            bool normalMapping = false;

            //Ids del objeto para dibujar
            GLuint idVAO = 0; // Identificador del vertex array object
            GLuint idVBO = 0; // Identificador del vertex buffer object
            GLuint idIBO = 0; // Identificador del index buffer object
            GLuint idVBOLinked[5];//Se crea un id del buffer object para los id de ambos vectores


            //Matriz de modelado
            glm::mat4 modelMatrix = glm::mat4(1.f); //Matrix de modelado del modelo

            //Material del modelo
            Material* material = nullptr;

            //Carga de los arrays de vertices, normales, indices, tangentes y bitangentes
            GLfloat* vertexArray = nullptr;
            GLfloat* normalArray = nullptr;
            GLfloat* coordinatesTextureArray = nullptr;
            GLfloat* tangentArray = nullptr;
            GLfloat* bitangentArray = nullptr;
            GLuint* indexArray = nullptr;



            //Tamaño de los arrays
            int vertexNumber = 0; //Numero de vertices
            int normalNumber = 0;
            int coordinatesTextureNumber = 0;
            int tangentNumber = 0;
            int bitangentNumber = 0;
            int indexNumber = 0; //Numero de indices

            //Texturas del modelo
            std::string textureFile = "";
            std::string normalMappingFile = "";
            GLuint textureId = -1;
            GLuint normalMappingId = -1;

            //Funciones privadas
            void buildModel();
            void loadObjectModel();
            void loadModel(std::string rute);
            GLuint loadTexture(std::string file);
            void loadRevolutionModel(GLfloat* generatrix,int generatrixTam, float longitudeDivisions);
            void calculateNormals(GLfloat* vertexArray, GLuint* indexArray);
            void calculateRevolutionNormals();


        public:
            Model(MODEL_TYPE model);
            Model(MODEL_TYPE model, std::string texture);
            Model(MODEL_TYPE model, std::string texture, std::string normalMap);
            virtual ~Model();
            Model(const Model& orig);
            void bindVAO_IBO();
            Material* getMaterial();
            MODEL_TYPE getModel();
            glm::mat4 getModelMatrix();
            void setModelMatrix(glm::mat4 newModelMatrix);
            int getIndexNumber();
            bool getWire();
            void setWire(bool value);
            bool getTexture();
            void setTexture(bool value);
            GLuint getTextureId();
            bool getNormalMapping();
            void setNormalMapping(bool value);
            GLuint getNormalMappingId();


    };
}

#endif //PRACTICA10_PAG_MODEL_H
