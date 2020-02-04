static const char* TexturedLightingVertexColorFragmentShader = STRINGIFY(

varying lowp vec3 DestinationColor;
varying lowp vec3 SpecularColor;
varying mediump vec2 TextureCoordOut;

uniform mediump float Opacity;
uniform sampler2D Sampler;

void main(void) {
    mediump vec4 texel = texture2D(Sampler, TextureCoordOut);
    gl_FragColor = vec4(texel.xyz * DestinationColor + SpecularColor, texel.w * Opacity);
}

);
