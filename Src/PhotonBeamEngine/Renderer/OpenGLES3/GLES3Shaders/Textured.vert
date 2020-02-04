static const char* TexturedVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec2 TextureCoord;

uniform mat4 ModelViewProjection;

varying vec2 TextureCoordOut;

void main(void) {
    gl_Position = ModelViewProjection * Position;
    TextureCoordOut = TextureCoord;
}

);
