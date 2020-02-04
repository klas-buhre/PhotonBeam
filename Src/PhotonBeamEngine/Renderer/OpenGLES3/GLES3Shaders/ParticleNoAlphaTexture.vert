static const char* ParticleNoAlphaTextureVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec2 TextureCoord;
attribute vec4 Color;

uniform mat4 ModelViewProjection;

varying vec2 TextureCoordOut;
varying vec4 ColorOut;

void main(void) {
    gl_Position = ModelViewProjection * Position;
    TextureCoordOut = TextureCoord;
    ColorOut = Color;
}

);
