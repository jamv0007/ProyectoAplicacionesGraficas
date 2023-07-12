//
// Created by Jose Antonio on 23/12/21.
//

#include "Shader.h"

/**
 * @brief Constructor del shader
 * @param vsFile Nombre archivo Vertex shader
 * @param fsFile Nombre archivo Fragment shader
 */
PAG::Shader::Shader(std::string vsFile, std::string fsFile) {
    this->vertexShaderFile = vsFile;
    this->fragmentShaderFile = fsFile;

    //Se cargan los shader
    try {
        loadShaders();
        loadShaderProgram();
    }catch (std::runtime_error &dato){
        std::string contenido = dato.what();
        throw std::runtime_error("PAG::Shader -> " + contenido);
    }
}

/**
 * @brief Destructor de la clase shader
 */
PAG::Shader::~Shader() {
    //Eliminar los recursos de los objects

    //Se borran el fragment shader y el vertex shader
    glDeleteShader(idFS);
    glDeleteShader(idVS);

    //Se elimina el programa
    glDeleteProgram(idSP);
}

/**
 * @brief Constructor copia de shader
 * @param orig Shader a copiar
 */
PAG::Shader::Shader(const Shader &orig) {

    idVS = orig.idVS;
    idFS = orig.idFS;
    idSP = orig.idSP;

    vertexShaderFile = orig.vertexShaderFile;
    fragmentShaderFile = orig.fragmentShaderFile;

    vertexShader = orig.vertexShader;
    fragmentShader = orig.fragmentShader;
}

/**
 * @brief Carga los shaders de los ficheros
 */
void PAG::Shader::loadShaders() {

    std::ifstream vsFile;
    std::ifstream fsFile;
    std::string line;

    vsFile.open(vertexShaderFile,std::ios::in);

    if(!vsFile.fail()){
        while(!vsFile.eof()){
            std::getline(vsFile,line);
            vertexShader = vertexShader + line + "\n";
        }

    }else{
        throw std::runtime_error("cargarShaders -> Fallo al abrir el archivo " + vertexShaderFile);
    }

    vsFile.close();

    fsFile.open(fragmentShaderFile,std::ios::in);

    if(!fsFile.fail()) {
        while (!fsFile.eof()) {
            std::getline(fsFile, line);
            fragmentShader = fragmentShader + line + "\n";
        }
    }else{
        throw std::runtime_error("cargarShaders -> Fallo al abrir el archivo " + fragmentShaderFile);
    }


    fsFile.close();
}

/**
 * @brief compila los shaders y genera el shader program
 */
void PAG::Shader::loadShaderProgram() {
    if(vertexShader == "" || fragmentShader == ""){
        throw std::runtime_error("creaShaderProgram -> No se han podido cargar los shaders");
    }

    //Se crea el vertex shader y se compila
    idVS = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* fuenteVS = vertexShader.c_str();
    glShaderSource( idVS, 1, &fuenteVS, nullptr );
    glCompileShader( idVS );

    try {
        compilationShaderError(idVS);
    }catch(std::runtime_error &dato){
        std::string contenido = dato.what();
        throw std::runtime_error("creaShaderProgram -> VertexShader -> " + contenido);
    }

    //Se crean los fragments shaders y se compilan
    idFS = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fuenteFS = fragmentShader.c_str();
    glShaderSource( idFS, 1, &fuenteFS, nullptr );
    glCompileShader( idFS );

    try{
        compilationShaderError(idFS);
    }catch(std::runtime_error &dato){
        std::string contenido = dato.what();
        throw std::runtime_error("creaShaderProgram -> FragmentShader -> " + contenido);
    }

    //Se crea el programa y se asignan los shadders al identificador
    idSP = glCreateProgram();
    glAttachShader(idSP,idVS);
    glAttachShader(idSP,idFS);
    glLinkProgram(idSP);

    //Se comprueba que tras ejecutar el programa hay algun  fallo con los shaders en openGL
    shadersError();
}

/**
 * @brief Activa el shader program de este shader
 */
void PAG::Shader::activeShaderProgram() {
    glUseProgram(idSP);
}

/**
 * @brief Comprueba fallos en los shaders tras copilarlos
 */
void PAG::Shader::shadersError() {
    GLint resultadoEnlazado = 0;
    glGetProgramiv(idSP,GL_LINK_STATUS,&resultadoEnlazado);

    if(resultadoEnlazado == GL_FALSE){

        GLint tamMsj = 0;
        std::string mensaje = "";
        glGetProgramiv(idSP,GL_INFO_LOG_LENGTH,&tamMsj);

        if(tamMsj > 0){

            GLchar* mensajeFormatoC = new GLchar[tamMsj];
            GLint datosEscritos = 0;

            glGetProgramInfoLog(idSP,tamMsj,&datosEscritos,mensajeFormatoC);
            mensaje.assign(mensajeFormatoC);

            delete[] mensajeFormatoC;
            mensajeFormatoC = nullptr;

            if(mensaje != ""){
                throw std::runtime_error("Error de los shaders -> " + mensaje);
            }

        }
    }
}

/**
 * @brief Compila el shader
 * @param idShader shader a compilar
 */
void PAG::Shader::compilationShaderError(GLuint idShader) {
    GLint resultadoCompilacion;
    glGetShaderiv ( idShader, GL_COMPILE_STATUS, &resultadoCompilacion );

    if ( resultadoCompilacion == GL_FALSE )
    {
        // Ha habido un error en la compilación.
        // Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL
        GLint tamMsj = 0;
        std::string mensaje = "";
        glGetShaderiv ( idShader, GL_INFO_LOG_LENGTH, &tamMsj );
        if ( tamMsj > 0 )
        {
            GLchar* mensajeFormatoC = new GLchar[tamMsj];
            GLint datosEscritos = 0;
            glGetShaderInfoLog ( idShader, tamMsj, &datosEscritos, mensajeFormatoC );
            mensaje.assign ( mensajeFormatoC );
            delete[] mensajeFormatoC;
            mensajeFormatoC = nullptr;

            if(mensaje != ""){
                throw std::runtime_error("Error compilacion shader -> " + mensaje);
            }
        }
    }
}

/***
 * Getters
 */
GLuint PAG::Shader::getIdSp(){
    return idSP;
}




