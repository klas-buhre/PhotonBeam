static const char* PointParticleFragmentShader = STRINGIFY(

uniform sampler2D Sampler;

varying highp vec4 ColorOut;

void main(void) {
    gl_FragColor = vec4(ColorOut.xyz, ColorOut.w * texture2D(Sampler, gl_PointCoord).a);
}

);
