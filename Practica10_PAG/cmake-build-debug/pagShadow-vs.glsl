#version 410

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normales;
layout (location = 2) in vec2 coordText;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 MVPmatrix;

void main(){

    gl_Position = MVPmatrix * vec4(vertex,1.0);
}
