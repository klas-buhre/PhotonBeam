static const char* TexturedFragmentShader = STRINGIFY(

varying mediump vec2 TextureCoordOut;

uniform mediump float Opacity;
uniform sampler2D Sampler;

void main(void) {
    mediump vec4 texel = texture2D(Sampler, TextureCoordOut);
    gl_FragColor = vec4(texel.xyz, texel.w * Opacity);
}

);
