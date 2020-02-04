static const char* TextFragmentShader = STRINGIFY(

varying highp vec2 TextureCoordOut;

uniform sampler2D Sampler;
uniform mediump vec4 TextColor;

void main(void) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(Sampler, TextureCoordOut).a) * TextColor;
}

);
