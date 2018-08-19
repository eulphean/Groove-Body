#version 150

uniform mat4 modelViewProjectionMatrix;
uniform samplerBuffer tex;

in vec4 position;
in vec4 color; 
in vec4 normal; 

out vec4 meshColor;
out vec3 vecNormal;
out vec4 vecPosition;

void main(){	 
    int x = gl_InstanceID*4;
    mat4 transformMatrix = mat4( 
        texelFetch(tex, x),
        texelFetch(tex, x+1),
        texelFetch(tex, x+2), 
        texelFetch(tex, x+3)
    );	

    vecPosition = modelViewProjectionMatrix * transformMatrix * position;
    vecNormal = normal.xyz;
    gl_Position = vecPosition;

     meshColor = color;
}