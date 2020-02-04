static const char* TextTopGradientFragmentShader = STRINGIFY(

varying highp vec2 TextureCoordOut;
varying highp float TextGradientFunctionOut;

uniform sampler2D Sampler;
uniform mediump vec4 TextColor;
uniform mediump vec3 TextTopColorSubtraction;

void main(void) {
    gl_FragColor = vec4(TextColor.xyz - TextTopColorSubtraction * (1.0 - TextGradientFunctionOut), texture2D(Sampler, TextureCoordOut).a * TextColor.w);
}

);
