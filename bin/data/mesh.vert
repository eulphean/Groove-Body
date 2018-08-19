#version 150

uniform mat4 modelViewProjectionMatrix;
uniform float uNormalScale;

in vec4 position;
in vec4 normal; 

out vec3 vecNormal;
out vec4 vecPosition;

void main(){	 
    vecPosition = modelViewProjectionMatrix * position;
    vecNormal = uNormalScale * normal.xyz;
    gl_Position = vecPosition;
}