#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform mat3 uAmbientMaterial;
uniform mat3 uDiffuseMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

out vec3 vColor;

void main() {
    // Transform to eye-space
    vec4 newPosition = uModelViewMatrix * aPosition;

    // Transform normals using the correct matrix.
    // It's important to normalize in the end
    vec3 newNormal = normalize(vec3(inverse(transpose(uModelViewMatrix)) * vec4(aNormal, 0.0)));
    //write the final vertex position into this variable
    gl_Position =  uProjectionMatrix * newPosition;

    // lighting vectors
    vec3 L = normalize(uLightPosition - vec3(newPosition));
    vec3 V = normalize(vec3(0.0, 0.0, 0.0) - vec3(newPosition));
    vec3 H = normalize(L + V);

    vec3 aAmbientTerm =  vec3(0.0, 0.0, 0.0);
    vec3 aDiffuseTerm =  vec3(0.0, 0.0, 0.0);
    vec3 aSpecularTerm = vec3(0.0, 0.0, 0.0);

    // ambient
    aAmbientTerm = uAmbientMaterial * uLightColor;

    // if light on backside we can return with ambient only
    float NdotL = dot(newNormal,L);
    if(NdotL <= 0.0) {
        vColor = aAmbientTerm;
        return;
    }

    // diffuse
    aDiffuseTerm = uDiffuseMaterial * uLightColor * NdotL;

    // specular
    float NdotH = max(dot(newNormal, H), 0.0); //clamping if viewer on backside
    aSpecularTerm = uSpecularMaterial * uLightColor * pow(NdotH, uSpecularityExponent);
    
    // Define the color of this vertex
    vColor = aAmbientTerm + aDiffuseTerm + aSpecularTerm;
}
