static const char* TexturedPixelLightingVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;
attribute vec2 TextureCoord;

uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;

varying vec3 NormalCameraSpace;
varying vec2 TextureCoordOut;

void main(void) {
    NormalCameraSpace = normalize(NormalMatrix * Normal);
    gl_Position = ModelViewProjection * Position;
    TextureCoordOut = TextureCoord;
}

);
