static const char* VertexColorVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec4 Color;

uniform mat4 ModelViewProjection;

varying vec4 ColorOut;

void main(void) {
    gl_Position = ModelViewProjection * Position;
    ColorOut = Color;
}

);
