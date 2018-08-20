#version 150

uniform vec3 uLightPosition; 
uniform vec4 uMaterialColor; 
uniform sampler2D coinTex;

in vec3 vecNormal; 
in vec4 vecPosition; 
in vec2 texCoordVarying;

out vec4 outputColor;

void main(){
    // vec3 lightDirection = normalize(vecPosition.xyz - uLightPosition);  
    // float dProd = max(0.0, dot(vecNormal, lightDirection));

    // // Color calculations.
    // vec4 color = texture(coinTex, texCoordVarying);
    // vec4 col = vec4( vec3( dProd ) * vec3( color), 1.0 );
    //outputColor = meshColor/1.5 + col * 2;  
    // Integrate lighting. 
    outputColor = texture(coinTex, texCoordVarying);
}