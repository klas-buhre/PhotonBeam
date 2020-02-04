static const char* VertexLightingVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;

uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;  // Normalized camera space.
uniform vec3 AmbientMaterial;
uniform vec3 DiffuseMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;
uniform float Opacity;

varying vec4 DestinationColor;

void main(void) {
    vec3 N = normalize(NormalMatrix * Normal);
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(LightPosition + E);

    float df = max(0.0, dot(N, LightPosition));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    vec3 color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;
    
    DestinationColor = vec4(color, Opacity);
    gl_Position = ModelViewProjection * Position;
}

);
