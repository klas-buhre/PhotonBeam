static const char* TexturedEnvMapLightingFragmentShader = STRINGIFY(

uniform sampler2D Sampler;
uniform samplerCube SecondSampler;
uniform mediump float Opacity;
uniform highp float Reflectivity;

varying highp vec3 DestinationColor;
varying highp vec3 SpecularColor;
varying mediump vec3 ReflectDir;
varying mediump vec2 TextureCoordOut;

void main(void) {
    highp vec4 texel = texture2D(Sampler, TextureCoordOut);
    highp vec3 reflectedTexel = textureCube(SecondSampler, ReflectDir).xyz;
    highp vec3 blendedReflectedTexel = (Reflectivity * reflectedTexel + (1.0 - Reflectivity) * DestinationColor);
    gl_FragColor = vec4(texel.xyz * blendedReflectedTexel * DestinationColor + SpecularColor, texel.w * Opacity);
}

);
