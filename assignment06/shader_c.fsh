#version 150

in vec4 vPosition;
in vec3 vNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightSpotLightFactor;

uniform mat3 uAmbientMaterial;
uniform mat3 uDiffuseMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

out vec4 oFragColor; //write the final color into this variable

void main() {
    vec3 newNormal = normalize(vNormal); //counteract interpolation

    vec3 L = normalize(uLightPosition - vec3(vPosition));
    vec3 V = normalize(vec3(0,0,0) - vec3(vPosition));

    vec3 aAmbientTerm =  vec3(0.0, 0.0, 0.0);
    vec3 aDiffuseTerm =  vec3(0.0, 0.0, 0.0);
    vec3 aSpecularTerm = vec3(0.0, 0.0, 0.0);

    // ambient
    aAmbientTerm = uAmbientMaterial * uLightColor;

    // if light on backside we can return with ambient only
    float NdotL = dot(newNormal,L); //counteract 
    if(NdotL <= 0.0) {
        oFragColor = vec4(aAmbientTerm, 1.0);
        return;
    }

    vec4 globalOrigin = uModelViewMatrix * vec4(0,0,0,1.0);
    vec3 D = normalize(vec3(globalOrigin) - uLightPosition);

    // if spotlight on backside we can return
    float DdotL = dot(D,-L);
    if(DdotL <= 0.0) {
        oFragColor = vec4(aAmbientTerm, 1.0);
        return;
    }
    float spot = pow(DdotL, uLightSpotLightFactor);

    // diffuse
    aDiffuseTerm = uDiffuseMaterial * uLightColor * NdotL;

    // specular
    vec3 R = 2.0 * dot(newNormal,L) * newNormal - L;
    float RdotV = max(dot(R,V), 0.0);
    aSpecularTerm = uSpecularMaterial * uLightColor * pow(RdotV, uSpecularityExponent);

    vec3 aColor = aAmbientTerm + spot * (aDiffuseTerm + aSpecularTerm);
    oFragColor = vec4(aColor, 1.0);
}
