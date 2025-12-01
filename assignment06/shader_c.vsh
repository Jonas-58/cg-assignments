#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

out vec4 vPosition;
out vec3 vNormal;

void main() {
    // Transform to eye-space
    vPosition = uModelViewMatrix * aPosition;
    vNormal = normalize(vec3(inverse(transpose(uModelViewMatrix)) * vec4(aNormal, 0.0)));
    gl_Position = uProjectionMatrix * vPosition;
}
