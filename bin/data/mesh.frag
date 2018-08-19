#version 150

uniform vec3 uLightPosition; 
uniform vec4 uMaterialColor; 
uniform float uOpacity; 

in vec3 vecNormal; 
in vec4 vecPosition; 
out vec4 outputColor;

void main(){
    vec3 lightDirection = normalize(vecPosition.xyz - uLightPosition);  
    float dProd = max(0.0, dot(vecNormal, lightDirection));

    //color
    vec4 color = uMaterialColor;
    // vec4 col = vec4( vec3( dProd ) * vec3( color), uOpacity );
    outputColor = vec4(color.rgb, uOpacity);
}