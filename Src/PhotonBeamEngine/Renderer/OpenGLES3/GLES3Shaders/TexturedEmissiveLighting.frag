static const char* TexturedEmissiveLightingFragmentShader = STRINGIFY(

varying lowp vec3 DestinationColor;
varying lowp vec3 SpecularColor;
varying mediump vec2 TextureCoordOut;

uniform mediump float Opacity;
uniform mediump vec3 EmissiveMaterial;
uniform sampler2D Sampler;
uniform sampler2D SecondSampler;

void main(void) {
    mediump vec4 texel = texture2D(Sampler, TextureCoordOut);
    mediump vec4 emissiveTexel = texture2D(SecondSampler, TextureCoordOut);
    gl_FragColor = vec4(texel.xyz * DestinationColor + SpecularColor + emissiveTexel.xyz * EmissiveMaterial, texel.w * Opacity);
}

);
