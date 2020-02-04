static const char* TexturedPixelLightingFragmentShader = STRINGIFY(

varying highp vec3 NormalCameraSpace;
varying highp vec2 TextureCoordOut;

uniform highp vec3 LightPosition;  // Normalized camera space.
uniform highp vec3 AmbientMaterial;
uniform highp vec3 DiffuseMaterial;
uniform highp vec3 SpecularMaterial;
uniform highp float Shininess;
uniform mediump float Opacity;
uniform sampler2D Sampler;

void main(void) {
    highp vec3 N = normalize(NormalCameraSpace);
    highp vec3 E = vec3(0, 0, 1);
    highp vec3 H = normalize(LightPosition + E);
    
    highp float df = max(0.0, dot(N, LightPosition));
    highp float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    highp vec3 DestinationColor = AmbientMaterial + df * DiffuseMaterial;
    highp vec3 SpecularColor = sf * SpecularMaterial;
    highp vec4 texel = texture2D(Sampler, TextureCoordOut);
    gl_FragColor = vec4(texel.xyz * DestinationColor + SpecularColor, texel.w * Opacity);
}

);
