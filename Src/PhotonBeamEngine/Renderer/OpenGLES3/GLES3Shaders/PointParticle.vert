static const char* PointParticleVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec4 Color;
attribute float PointSize;

uniform mat4 ModelViewProjection;

varying vec4 ColorOut;

void main(void) {
    gl_Position = ModelViewProjection * Position;
    gl_PointSize = PointSize;
    ColorOut = Color;
}

);
