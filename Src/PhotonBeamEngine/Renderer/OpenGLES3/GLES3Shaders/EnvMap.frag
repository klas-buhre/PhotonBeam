static const char* EnvMapFragmentShader = STRINGIFY(

uniform samplerCube Sampler;
uniform mediump float Opacity;
uniform highp float Reflectivity;

varying highp vec3 DestinationColor;
varying highp vec3 SpecularColor;
varying mediump vec3 ReflectDir;

void main(void) {
    highp vec3 reflectedTexel = textureCube(Sampler, ReflectDir).xyz;
    highp vec3 blendedTexel = (Reflectivity * reflectedTexel + (1.0 - Reflectivity) * DestinationColor);
    gl_FragColor = vec4(blendedTexel * DestinationColor + SpecularColor, Opacity);
}

);
