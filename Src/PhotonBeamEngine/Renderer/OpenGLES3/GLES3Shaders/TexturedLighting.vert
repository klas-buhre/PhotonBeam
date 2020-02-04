static const char* TexturedLightingVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;
attribute vec2 TextureCoord;

uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;  // Normalized camera space.
uniform vec3 AmbientMaterial;
uniform vec3 DiffuseMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

varying vec3 DestinationColor;
varying vec3 SpecularColor;
varying vec2 TextureCoordOut;

void main(void) {
    vec3 N = normalize(NormalMatrix * Normal);
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(LightPosition + E);

    float df = max(0.0, dot(N, LightPosition));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    DestinationColor = AmbientMaterial + df * DiffuseMaterial;
    SpecularColor = sf * SpecularMaterial;
    
    gl_Position = ModelViewProjection * Position;
    TextureCoordOut = TextureCoord;
}

);
