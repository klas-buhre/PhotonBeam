static const char* VertexColorFragmentShader = STRINGIFY(

varying highp vec4 ColorOut;

uniform mediump float Opacity;

void main(void) {
    gl_FragColor = vec4(ColorOut.x, ColorOut.y, ColorOut.z, ColorOut.w * Opacity);
}

);
