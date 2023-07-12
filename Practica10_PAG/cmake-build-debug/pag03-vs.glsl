#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normales;
layout (location = 2) in vec2 coordText;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

//Salida del Vertex shader al fragment shader
out outVS{
  vec3 position;//Posicion
  vec3 normal;//Normales
  vec2 coordTextura;//Coordenadas de textura
  vec3 lightPosition;//Posicion de la luz
  vec3 lightDirection;//Direccion de la luz
  vec4 shadowCoordinates;//Coordenadas de sombras
} outvs;

//Uniforms
uniform mat4 MVPmatrix;//Matriz Modelado-Vision-Proyeccion
uniform mat4 MVmatrix;//Matriz Modelado-Vision
uniform mat4 MVitMatrix;//Matriz Modelado-Vision inversa traspuesta
uniform vec3 lightPos;//Posición de la luz
uniform vec3 lightDir;//Dirección de la luz
uniform mat4 shadowMVPMatrix;//Matriz de sombras


//Subrutina que pasa al fragment los parametros de posicion e iluminanacion(poscion y direccion)
//de forma normal o en el espacio de la tangente
subroutine void tangentSpace();
subroutine uniform tangentSpace activeTBN;

//Pasa de forma normal los parametros al fragment
subroutine(tangentSpace)
void withoutTBN(){
  outvs.lightDirection = lightDir;
  outvs.lightPosition = lightPos;
  outvs.position = vec3(MVmatrix * vec4(posicion, 1.0));
}

//Calcula la matriz TBN (Espacio de la tangente) y multiplica los paramentros ppor ella
subroutine(tangentSpace)
void withTBN(){
  //Matriz TBN de tangente, bitangente y normal
  vec3 normalMV = normalize(vec3(MVitMatrix * vec4(normales,0.0)));
  vec3 tangentMV = normalize(vec3(MVitMatrix * vec4(tangent,0.0)));
  vec3 bitangentMV = normalize(vec3(MVitMatrix * vec4(bitangent,0.0)));
  //Se traspone ya que es lo mismo que la inversa pero menos costosa
  mat3 TBN = transpose(mat3(tangentMV,bitangentMV,normalMV));
  //Se aplica a los parametros
  outvs.lightDirection = TBN * lightDir;
  outvs.lightPosition = TBN * lightPos;
  outvs.position = TBN * vec3(MVmatrix * vec4(posicion, 1.0));

}


void main ()
{
  //Activamos la subrutina
  activeTBN();
  //Las coordenadas de textura y normal se pasan igual
  outvs.coordTextura = coordText;
  outvs.normal = vec3(MVitMatrix * vec4(normales,0.0));
  //Se calculan las coordenadas de sombra multiplicando la matriz de sombra por la posicion
  outvs.shadowCoordinates = shadowMVPMatrix * vec4 ( posicion, 1.0 );
  gl_Position = MVPmatrix * vec4 ( posicion, 1.0 );
}