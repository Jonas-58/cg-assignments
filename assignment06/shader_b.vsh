#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightSpotLightFactor;

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
    mat3 normalMatrix = transpose(inverse(mat3(uModelViewMatrix)));
    vec3 newNormal = normalize(normalMatrix * aNormal);
    
    //write the final vertex position into this variable
    gl_Position = uProjectionMatrix * newPosition;

    // Gouraud
    // ambient
    vec3 ambient = uAmbientMaterial * uLightColor;
    
    // diffuse
    vec3 lightDirection = normalize(uLightPosition - vec3(newPosition));
    float cosAngle = max(dot(newNormal, lightDirection), 0);
    
    // spotlight
    vec3 spotPosition = vec3(uModelViewMatrix * vec4(0, 0, 0, 1));
    vec3 spotDirection = normalize(spotPosition - uLightPosition);
    float cosAngleSpot = max(dot(spotDirection, - lightDirection), 0);
    
    float spot = pow(cosAngleSpot, uLightSpotLightFactor);
    
    vec3 diffuse = uDiffuseMaterial * uLightColor * cosAngle;
    
    // specular
    vec3 viewDirection = normalize(-vec3(newPosition));
    vec3 h = normalize(lightDirection + viewDirection);
    float cosAngleSp = max(dot(newNormal, h), 0);
    
    vec3 specular = uSpecularMaterial * uLightColor * pow(cosAngleSp, uSpecularityExponent);
        
    // Define the color of this vertex
    vColor = ambient + spot*diffuse + spot*specular;
}
