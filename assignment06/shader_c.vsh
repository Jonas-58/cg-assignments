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

    // Transform normals using the correct matrix.
    // It's important to normalize in the end
    mat3 normalMatrix = transpose(inverse(mat3(uModelViewMatrix)));
    vNormal = normalize(normalMatrix * aNormal);
    
    //write the final vertex position into this variable
    gl_Position = uProjectionMatrix * vPosition;
}
