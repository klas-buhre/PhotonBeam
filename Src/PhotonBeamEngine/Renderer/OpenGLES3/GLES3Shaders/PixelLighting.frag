static const char* PixelLightingFragmentShader = STRINGIFY(

varying mediump vec3 NormalCameraSpace;

uniform highp vec3 LightPosition;  // Normalized camera space.
uniform highp vec3 AmbientMaterial;
uniform highp vec3 DiffuseMaterial;
uniform highp vec3 SpecularMaterial;
uniform highp float Shininess;
uniform mediump float Opacity;

void main(void) {
    highp vec3 N = normalize(NormalCameraSpace);
    highp vec3 E = vec3(0, 0, 1);
    highp vec3 H = normalize(LightPosition + E);
    
    highp float df = max(0.0, dot(N, LightPosition));
    highp float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    lowp vec3 Color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;

    gl_FragColor = vec4(Color, Opacity);
}

);
