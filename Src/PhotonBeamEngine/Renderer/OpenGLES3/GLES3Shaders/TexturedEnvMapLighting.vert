static const char* TexturedEnvMapLightingVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec3 Normal;
attribute vec2 TextureCoord;

uniform mat4 ModelViewProjection;
uniform mat4 Model;
uniform mat3 Model3x3;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;  // Normalized camera space.
uniform vec3 CameraPosition; // World space.
uniform vec3 AmbientMaterial;
uniform vec3 DiffuseMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

varying vec3 DestinationColor;
varying vec3 SpecularColor;
varying vec3 ReflectDir;
varying vec2 TextureCoordOut;

void main(void) {
    gl_Position = ModelViewProjection * Position;

    // Vertex lighting calculations in camera space:
    vec3 N = normalize(NormalMatrix * Normal);
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(LightPosition + E);

    float df = max(0.0, dot(N, LightPosition));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    DestinationColor = AmbientMaterial + df * DiffuseMaterial;
    SpecularColor = sf * SpecularMaterial;
    
    TextureCoordOut = TextureCoord;
    
    // Reflection calculations in world space:
    vec3 NormalWorldSpace = normalize(Model3x3 * Normal);
    vec3 PositionWorldSpace = vec3(Model * Position);
    
    vec3 I = normalize(PositionWorldSpace - CameraPosition);
    ReflectDir = I - 2.0 * dot(NormalWorldSpace, I) * NormalWorldSpace;
}

);
