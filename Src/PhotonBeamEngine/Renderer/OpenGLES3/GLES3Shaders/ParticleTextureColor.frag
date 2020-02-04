static const char* ParticleTextureColorFragmentShader = STRINGIFY(

varying mediump vec2 TextureCoordOut;
varying highp vec4 ColorOut;

uniform sampler2D Sampler;

void main(void) {
    gl_FragColor = vec4(ColorOut * texture2D(Sampler, TextureCoordOut));
}

);
