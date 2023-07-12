//
// Created by Jose Antonio on 23/12/21.
//

#include "Model.h"
/***
 * @brief Constructor de modelo
 * @param model Tipo de modelo
 */
PAG::Model::Model(MODEL_TYPE model) {
    this->model = model;
    this->material = new Material();
    buildModel();
}
/***
 * @brief Constructor de modelo
 * @param model Tipo de modelo
 * @param texture Nombre de la imagen de la textura
 */
PAG::Model::Model(MODEL_TYPE model, std::string texture) {
    this->model = model;
    this->material = new Material();
    this->textureFile = texture;
    buildModel();
}

/***
 * @brief Constructor de modelo
 * @param model Tipo de modelo
 * @param texture Nombre de la imagen de la textura
 * @param normalMap Nombre de la imagen del mapa de normales para normal mapping
 */
PAG::Model::Model(MODEL_TYPE model, std::string texture, std::string normalMap) {
    this->model = model;
    this->textureFile = texture;
    this->material = new Material();
    this->normalMappingFile = normalMap;
    buildModel();
}

/***
 * @brief Destructor del modelo
 */
PAG::Model::~Model() {

    delete material;

    if(textureFile != "") {
        glDeleteTextures(1, &textureId);
    }

    if(normalMappingFile != ""){
        glDeleteTextures(1,&normalMappingId);
    }



    switch (model) {
        case MODEL_TYPE::TRIANGLE:
            //Si es entrelazado o no se borran los elementos
            glDeleteBuffers(2,idVBOLinked);
            break;
        case MODEL_TYPE::TETRAHEDRON:
            //Si es entrelazado o no se borran los elementos
            glDeleteBuffers(2,idVBOLinked);
            break;
        case MODEL_TYPE::CYLINDER:
            //Si es entrelazado o no se borran los elementos
            glDeleteBuffers(2,idVBOLinked);
            break;
        case MODEL_TYPE::COW:
            //Si es entrelazado o no se borran los elementos
            glDeleteBuffers(3,idVBOLinked);
            break;
        case MODEL_TYPE::DICE:
            //Si es entrelazado o no se borran los elementos
            glDeleteBuffers(5,idVBOLinked);
            break;
    }


    if (vertexArray != nullptr) {
        delete[] vertexArray;
        vertexArray = nullptr;
    }
    if (normalArray != nullptr) {
        delete[] normalArray;
        normalArray = nullptr;
    }

    if (indexArray != nullptr) {
        delete[] indexArray;
        indexArray = nullptr;
    }


    if(coordinatesTextureArray != nullptr){
        delete[] coordinatesTextureArray;
        coordinatesTextureArray = nullptr;
    }


    if(tangentArray != nullptr){
        delete[] tangentArray;
        tangentArray = nullptr;
    }

    if(bitangentArray != nullptr){
        delete[] bitangentArray;
        bitangentArray = nullptr;
    }


    //Se borra el VAO
    glDeleteVertexArrays(1,&idVAO);
    if(!textureFile.empty()) {
        glDeleteTextures(1,&textureId);
    }
    if(!normalMappingFile.empty()){
        glDeleteTextures(1,&normalMappingId);
    }
}
/***
 * @brief Constructor copia del modelo
 * @param orig Objeto a copiar
 */
PAG::Model::Model(const Model &orig) {
    this->model = orig.model;
    this->wire = orig.wire;
    this->texture = orig.texture;
    this->idVAO = orig.idVAO;
    this->idVBO = orig.idVBO;
    this->idIBO = orig.idIBO;
    for(int i = 0; i < 3; i++){
        this->idVBOLinked[i] = orig.idVBOLinked[i];
    }
    this->modelMatrix = orig.modelMatrix;
    this->material = new Material(*orig.material);

    this->vertexNumber = orig.vertexNumber;
    this->indexNumber = orig.indexNumber;
    this->normalNumber = orig.normalNumber;
    this->tangentNumber = orig.tangentNumber;
    this->bitangentNumber = orig.bitangentNumber;

    if(orig.vertexArray != nullptr){
        this->vertexArray = new GLfloat[orig.vertexNumber];
        for(int i = 0; i < vertexNumber; i++){
            this->vertexArray[i] = orig.vertexArray[i];
        }
    }else{
        vertexArray = nullptr;
    }

    if(orig.normalArray != nullptr){
        this->normalArray = new GLfloat[orig.normalNumber];
        for(int i = 0; i < normalNumber; i++){
            this->normalArray[i] = orig.normalArray[i];
        }
    }else{
        normalArray = nullptr;
    }

    if(orig.indexArray != nullptr){
        this->indexArray = new GLuint[orig.indexNumber];
        for(int i = 0; i < indexNumber; i++){
            this->indexArray[i] = orig.indexArray[i];
        }
    }else{
        this->indexArray = nullptr;
    }

    if(orig.tangentArray != nullptr){
        this->tangentArray = new GLfloat[orig.tangentNumber];
        for(int i = 0; i < tangentNumber; i++){
            this->tangentArray[i] = orig.tangentArray[i];
        }
    }else{
        this->tangentArray = nullptr;
    }

    if(orig.bitangentArray != nullptr){
        this->bitangentArray = new GLfloat[orig.bitangentNumber];
        for(int i = 0; i < bitangentNumber; i++){
            this->bitangentArray[i] = orig.bitangentArray[i];
        }
    }else{
        this->bitangentArray = nullptr;
    }

    this->textureFile = orig.textureFile;
    this->textureId = orig.textureId;
    this->normalMappingFile = orig.normalMappingFile;
    this->normalMappingId = orig.normalMappingId;
}

void PAG::Model::buildModel() {

    switch (model) {
        case MODEL_TYPE::TRIANGLE:
            loadObjectModel();
            break;
        case MODEL_TYPE::TETRAHEDRON:
            loadObjectModel();
            break;
        case MODEL_TYPE::COW:
            loadModel("vaca.obj");
            loadObjectModel();
            if(!textureFile.empty()){
                textureId = loadTexture(textureFile);
            }
            break;
        case MODEL_TYPE::DICE:
            loadModel("dado.obj");
            loadObjectModel();
            if(!textureFile.empty()){
                textureId = loadTexture(textureFile);
            }
            if(!normalMappingFile.empty()){
                normalMappingId = loadTexture(normalMappingFile);
            }
            break;
        case MODEL_TYPE::CYLINDER:
            GLfloat generatrixPoints[] = {0.3,0.5,0.0,0.3,0.0,0.0};
            int vertexGeneratrixNumber = 6;//Con las componentes en 3d
            float divU = 60.0;
            loadRevolutionModel(generatrixPoints,vertexGeneratrixNumber,divU);
            loadObjectModel();
            break;
    }

}
/***
 * @brief Funcion para calculo de las normales de un modelo (El tamaño instanciado como atributo)
 * @param vertexArray Array de vertices
 * @param indexArray Array de indices
 */
void PAG::Model::calculateNormals(GLfloat *vertexArray, GLuint *indexArray) {

    normalNumber = vertexNumber;
    normalArray = new GLfloat[normalNumber];

    for(int i = 0; i < indexNumber; i=i+3) {
        //Calculo de la distancia de dos vectores del triangulo
        GLfloat ux = vertexArray[indexArray[i + 1] * 3] - vertexArray[indexArray[i] * 3];
        GLfloat uy = vertexArray[(indexArray[i + 1] * 3) + 1] - vertexArray[(indexArray[i] * 3) + 1];
        GLfloat uz = vertexArray[(indexArray[i + 1] * 3) + 2] - vertexArray[(indexArray[i] * 3) + 2];

        GLfloat vx = vertexArray[indexArray[i + 2] * 3] - vertexArray[indexArray[i] * 3];
        GLfloat vy = vertexArray[(indexArray[i + 2] * 3) + 1] - vertexArray[(indexArray[i] * 3) + 1];
        GLfloat vz = vertexArray[(indexArray[i + 2] * 3) + 2] - vertexArray[(indexArray[i] * 3) + 2];

        //Se hace el producto vectorial
        glm::vec3 u = glm::vec3(ux, uy, uz);
        glm::vec3 v = glm::vec3(vx, vy, vz);

        glm::vec3 uv = glm::cross(v, u);

        //Normalizan
        uv = glm::normalize(uv);

        //Se almacenan en el array de normales
        normalArray[indexArray[i] * 3] =-(GLfloat)uv.x;
        normalArray[(indexArray[i] * 3)+1] =-(GLfloat) uv.y;
        normalArray[(indexArray[i] * 3)+2] =-(GLfloat) uv.z;

        normalArray[indexArray[i + 1] * 3] =-(GLfloat) uv.x;
        normalArray[(indexArray[i + 1] * 3) + 1] =-(GLfloat) uv.y;
        normalArray[(indexArray[i + 1] * 3) + 2] =-(GLfloat) uv.z;

        normalArray[indexArray[i + 2] * 3] =-(GLfloat) uv.x;
        normalArray[(indexArray[i + 2] * 3) + 1] =-(GLfloat) uv.y;
        normalArray[(indexArray[i + 2] * 3) + 2] =-(GLfloat) uv.z;
    }


}

/***
 * @brief Calcula las normales para el modelo de revolucion
 * @param vertexArray Array de vertices
 */
void PAG::Model::calculateRevolutionNormals() {
    normalNumber = vertexNumber;
    normalArray = new GLfloat[normalNumber];
    for(int i = 0; i < vertexNumber; i++){
        normalArray[i] = vertexArray[i];
    }

}


/***
 * @brief Función que crea el VAO, los VBOs de las propiedades del modelo y los IBOs
 */
void PAG::Model::loadObjectModel() {

    if(model == MODEL_TYPE::TRIANGLE){
        //vectores vertices para no enlazados
        indexNumber = 6;
        vertexNumber = 18;
        normalNumber = 18;

        vertexArray = new GLfloat[vertexNumber];
        normalArray = new GLfloat[normalNumber];
        indexArray = new GLuint[indexNumber];

        //Vectores de vertices y colores
        GLfloat vertex[] = {-0.5,-0.5,0, -0.5,-0.5,0, 0.5,-0.5,0, 0.5,-0.5,0, 0.0,0.5,0, 0.0,0.5,0};
        GLuint index[] = {0,2,4, 1,3,5};
        GLfloat normal[] = {0,0,1, 0,0,-1, 0,0,1, 0,0,-1, 0,0,1, 0,0,-1};

        for(int i = 0; i < vertexNumber; i++){
            vertexArray[i] = vertex[i];
            normalArray[i] = normal[i];
        }

        for(int i = 0; i < indexNumber; i++){
            indexArray[i] = index[i];
        }

    }else if(model == MODEL_TYPE::TETRAHEDRON){

        //vectores vertices para no enlazados
        indexNumber = 12;
        vertexNumber = 36;

        vertexArray = new GLfloat[vertexNumber];
        indexArray = new GLuint[indexNumber];

        GLfloat vertex[] = {-0.5,-0.5,0, -0.5,-0.5,0, -0.5,-0.5,0, 0.5,-0.5,0, 0.5,-0.5,0, 0.5,-0.5,0, 0.0,0.5,0.0, 0.0,0.5,0.0, 0.0,0.5,0.0, 0.0,-0.5,-0.5, 0.0,-0.5,-0.5, 0.0,-0.5,-0.5};
        GLuint index[] = {0,3,6, 4,9,7, 10,1,8, 2,5,11};
        calculateNormals(vertex,index);

        for(int i = 0; i < vertexNumber; i++){
            vertexArray[i] = vertex[i];
        }

        for(int i = 0; i < indexNumber; i++){
            indexArray[i] = index[i];
        }

    }else if(model == MODEL_TYPE::CYLINDER){
        calculateRevolutionNormals();
    }

    //Se generan el numero de arrays de vertices
    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);
    //Se crean dos buffers y se asignan al id de antes
    if(model == MODEL_TYPE::TRIANGLE || model == MODEL_TYPE::TETRAHEDRON || model == MODEL_TYPE::CYLINDER){
        glGenBuffers(2, idVBOLinked);
    }else if(model == MODEL_TYPE::COW){
        glGenBuffers(3, idVBOLinked);
    }else if(model == MODEL_TYPE::DICE){
        glGenBuffers(5, idVBOLinked);
    }
    //Se asigna el buffer y se lee los vertices
    glBindBuffer(GL_ARRAY_BUFFER, idVBOLinked[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    //Se asigna el buffer de color y se asigna
    glBindBuffer(GL_ARRAY_BUFFER, idVBOLinked[1]);
    glBufferData(GL_ARRAY_BUFFER, normalNumber * sizeof(GLfloat), normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    if(model == MODEL_TYPE::COW || model == MODEL_TYPE::DICE) {
        glBindBuffer(GL_ARRAY_BUFFER, idVBOLinked[2]);
        glBufferData(GL_ARRAY_BUFFER, coordinatesTextureNumber * sizeof(GLfloat), coordinatesTextureArray,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
    }

    if(model == MODEL_TYPE::DICE) {
        glBindBuffer(GL_ARRAY_BUFFER, idVBOLinked[3]);
        glBufferData(GL_ARRAY_BUFFER, tangentNumber * sizeof(GLfloat), tangentArray, GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, idVBOLinked[4]);
        glBufferData(GL_ARRAY_BUFFER, bitangentNumber * sizeof(GLfloat), bitangentArray, GL_STATIC_DRAW);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(4);
    }

    //vector de indices

    glGenBuffers(1,&idIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexNumber*sizeof(GLuint),indexArray,GL_STATIC_DRAW);


}


/***
 * @brief Función que carga el modelo .obj
 * @param rute Ruta al archivo obj
 */
void PAG::Model::loadModel(std::string rute) {

    //Importamos el modelo
    Assimp::Importer import;
    const aiScene* escena = import.ReadFile(rute, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    //Si no se ha importado bien
    if(!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode){
        std::string mensajeError = "Error::Assimp::";
        throw std::runtime_error(mensajeError + import.GetErrorString());

    }

    //Como solo hay una malla
    aiNode* node = escena->mRootNode->mChildren[0];

    //Se calculan los tamaños de los vectores de vertices, normales ,...
    aiMesh *mesh1 = escena->mMeshes[node->mMeshes[0]];
    aiFace face = mesh1->mFaces[0];

    this->vertexNumber =  node->mNumMeshes * mesh1->mNumVertices * 3;
    this->normalNumber = node->mNumMeshes * mesh1->mNumVertices * 3;
    this->tangentNumber = node->mNumMeshes * mesh1->mNumVertices * 3;
    this->bitangentNumber = node->mNumMeshes * mesh1->mNumVertices * 3;
    this->coordinatesTextureNumber = node->mNumMeshes * mesh1->mNumVertices * 2;
    this->indexNumber = node->mNumMeshes * mesh1->mNumFaces * face.mNumIndices;

    //Inicializan los vectores
    this->vertexArray = new GLfloat[vertexNumber];
    this->normalArray = new GLfloat[normalNumber];
    this->coordinatesTextureArray = new GLfloat[coordinatesTextureNumber];
    if(mesh1->mTangents){
        this->tangentArray = new GLfloat[tangentNumber];
        this->bitangentArray = new GLfloat[bitangentNumber];
    }
    this->indexArray = new GLuint[indexNumber];

    int countVertex = 0;
    int countTexture = 0;
    int countIndex = 0;
    int countTangents = 0;

    //Se recorren las mallas y nodos para recoger los vertices y otros elmentos
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = escena->mMeshes[node->mMeshes[i]];
        for(int j = 0; j < mesh->mNumVertices; j++){
            //Vertices, y normales
            vertexArray[countVertex] = mesh->mVertices[j].x;
            normalArray[countVertex] = mesh->mNormals[j].x;
            countVertex++;
            vertexArray[countVertex] = mesh->mVertices[j].y;
            normalArray[countVertex] = mesh->mNormals[j].y;
            countVertex++;
            vertexArray[countVertex] = mesh->mVertices[j].z;
            normalArray[countVertex] = mesh->mNormals[j].z;
            countVertex++;

            //Coordenadas de textura
            if(mesh->mTextureCoords[0]){
                coordinatesTextureArray[countTexture] = mesh->mTextureCoords[0][j].x;
                countTexture++;
                coordinatesTextureArray[countTexture] = mesh->mTextureCoords[0][j].y;
                countTexture++;
            }

            if(mesh->mTangents){
                tangentArray[countTangents] = mesh->mTangents[j].x;
                bitangentArray[countTangents] = mesh->mBitangents[j].x;
                countTangents++;
                tangentArray[countTangents] = mesh->mTangents[j].y;
                bitangentArray[countTangents] = mesh->mBitangents[j].y;
                countTangents++;
                tangentArray[countTangents] = mesh->mTangents[j].z;
                bitangentArray[countTangents] = mesh->mBitangents[j].z;
                countTangents++;
            }

        }


        //Para cada cara de la malla, los indices que la componen
        for(int j = 0; j < mesh->mNumFaces; j++){
            aiFace cara = mesh->mFaces[j];
            for(int k = 0; k < cara.mNumIndices; k++){
                indexArray[countIndex] = cara.mIndices[k];
                countIndex++;
            }
        }
    }

}

/***
 * @brief Funcion que carga un modelo de revolucion en el array de vertices y de indices del modelo
 * @param generatrix Array de los vertices de la generatriz, x, y, z para cada vertice
 * @param generatrixTam Tamaño del array de vertices de la generatriz
 * @param longitudeDivisions Divisiones en longitud para el modelo de revolución
 */
void PAG::Model::loadRevolutionModel(GLfloat* generatrix,int generatrixTam, float longitudeDivisions) {

    //Vertices

    vertexNumber = generatrixTam * ((int)longitudeDivisions+1);
    vertexArray = new GLfloat[vertexNumber];

    int cont = 0;
    float complex = 360.0;
    float angle = complex / longitudeDivisions;


    for(int i = 0; i < generatrixTam; i=i+3){
        for(int j = 0; j <= longitudeDivisions; j++){
            float anglePoint = (float) j * angle;
            float x = generatrix[i] * (float) glm::cos((anglePoint * 3.14) / 180);
            float z = generatrix[i] * (float) glm::sin((anglePoint * 3.14) / 180);

            vertexArray[cont] = x;
            cont++;
            vertexArray[cont] = generatrix[i+1];
            cont++;
            vertexArray[cont] = z;
            cont++;


        }


    }

    //Indices
    int n = generatrixTam / 3;
    indexNumber = longitudeDivisions * ((n * 2) + 1) ;
    indexArray = new GLuint[indexNumber];
    int contIndex = 0;

    for(int i = 0; i < longitudeDivisions; i++){
        for(int j = 0; j < n; j++){
            indexArray[contIndex] = (j*(longitudeDivisions+1))+i;
            contIndex++;
            indexArray[contIndex] = (j*(longitudeDivisions+1))+(i+1);
            contIndex++;
        }

        indexArray[contIndex] = 0xFFFF;
        contIndex++;
    }



}

/***
 * @brief Funcion que carga una textura
 * @param file Archivo de textura a cargar
 * @return Id generado de la textura
 */
GLuint PAG::Model::loadTexture(std::string file) {

    std::vector<unsigned char> imagen;
    unsigned ancho, alto;
    unsigned error = lodepng::decode(imagen,ancho,alto,file);

    if(error){
        std::string mensaje = file + " no se puede cargar";
        throw std::runtime_error(mensaje);
    }

    unsigned char* imgPtr = &imagen[0];
    int numeroComponentesColor = 4;
    int incrementoAncho = ancho * numeroComponentesColor;
    unsigned char* top = nullptr;
    unsigned char* bottom = nullptr;
    unsigned char temp = 0;

    for(int i = 0; i < alto/2; i++){
        top = imgPtr + i * incrementoAncho;
        bottom = imgPtr + (alto -i -1) * incrementoAncho;
        for(int j = 0; j < incrementoAncho; j++){
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            ++top;
            ++bottom;
        }
    }

    GLuint aux;
    glGenTextures(1,&aux);
    glBindTexture(GL_TEXTURE_2D,aux);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ancho,alto,0,GL_RGBA,GL_UNSIGNED_BYTE, imagen.data());

    return aux;
}


/***
 * @brief Función que enlaza el VAO y el IBO del modelo creado
 */
void PAG::Model::bindVAO_IBO() {

    glBindVertexArray(idVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);

}


/**
 * Getters y Setters
 */
PAG::Material* PAG::Model::getMaterial() {
    return material;
}

PAG::MODEL_TYPE PAG::Model::getModel() {
    return model;
}

glm::mat4 PAG::Model::getModelMatrix() {
    return modelMatrix;
}

void PAG::Model::setModelMatrix(glm::mat4 newModelMatrix) {
    this->modelMatrix = newModelMatrix;
}

int PAG::Model::getIndexNumber() {
    return indexNumber;
}


bool PAG::Model::getWire() {
    return wire;
}

bool PAG::Model::getTexture() {
    return texture;
}

void PAG::Model::setWire(bool value) {
    this->wire = value;
}

void PAG::Model::setTexture(bool value) {
    if(textureFile != ""){
        this->texture = value;
        if(!value){
            normalMapping = false;
        }
    }

}

GLuint PAG::Model::getTextureId() {
    return textureId;
}

bool PAG::Model::getNormalMapping() {
    return normalMapping;
}

void PAG::Model::setNormalMapping(bool value) {
    if(normalMappingFile != ""){
        normalMapping = value;
    }

}

GLuint PAG::Model::getNormalMappingId() {
    return normalMappingId;
}