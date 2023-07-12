#version 410

#define M_PI 3.1415926535897932384626433832795

//Entrada del vertex shader
in outVS{
    vec3 position;
    vec3 normal;
    vec2 coordTextura;
    vec3 lightPosition;
    vec3 lightDirection;
    vec4 shadowCoordinates;
} invs;

//Color modo alambre
uniform vec3 wireColor;
//Intensidades de la luz
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
//Color de la luz
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
//Parametros de la luz
uniform float ns;//Phong
uniform float spotLightAngle;//Angulo del foco
uniform float s;//Exponente atenuacion foco

//Sampler para textura y normal
uniform sampler2D TexSamplerColor;
uniform sampler2D TexNormalSampler;
uniform sampler2DShadow ShadowSampler;

layout (location = 0) out vec4 fragmentColor;

//Subrutina para el color ambiente o textura
subroutine vec3 calculateAmbColor ();
subroutine uniform calculateAmbColor selectAmbientColor;

//Color de textura ambiente
subroutine( calculateAmbColor )
vec3 textureColor () {
    vec4 textColor = texture(TexSamplerColor,invs.coordTextura);
    return vec3(textColor.rgb);
}
//Color de material ambiente
subroutine( calculateAmbColor )
vec3 materialColor () {
    return Ka;

}

//Subrutina que permite devolver la normal del modelo o del mapa de normales en caso de tener
subroutine vec3 calculateNormal();
subroutine uniform calculateNormal selectNormal;
//Devuelve las normales del mapa convirtiendo el color de la textura en componente normal
subroutine(calculateNormal)
vec3 normalNormalMap(){
    vec4 textNormal = texture(TexNormalSampler,invs.coordTextura);
    vec3 normalRGB = textNormal.rgb;
    normalRGB = 2.0 * (normalRGB - 0.5);
    return normalRGB;
}
//Devuelve la normal del modelo
subroutine(calculateNormal)
vec3 normalModel(){
    return invs.normal;
}

//Subrutina para color difuso o textura
subroutine vec3 calculateDifColor ();
subroutine uniform calculateDifColor selectDiffuseColor;
//Color de textura para luz difusa
subroutine( calculateDifColor )
vec3 textureColorDif () {

    vec4 textColor = texture(TexSamplerColor,invs.coordTextura);
    return vec3(textColor.rgb);

}
//Color de material difuso
subroutine( calculateDifColor )
vec3 materialColorDif () {
    return Kd;

}


//Iluminacion
subroutine vec3 ilumination( vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM);
subroutine uniform ilumination lightType;
//Color en modo alambre
subroutine (ilumination)
vec3 wireMode( vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM){
    return wireColor;
}
//Iluminacion ambiente
subroutine (ilumination)
vec3 ambientLight( vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM){
    vec3 ambient = (Ia * ambientColor);
    return ambient;

}
//Iluminacion puntual
subroutine (ilumination)
vec3 pointLight( vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM){

    vec3 n = normalize(normals);
    vec3 l = normalize(invs.lightPosition - invs.position);
    vec3 v = normalize(-invs.position);
    vec3 r = reflect(-l,n);

    vec3 diffuse = (Id * diffuseColor * max(dot(l,n),0.0));
    vec3 specular;
    specular = (Is * Ks * pow(max(dot(r,v),0.0),ns));

    vec3 puntual = diffuse + specular;

    return puntual;

}
//Iluminacion Direccional
subroutine (ilumination)
vec3 directionalLight(  vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM){

    vec3 n = normalize(normals);
    vec3 l = -invs.lightDirection;
    vec3 v = normalize(-invs.position);
    vec3 r = reflect(-l,n);

    vec3 diffuse = (Id * diffuseColor * max(dot(l,n),0.0));
    vec3 specular;
    specular = (Is * Ks * pow(max(dot(r,v),0.0),ns));

    return  shadowM * (diffuse + specular);

}
//Iluminacion foco
subroutine (ilumination)
vec3 spotLight( vec3 diffuseColor, vec3 ambientColor,vec3 normals, float shadowM){

    vec3 l = normalize(invs.lightPosition - invs.position);
    vec3 d = invs.lightDirection;

    float cosGamma = cos(spotLightAngle);

    float spot = pow(dot(-l,d),s);

    if(cos(dot(-l,d)) < cosGamma){
        spot = 0.0;
    }

    vec3 n = normalize(normals);
    vec3 v = normalize(-invs.position);
    vec3 r = reflect(-l,n);

    vec3 diffuse = (Id * diffuseColor * max(dot(l,n),0.0));
    vec3 specular;
    specular = (Is * Ks * pow(max(dot(r,v),0.0),ns));

    return shadowM * spot * (diffuse + specular);

}
//Funcion para calcular el factor sombra
float shadowFactor(){
    //Se extrae del mapa de sombras con las coordenadas de sombra y deuelve 0.0 o 1.0 si hay iluminacion o no
    //se realiza la sumatoria para el antialiasing de la sombra ya que tiene efecto sierra
    float sum = 0;
    sum += textureProjOffset(ShadowSampler, invs.shadowCoordinates, ivec2(-1,-1));
    sum += textureProjOffset(ShadowSampler, invs.shadowCoordinates, ivec2(-1,1));
    sum += textureProjOffset(ShadowSampler, invs.shadowCoordinates, ivec2(1,1));
    sum += textureProjOffset(ShadowSampler, invs.shadowCoordinates, ivec2(1,-1));
    sum += textureProjOffset(ShadowSampler, invs.shadowCoordinates, ivec2(0,0));

    float shadowFactor = sum * (1.0/5.0);
    //Se aproxima entre 0.2 y 1.0
    shadowFactor = clamp(shadowFactor, 0.2, 1.0);

    return shadowFactor;
}



void main ()
{

    fragmentColor = vec4(lightType(selectDiffuseColor(),selectAmbientColor(),selectNormal(),shadowFactor()),1.0);

}