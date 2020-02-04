static const char* TextDoubleGradientVertexShader = STRINGIFY(

attribute vec4 TextCoords;
attribute float TextGradientFunction;

uniform mat4 Projection;

varying vec2 TextureCoordOut;
varying float TextGradientFunctionOut;

void main(void) {
    gl_Position = Projection * vec4(TextCoords.xy, 0.0, 1.0);
    TextureCoordOut = TextCoords.zw;
    TextGradientFunctionOut = TextGradientFunction;
}
    
);
