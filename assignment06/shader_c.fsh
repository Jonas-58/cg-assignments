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
    vec3 N = normalize(vNormal);

	// ambient
	vec3 ambient = uAmbientMaterial * uLightColor;

	// diffuse
    vec3 lightDirection = normalize(uLightPosition - vec3(vPosition));
    float cosAngle = max(dot(N, lightDirection), 0);
    
    // spotlight
    vec3 spotFocusPosition = vec3(uModelViewMatrix * vec4(0, 0, 0, 1));
    vec3 spotDirection = normalize(spotFocusPosition - uLightPosition);
    float cosAngleSpot = max(dot(spotDirection, -lightDirection), 0);
    
    float spot = pow(cosAngleSpot, uLightSpotLightFactor);
    
    vec3 diffuse = uDiffuseMaterial * uLightColor * cosAngle;
    
    // specular
    vec3 viewDirection = normalize(vec3(0, 0, 0) - vec3(vPosition));
    vec3 r = 2.0 * N * dot(N, lightDirection) - lightDirection;
    float rvl = max(dot(r, viewDirection), 0.0);
    
    vec3 specular = uSpecularMaterial * uLightColor * pow(rvl, uSpecularityExponent);

	// color
	vec3 color = ambient + spot * (diffuse + specular);

	// final color
	oFragColor = vec4(color, 1);
}
