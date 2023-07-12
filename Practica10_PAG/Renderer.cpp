//
// Created by Jose Antonio on 23/12/21.
//

#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;

/***
 * @brief Constructor del Renderer
 */
PAG::Renderer::Renderer() {
    //Inicializacion colores
    r = 0.6;
    g = 0.6;
    b = 0.6;

    //Se crea la cámara
    glm::vec3 position(0.0f,0.0f,3.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    glm::vec3 up(0.0f,1.0f,0.0f);
    camara = new Camera(position,cameraTarget,up);

    //--Luces--

    //Luz ambiente
    Light* ambient = new Light();
    glm::vec3 Ia = glm::vec3(0.5,0.5,0.5);
    ambient->setAmbientLight(Ia);
    //Luz puntual
    Light* point = new Light();
    glm::vec3 p = glm::vec3(5.0,0.0,0.0);
    glm::vec3 IdPuntual = glm::vec3(0.5,0.5,0.5);
    glm::vec3 IsPuntual = glm::vec3(1.0,1.0,1.0);
    point->setPointLight(p,IdPuntual,IsPuntual);
    //Luz direccional
    Light* directional = new Light();
    glm::vec3 direction = glm::vec3(1,0,0);
    glm::vec3 IdDirectional = glm::vec3(0.5,0.5,0.5);
    glm::vec3 IsDirectional = glm::vec3(1.0,1.0,1.0);
    directional->setDirectionalLight(direction,IdDirectional,IsDirectional);
    //Foco de luz
    Light* spotLight = new Light();
    glm::vec3 positionSpot = glm::vec3(-1,0,2.5);
    glm::vec3 directionSpot = glm::vec3(0,0,-1);
    glm::vec3 IdSpot = glm::vec3(0.5,0.5,0.5);
    glm::vec3 IsSpot = glm::vec3(1.0,1.0,1.0);
    spotLight->setSpotLight(positionSpot,directionSpot,IdSpot,IsSpot,60.0,256.0);

    //Se añaden
    lights.push_back(ambient);
    lights.push_back(point);
    lights.push_back(spotLight);
    lights.push_back(directional);

    //Se crean los shaders
    shaders.push_back(new Shader("pag03-vs.glsl", "pag03-fs.glsl"));
    shaders.push_back(new Shader("pagShadow-vs.glsl", "pagShadow-fs.glsl"));

    //Se añaden los modelos por defecto
    addModel(MODEL_TYPE::COW);
    addModel(MODEL_TYPE::DICE);
    addModel(MODEL_TYPE::TETRAHEDRON);
    addModel(MODEL_TYPE::TRIANGLE);
    addModel(MODEL_TYPE::CYLINDER);
    //Se genera el framebuffer del fbo
    glGenFramebuffers(2,shadowFbo);
    shadowMap();

}
/**
 * @brief Destructor de renderer
 */
PAG::Renderer::~Renderer() {

    for(int i = 0; i < shaders.size(); i++){
        delete shaders[i];
    }

    shaders.clear();

    for(int i = 0; i < models.size(); i++){
        delete models[i];
    }
    models.clear();

    delete camara;

    for(int i = 0; i < lights.size(); i++){
        delete lights[i];
    }
    lights.clear();
}
/**
 * @brief Instancia del renderer
 * @return Instancia
 */
PAG::Renderer *PAG::Renderer::getInstance() {
    if(!instance){
        instance = new Renderer;
    }

    return instance;
}

/**
 * @brief Refresca la ventana de vision
 */
void PAG::Renderer::refresh_Window(int width,int height) {
    //Se crea el mapa de sombras
    shadowRender(width,height);

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //Se crea la matriz de sombra
    glm::mat4 shadowMatrix = glm::scale ( glm::mat4(1.0f),glm::vec3 ( 0.5, 0.5, 0.5 ) );
    shadowMatrix[3][0] = shadowMatrix[3][1] = shadowMatrix[3][2] = 0.5;

    //Activar shader program
    shaders[PAG::USE_SHADER::COMPLETE]->activeShaderProgram();
    GLuint sp = shaders[PAG::USE_SHADER::COMPLETE]->getIdSp();
    //Para cada luz
    for(int j = 0; j < lights.size(); j++){

        if (j == 0) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }
        //Se envian los uniforms de la luz
        sendLightUniforms(sp,j);

        //Se envian los uniforms de los samplers de sombra, color de textura y normal
        glUniform1i(glGetUniformLocation(sp, "ShadowSampler"), 0);
        glUniform1i(glGetUniformLocation(sp, "TexSamplerColor"),1);
        glUniform1i(glGetUniformLocation(sp, "TexNormalSampler"), 2);

        //Se activa la textura de la sombra si la luz es direccional o foco
        if(lights[j]->getLight() == LIGHT_TYPE::DIRECTIONAL || lights[j]->getLight() == LIGHT_TYPE::SPOTLIGHT) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lights[j]->getShadowTexture());

        }

        //Se crea la matriz de luz
        glm::mat4 VPLightMatrix = shadowMatrix * lights[j]->getPm() * lights[j]->getVm();

        for(int i = 0; i < models.size(); i++) {

            //Llamamos a las subrutinas
            callSubroutines(sp, i, j);

            //Enviamos los uniforms del modelo
            sendModelUniforms(sp, i);

            //Enviamos la matrix de sombreado
            glm::mat4 aux = VPLightMatrix * models[i]->getModelMatrix();
            glUniformMatrix4fv(glGetUniformLocation(sp, "shadowMVPMatrix"), 1, GL_FALSE, glm::value_ptr(aux));

            //Activamos la textura
            if(models[i]->getTexture()){
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D,models[i]->getTextureId());

            }

            //Activamos la textura del normal mapping
            if(models[i]->getTexture() && models[i]->getNormalMapping()){
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D,models[i]->getNormalMappingId());
            }

            //Lo dibujamos modo alambre o relleno segun el modelo
            if (models[i]->getWire()) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            //Se dibuja el modelo
            drawModel(i);

        }
    }

}

/***
 * @brief Función que para cada luz que permita mapa de sombras crea el mapa asignado a su identificador del mapa de sombras
 */
void PAG::Renderer::shadowMap() {

    GLfloat borde[] = {1.0, 1.0, 1.0, 1.0};
    GLsizei _anchoMS = 1024;   // Ajusta el tamaño a tu gusto
    GLsizei _altoMS = 1024;
    int cont = 0;

    for(int i = 0; i < lights.size(); i++) {
        if(lights[i]->getLight() == LIGHT_TYPE::DIRECTIONAL || lights[i]->getLight() == LIGHT_TYPE::SPOTLIGHT) {

            //Se crea la textura del mapa de sombras
            glBindTexture(GL_TEXTURE_2D, lights[i]->getShadowTexture());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _anchoMS, _altoMS, 0,
                         GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
            glTexParameterfv ( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borde );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS );

            //Se asigna el framebuffer a cada textura
            glBindFramebuffer ( GL_FRAMEBUFFER, shadowFbo[cont] );
            glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,lights[i]->getShadowTexture(), 0 );
            glDrawBuffer ( GL_NONE );
            glReadBuffer ( GL_NONE );
            GLenum estado = glCheckFramebufferStatus ( GL_FRAMEBUFFER );
            if ( estado != GL_FRAMEBUFFER_COMPLETE ){
                throw std::runtime_error("El framebufer no esta completo");
            }

            glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
            cont++;
        }
    }

}

/***
 * @brief Se crea el mapa de sombras para cada luz
 * @param width Anchura de la pantalla
 * @param height Altura de la pantalla
 */
void PAG::Renderer::shadowRender(int width,int height) {

    //Se activa el shader de sombreado
    shaders[PAG::USE_SHADER::SHADOW]->activeShaderProgram();
    GLuint sp = shaders[PAG::USE_SHADER::SHADOW]->getIdSp();
    int cont = 0;
    //Para cada luz
    for(int i = 0; i < lights.size();i++) {
        if(lights[i]->getLight() == LIGHT_TYPE::DIRECTIONAL || lights[i]->getLight() == LIGHT_TYPE::SPOTLIGHT ) {
            //Se establece el viewport a tamaño del mapa
            glViewport(0, 0, 1024, 1024);
            glBindFramebuffer(GL_FRAMEBUFFER,shadowFbo[cont]);
            //Se usa solo la profundidad
            glClear(GL_DEPTH_BUFFER_BIT);
            //Se desactiva la cara delanteras para evitar shadow acne
            glEnable ( GL_CULL_FACE );
            glCullFace ( GL_FRONT );
            //Se dibuja cada modelo
            for (int j = 0; j < models.size(); j++) {
                glUniformMatrix4fv(glGetUniformLocation(sp, "MVPmatrix"), 1, GL_FALSE, glm::value_ptr((lights[i]->getPm() * lights[i]->getVm()) * models[j]->getModelMatrix()));
                drawModel(j);
            }
            //Se restablece el buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            cont++;
        }

    }

    //Restauramos las opciones por defecto
    glViewport(0, 0, width, height);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

/***
 * @brief Funcion que llama a las subrutinas
 * @param subroutines Vector con las subrutinas llamadas
 * @param tamS Numero de subrutinas y por tanto implementaciones tambien
 * @param sName Vector de nombres de las subrutinas
 * @param iName Vector de nombres de las implementaciones de cada una de las subrutinas
 * @param sp El shader program
 * @param shader Shader donde estan las subrutinas
 */
void PAG::Renderer::subImplementation(GLuint *subroutines, int tamS, std::string *sName, std::string *iName, GLuint sp,
                                      GLenum shader) {
    for(int i = 0; i < tamS; i++){
        GLuint posUniform = glGetSubroutineUniformLocation(sp, shader,sName[i].c_str());

        if (posUniform == -1) {
            throw std::runtime_error("No se ha encontrado el uniform de la subrutina: " + sName[i]);
        }

        subroutines[posUniform] = glGetSubroutineIndex(sp, shader, iName[i].c_str());

    }
}

/***
 * @brief Funcion que llama a las subrutinas
 * @param sp shader program
 * @param model Modelo actual
 * @param light Luz actual
 */
void PAG::Renderer::callSubroutines(GLuint sp,int model, int light) {

    //Se obtiene el numero de subrutinas
    GLint tam;
    glGetProgramStageiv(sp, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &tam);

    //Se crea vector con subrutinas
    GLuint* subroutines = new GLuint[tam];

    //Nombres de las subrutinas que se llaman
    std::string subroutineName[4] = {"selectAmbientColor","selectDiffuseColor","lightType","selectNormal"};
    std::string implementationName[4];

    //Se llenan las implementaciones segun la configuracion del modelo y la luz
    //Si tiene textura activa o no usa el color ambiente/difuso o el de la textura
    if (models[model]->getTexture()) {
        implementationName[0] = "textureColor";
        implementationName[1] = "textureColorDif";
    } else {
        implementationName[0] = "materialColor";
        implementationName[1] = "materialColorDif";
    }

    //Si esta en modo alambre o no
    //No esta en modo alambre, usa la luz correspondiente a la luz actual
    if (!models[model]->getWire()) {
        switch (lights[light]->getLight()) {
            case LIGHT_TYPE::AMBIENT: {
                implementationName[2] = "ambientLight";
                break;
            }
            case LIGHT_TYPE::DIRECTIONAL: {
                implementationName[2] = "directionalLight";
                break;
            }
            case LIGHT_TYPE::POINT: {
                implementationName[2] = "pointLight";
                break;
            }
            case LIGHT_TYPE::SPOTLIGHT: {
                implementationName[2] = "spotLight";
                break;
            }

        }

    } else {
        //Si esta el modo alambre activo usa el color de alambre
        implementationName[2] = "wireMode";
    }

    //Si esta el normal mapping activo se llama a la normal del mapa de normales y
    // se calcula la iluminacion con la TBN; Sino se usa la normal del modelo y los parametros de luz normales
    if (models[model]->getTexture() && models[model]->getNormalMapping()) {
        implementationName[3] = "normalNormalMap";
        GLuint sub = glGetSubroutineIndex(sp, GL_VERTEX_SHADER, "withTBN");
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &sub);
    } else {
        implementationName[3] = "normalModel";
        GLuint sub = glGetSubroutineIndex(sp, GL_VERTEX_SHADER, "withoutTBN");
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &sub);
    }

    //Se activan las implementaciones
    subImplementation(subroutines,tam,subroutineName,implementationName,sp,GL_FRAGMENT_SHADER);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, tam, subroutines);
    delete[] subroutines;
}

/***
 * @brief Función que envia los uniforms de la luz
 * @param sp shader program
 * @param light Luz activa en el momento
 */
void PAG::Renderer::sendLightUniforms(GLuint sp, int light) {

    glUniform3fv(glGetUniformLocation(sp, "Ia"), 1, glm::value_ptr(lights[light]->getIa()));
    glUniform3fv(glGetUniformLocation(sp, "Id"), 1, glm::value_ptr(lights[light]->getId()));
    glUniform3fv(glGetUniformLocation(sp, "Is"), 1, glm::value_ptr(lights[light]->getIs()));
    glUniform1f(glGetUniformLocation(sp, "s"), lights[light]->getSoftEdges());
    glUniform1f(glGetUniformLocation(sp, "spotLightAngle"), lights[light]->getGamma());
    glUniform3fv(glGetUniformLocation(sp, "lightPos"), 1, glm::value_ptr(glm::vec3(camara->getViewMatrix() * glm::vec4(lights[light]->getPosition(), 1.0))));
    glUniform3fv(glGetUniformLocation(sp, "lightDir"), 1, glm::value_ptr(glm::vec3(glm::transpose(glm::inverse(camara->getViewMatrix())) * glm::vec4(lights[light]->getDirection(), 1.0))));


}

/***
 * @brief Función que envia los uniforms del modelo
 * @param sp shader program
 * @param model modelo actual
 */
void PAG::Renderer::sendModelUniforms(GLuint sp, int model) {

    glUniformMatrix4fv(glGetUniformLocation(sp, "MVPmatrix"), 1, GL_FALSE, glm::value_ptr(camara->getProjectionMatrix() * camara->getViewMatrix() *models[model]->getModelMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(sp, "MVmatrix"), 1, GL_FALSE,glm::value_ptr(camara->getViewMatrix() * models[model]->getModelMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(sp, "MVitMatrix"), 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(glm::mat4(camara->getViewMatrix() *models[model]->getModelMatrix())))));

    glUniform3fv(glGetUniformLocation(sp, "wireColor"), 1,glm::value_ptr(glm::vec3(0.5, 0.0, 0.0)));

    glUniform3fv(glGetUniformLocation(sp, "Ka"), 1,glm::value_ptr(models[model]->getMaterial()->getKa()));
    glUniform3fv(glGetUniformLocation(sp, "Kd"), 1,glm::value_ptr(models[model]->getMaterial()->getKd()));
    glUniform3fv(glGetUniformLocation(sp, "Ks"), 1, glm::value_ptr(models[model]->getMaterial()->getKs()));
    glUniform1f(glGetUniformLocation(sp, "ns"), models[model]->getMaterial()->getNs());



}

/**
 * @brief Al redimensionar la ventana
 */
void PAG::Renderer::size_Window(int width, int height){
    glViewport (0,0,width,height);
}


/**
 * @brief Cambio de color
 */
void PAG::Renderer::colorApply() {
    //Se cambia y se actualiza
    glClearColor(r, g, b, 1.0);
}



/**
 * @brief Carga otros parametros de openGL
 */
void PAG::Renderer::loadParameterGL(){
    // - Establecemos un gris medio como color con el que se borrará el frame buffer.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glClearColor (r, g, b, 1.0 );


    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable ( GL_DEPTH_TEST );
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFF);


}
/**
 * @brief Añade un nuevo modelo a la escena de un tipo si no esta
 * @param model Modelo a añadir
 */
void PAG::Renderer::addModel(MODEL_TYPE model) {
    bool exist = false;
    for(int i = 0; i < models.size() && !exist; i++){
        if(models[i]->getModel() == model){
            exist = true;
        }
    }

    if(!exist){
        Model *nuevo;
        try {

            switch (model) {
                case MODEL_TYPE::TRIANGLE:{
                    std::cout<<"Dibujando Triangulo"<<std::endl;
                    nuevo = new Model(model);

                    glm::mat4 mModelado = glm::translate(nuevo->getModelMatrix(),glm::vec3(-1,0.6,-1));
                    nuevo->setModelMatrix(mModelado);
                    break;
                }
                case MODEL_TYPE::TETRAHEDRON:{
                    std::cout<<"Dibujando Tetraedro"<<std::endl;
                    nuevo = new Model(model);

                    glm::mat4 mModelado = glm::translate(nuevo->getModelMatrix(),glm::vec3(-1,0,0));
                    nuevo->setModelMatrix(mModelado);
                    break;
                }
                case MODEL_TYPE::COW:{
                    std::cout<<"Dibujando Vaca"<<std::endl;

                    nuevo = new Model(model,"spot_texture.png");
                    float grados = -90.0;
                    float rotHorizontal = 180.0;
                    glm::mat4 mModelado = glm::rotate(nuevo->getModelMatrix(),glm::radians(grados), glm::vec3(1,0,0));
                    mModelado = glm::rotate(mModelado,glm::radians(rotHorizontal),glm::vec3(0,0,1));
                    nuevo->setModelMatrix(mModelado);
                    break;
                }
                case MODEL_TYPE::DICE:{
                    std::cout<<"Dibujando Dado"<<std::endl;
                    nuevo = new Model(model,"dado.png","NormalMap.png");
                    glm::mat4 mModelado = glm::translate(nuevo->getModelMatrix(),glm::vec3(1,0,0));
                    nuevo->setModelMatrix(mModelado);
                    break;
                }
                case MODEL_TYPE::CYLINDER:{
                    std::cout<<"Dibujando Cilindro"<<std::endl;
                    nuevo = new Model(model);
                    glm::mat4 mModelado = glm::translate(nuevo->getModelMatrix(),glm::vec3(1,-0.5,1));
                    nuevo->setModelMatrix(mModelado);
                    break;
                }

            }
        }catch(std::runtime_error e){
            std::string mensaje = e.what();
            throw std::runtime_error("PAG::Renderer::addModeloEscena->" + mensaje);
        }

        models.push_back(nuevo);
        applyConfiguration();
    }
}
/**
 * @brief Elimina un modelo de la escena si este existe
 * @param model Modelo a eliminar
 */
void PAG::Renderer::removeModel(MODEL_TYPE model) {
    bool exist = false;
    int position = -1;
    for(int i = 0; i < models.size() && !exist; i++){
        if(models[i]->getModel() == model){
            exist = true;
            position = i;
        }
    }

    if(exist) {
        models.erase(models.begin() + position);
    }
}

/***
 * @brief Función que dibuja el modelo
 * @param model Modelo a dibujar
 * @param indexNumber Numero de indices del modelo
 */
void PAG::Renderer::drawModel(int inxModel) {

    models[inxModel]->bindVAO_IBO();
    if(models[inxModel]->getModel() == MODEL_TYPE::CYLINDER){
        glDrawElements(GL_TRIANGLE_STRIP, models[inxModel]->getIndexNumber(), GL_UNSIGNED_INT, nullptr);
    }else {
        glDrawElements(GL_TRIANGLES, models[inxModel]->getIndexNumber(), GL_UNSIGNED_INT, nullptr);
    }

}

/***
 * @brief Función que realiza movimiento de camara
 * @param amount Cantidad
 * @param horizontal Si es en horizontal (true) o vertical (false)
 */
void PAG::Renderer::cameraMovement(float amount, bool horizontal) {
    switch (camara->getActiveMove()) {
        case MOVEMENT_TYPE::PAN: {
            if(horizontal) {

                if (amount < 0) {
                    camara->applyPanRotation(amount);
                } else {
                    camara->applyPanRotation(amount);
                }

            }
            break;
        }
        case MOVEMENT_TYPE::TILT: {
            if(!horizontal) {
                if (amount < 0) {
                    camara->applyTiltRotation(amount);
                } else {
                    camara->applyTiltRotation(amount);
                }
            }
            break;
        }
        case MOVEMENT_TYPE::DOLLY: {
            if(horizontal) {
                camara->applyDollyCamera(-amount / 10,0);
            }else{
                camara->applyDollyCamera(0, -amount / 10);
            }
            break;
        }
        case MOVEMENT_TYPE::CRANE: {
            if(!horizontal) {
                camara->applyCraneCamera(amount/10);
            }
            break;
        }
        case MOVEMENT_TYPE::ORBIT: {
            if(horizontal){
                camara->applyOrbitRotation(-amount,0);
            }else{
                camara->applyOrbitRotation(0,-amount);
            }
            break;
        }
        case MOVEMENT_TYPE::ZOOM: {
            if(!horizontal){
                camara->applyZoom(-(amount/10));
            }
            break;
        }
        case MOVEMENT_TYPE::NONE:{
            break;
        }
    }
}
/***
 * @brief Función que cambia el tipo de movimiento activo de la camara
 * @param movement Movimiento que se activa
 */
void PAG::Renderer::setCameraMovement(MOVEMENT_TYPE movement) {
    camara->setActiveMove(movement);
}

/***
 * @brief Función que aplica la configuracion de los modelos que esten en la escena a los nuevo
 * (textura activa,normalmapping, modo alambre)
 */
void PAG::Renderer::applyConfiguration() {
    bool wire = false;
    bool texture = false;
    bool normalMapping = false;

    for(int i = 0; i < models.size(); i++){
        if(models[i]->getWire()){
            wire = true;
            models[models.size()-1]->setWire(wire);
        }
        if(models[i]->getTexture()){
            texture = true;
            models[models.size()-1]->setTexture(texture);
        }
        if(models[i]->getNormalMapping()){
            normalMapping = true;
            models[models.size()-1]->setNormalMapping(normalMapping);
        }

        if(wire && texture && normalMapping){
            break;
        }
    }

}

/***
 * @brief Función que cambia a modo alambre los modelos
 */
void PAG::Renderer::wireMode() {

    for(int i = 0; i < models.size(); i++){
        if(models[i]->getWire()){
            models[i]->setWire(false);
        }else{
            models[i]->setWire(true);
        }
    }
}

/***
 * @brief Activa la textura de los modelos que la tengan
 */
void PAG::Renderer::textureMode() {

    for(int i = 0; i < models.size(); i++){
        if(models[i]->getTexture()){
            models[i]->setTexture(false);
        }else{

            models[i]->setTexture(true);
        }
    }
}

/***
 * @brief Funcion que activa desactiva el normal mapping
 */
void PAG::Renderer::normalMappingMode() {

    for(int i = 0; i < models.size(); i++){
        if(models[i]->getNormalMapping()){

            models[i]->setNormalMapping(false);
        }else{
            models[i]->setNormalMapping(true);
        }
    }

}


/**
 * Setters y Getters
 * */
double PAG::Renderer::getR() {
    return r;
}

double PAG::Renderer::getG() {
    return g;
}
double PAG::Renderer::getB() {
    return b;
}

void PAG::Renderer::setR(double value) {
    r = value;
}

void PAG::Renderer::setG(double value) {
    g = value;
}

void PAG::Renderer::setB(double value) {
    b = value;
}

