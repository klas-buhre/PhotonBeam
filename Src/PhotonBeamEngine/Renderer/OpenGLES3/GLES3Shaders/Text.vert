static const char* TextVertexShader = STRINGIFY(

attribute vec4 TextCoords;
attribute float TextGradientFunction;

uniform mat4 Projection;

varying vec2 TextureCoordOut;

void main(void) {
    gl_Position = Projection * vec4(TextCoords.xy, 0.0, 1.0);
    TextureCoordOut = TextCoords.zw;
}
    
);
