//
//  main.cpp
//  pag
//
//  Created by Jose Antonio on 14/9/21.
//

#include <iostream>
#include <cstdlib>
// IMPORTANTE: El include de GLEW debe estar siempre ANTES de el de GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>
#include "Renderer.h"

static bool botonPress = false;
static bool updateValues = false;
static double posX = 0;
static double posY = 0;

double mayor(double a, double b){

    if (a > b) {
        return a;
    }else{
        return b;
    }

}


//LLaman a la funcion de la instancia que haga falta
void callbackRefresh(GLFWwindow* window){
    int w,h;
    glfwGetFramebufferSize(window,&w,&h);
    PAG::Renderer::getInstance()->refresh_Window(w,h);
    glfwSwapBuffers(window);
}

void callbackResizeWindow(GLFWwindow* window,int width, int heigth){
    int w,h;
    glfwGetFramebufferSize(window,&w,&h);
    PAG::Renderer::getInstance()->size_Window(w, h);
}

void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods){
    //Se comprueba el tiempo para no sobrecargar


        switch (key) {

            case GLFW_KEY_Q:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->addModel(PAG::MODEL_TYPE::TRIANGLE);
                }
                break;
            }
            case GLFW_KEY_W:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->addModel(PAG::MODEL_TYPE::TETRAHEDRON);
                }
                break;
            }
            case GLFW_KEY_A:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->addModel(PAG::MODEL_TYPE::COW);
                }
                break;
            }
            case GLFW_KEY_S:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->addModel(PAG::MODEL_TYPE::DICE);
                }
                break;
            }
            case GLFW_KEY_X:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->addModel(PAG::MODEL_TYPE::CYLINDER);
                }
                break;
            }
            case GLFW_KEY_E:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->removeModel(PAG::MODEL_TYPE::TRIANGLE);
                }
                break;
            }
            case GLFW_KEY_R:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->removeModel(PAG::MODEL_TYPE::TETRAHEDRON);
                }
                break;
            }
            case GLFW_KEY_F:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->removeModel(PAG::MODEL_TYPE::COW);
                }
                break;
            }
            case GLFW_KEY_G:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->removeModel(PAG::MODEL_TYPE::DICE);
                }
                break;
            }
            case GLFW_KEY_H:{
                if(action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->removeModel(PAG::MODEL_TYPE::CYLINDER);
                }
                break;
            }
            case GLFW_KEY_P:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Pan"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::PAN);
                }
                break;
            }

            case GLFW_KEY_T:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Tilt"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::TILT);
                }
                break;
            }

            case GLFW_KEY_D:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Dolly"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::DOLLY);
                }
                break;
            }

            case GLFW_KEY_C:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Crane"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::CRANE);
                }
                break;
            }

            case GLFW_KEY_O:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Orbit"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::ORBIT);
                }
                break;
            }

            case GLFW_KEY_Z:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activado movimiento Zoom"<<std::endl;
                    PAG::Renderer::getInstance()->setCameraMovement(PAG::MOVEMENT_TYPE::ZOOM);
                }
                break;
            }
            case GLFW_KEY_L:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activando/Desactivando modo alambre"<<std::endl;
                    PAG::Renderer::getInstance()->wireMode();
                }
                break;
            }

            case GLFW_KEY_K:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activando/Desactivando texturas"<<std::endl;
                    PAG::Renderer::getInstance()->textureMode();
                }
                break;
            }

            case GLFW_KEY_J:{
                if(action == GLFW_PRESS) {
                    std::cout<<"Activando/Desactivando normal mapping"<<std::endl;
                    PAG::Renderer::getInstance()->normalMappingMode();
                }
                break;
            }
            case GLFW_KEY_I:{
                if (action == GLFW_PRESS) {
                    std::cout<<"Teclas: "<<std::endl;
                    std::cout<<"Tecla q --> Modelo triangulo "<<std::endl;
                    std::cout<<"Tecla w --> Modelo tetraedro "<<std::endl;
                    std::cout<<"Tecla a --> Modelo vaca "<<std::endl;
                    std::cout<<"Tecla s --> Modelo dado "<<std::endl;
                    std::cout<<"Tecla x --> Modelo revolucion cilindro "<<std::endl;
                    std::cout<<"Tecla e --> Elimina modelo triangulo "<<std::endl;
                    std::cout<<"Tecla r --> Elimina modelo tetraedro "<<std::endl;
                    std::cout<<"Tecla f --> Elimina modelo vaca "<<std::endl;
                    std::cout<<"Tecla g --> Eliminar modelo dado "<<std::endl;
                    std::cout<<"Tecla h --> Eliminar modelo revolucion cilindro "<<std::endl;
                    std::cout<<"Tecla p --> Pan "<<std::endl;
                    std::cout<<"Tecla t --> Tilt "<<std::endl;
                    std::cout<<"Tecla d --> Dolly "<<std::endl;
                    std::cout<<"Tecla c --> Crane "<<std::endl;
                    std::cout<<"Tecla o --> Orbit "<<std::endl;
                    std::cout<<"Tecla z --> Zoom "<<std::endl;
                    std::cout<<"Tecla l --> Modo Alambre "<<std::endl;
                    std::cout<<"Tecla k --> Modo Textura "<<std::endl;
                    std::cout<<"Tecla j --> Modo Normal mapping "<<std::endl;
                    std::cout<<"Tecla i --> Info "<<std::endl;

                }
                break;
            }
            case GLFW_KEY_RIGHT: {
                if (action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->cameraMovement(-5, true);
                } else if (action == GLFW_REPEAT) {
                    PAG::Renderer::getInstance()->cameraMovement(-5, true);
                }
                break;
            }
            case GLFW_KEY_LEFT: {
                if (action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->cameraMovement(5, true);
                } else if (action == GLFW_REPEAT) {
                    PAG::Renderer::getInstance()->cameraMovement(5, true);
                }
                break;
            }
            case GLFW_KEY_UP: {
                if (action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->cameraMovement(5, false);
                } else if (action == GLFW_REPEAT) {
                    PAG::Renderer::getInstance()->cameraMovement(5, false);
                }
                break;
            }
            case GLFW_KEY_DOWN: {
                if (action == GLFW_PRESS) {
                    PAG::Renderer::getInstance()->cameraMovement(-5, false);
                } else if (action == GLFW_REPEAT) {
                    PAG::Renderer::getInstance()->cameraMovement(-5, false);
                }
                break;
            }
            case GLFW_KEY_ESCAPE:
                exit(0);
                break;
        }


        //Refresco de la ventana
        callbackRefresh(window);

}

void callbackMouseMovement(GLFWwindow* window, double xpos,double ypos){

    if (botonPress && !updateValues) {
        posX = xpos;
        posY = ypos;
        updateValues = true;

    }else if(botonPress && updateValues){
        double diferenceX = xpos - posX;
        double diferenceY = ypos - posY;



        PAG::Renderer::getInstance()->cameraMovement(float(diferenceX/10), true);
        PAG::Renderer::getInstance()->cameraMovement(float(diferenceY/10), false);

        posX = xpos;
        posY = ypos;


    }else if(!botonPress){
        updateValues = false;
    }

    callbackRefresh(window);



}

void callbackMouseButton(GLFWwindow* window, int button,int action,int mods){

    if ( action == GLFW_PRESS ){
        botonPress = true;
    }else if ( action == GLFW_RELEASE ){
        botonPress = false;
    }
}

void callbackScrollButton(GLFWwindow* window, double xoffset, double yoffset){

    //Se crean las variables de color
    double r = PAG::Renderer::getInstance()->getR();
    double g = PAG::Renderer::getInstance()->getG();
    double b = PAG::Renderer::getInstance()->getB();

    //Aqui se ha modificado como me dijiste para que se multiplique cada el movimiento por un aleatorio entre 0 y 1 y se sume para cambiar el color
    int FLOAT_MIN = 0;
    int FLOAT_MAX = 1;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(FLOAT_MIN, FLOAT_MAX);

    //Se suman el movimiento
    r = r + (yoffset*distr(eng));
    g = g + (yoffset*distr(eng));
    b = b + (yoffset*distr(eng));

    //se calcula el nuevo valor si se pasa
    if (r > 1.0 || g > 1.0 || b > 1.0) {
        double div = mayor(mayor(r, g),b);
        r = r / div;
        g = g / div;
        b = b / div;

    }


    //Se pone a 0 si algun color baja de este
    if(r < 0.0){
        r = 0.0;
    }else if(g < 0.0){
        g = 0.0;
    }else if(b < 0.0){
        b = 0.0;
    }

    //Aplicar color y refrescar ventana
    PAG::Renderer::getInstance()->setR(r);
    PAG::Renderer::getInstance()->setG(g);
    PAG::Renderer::getInstance()->setB(b);
    PAG::Renderer::getInstance()->colorApply();
    callbackRefresh(window);


}

int main(){


        std::cout << "Starting application pag prueba 01" << std::endl;
    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
        if ( glfwInit () != GLFW_TRUE ){

            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;

        }


    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
        glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
        /*glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
        glfwWindowHint ( GLFW_VERSION_MAJOR, 4 ); // siguientes activan un contexto
        glfwWindowHint ( GLFW_VERSION_MINOR, 1 ); // OpenGL Core Profile 4.1.*/

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
        GLFWwindow *window;

    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.576
        window = glfwCreateWindow ( 1024, 820, "Pag practice", nullptr, nullptr );

    // - Comprobamos si la creación de la ventana ha tenido éxito.
        if ( window == nullptr ){
            std::cerr << "Failed to open GLFW window" << std::endl;
            glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
            return -2;
        }

        // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
        glfwMakeContextCurrent ( window );

    // - Ahora inicializamos GLEW.
    // IMPORTANTE: GLEW debe inicializarse siempre DESPUÉS de que se haya
    // inicializado GLFW y creado la ventana
        glewExperimental = true;
        if ( glewInit () != GLEW_OK ){

            std::cerr << "Failed to initialize GLEW" << std::endl;
            glfwDestroyWindow ( window );
            glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
            return -3;
        }

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
        std::cout << glGetString ( GL_RENDERER ) << std::endl
                  << glGetString ( GL_VENDOR ) << std::endl
                  << glGetString ( GL_VERSION ) << std::endl
                  << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;


        std::cout<<"Tecla i --> Info teclas"<<std::endl;



    try {
        // - Registramos los callbacks que responderán a los eventos principales
        glfwSetWindowRefreshCallback(window, callbackRefresh);
        glfwSetFramebufferSizeCallback(window, callbackResizeWindow);
        glfwSetKeyCallback(window, callbackKey);
        glfwSetMouseButtonCallback(window, callbackMouseButton);
        glfwSetScrollCallback(window, callbackScrollButton);
        glfwSetCursorPosCallback(window,callbackMouseMovement);


        //Se carga la profundidad y color inical
        PAG::Renderer::getInstance()->loadParameterGL();


    
    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
        while ( !glfwWindowShouldClose ( window ) ){

            // - Borra los buffers (color y profundidad)
            //glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (en el que se ha estado dibujando) por el
    // que se mostraba hasta ahora (front).
            //glfwSwapBuffers ( window );


    // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
    // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
    // de eventos y después de glfwSwapBuffers(window);
            glfwPollEvents ();
        }

    }catch(std::runtime_error &dato){
        std::cout<<"Main -> "<<dato.what()<<std::endl;
    }

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.


}
