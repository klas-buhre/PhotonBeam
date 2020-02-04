static const char* PixelLightingVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;

uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;

varying vec3 NormalCameraSpace;

void main(void) {
    NormalCameraSpace = normalize(NormalMatrix * Normal);
    gl_Position = ModelViewProjection * Position;
}

);
