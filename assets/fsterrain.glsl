#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

// ‹bergabe aus Terrain.cpp
uniform sampler2D MixTex; // for exercise 3
uniform sampler2D DetailTex[2]; // for exercise 3
uniform vec3 Scaling;

// Entgegennehmen aus dem Vertex-Shader
in vec3 Position;                           // Position im Weltraum
in vec3 Normal;                             // Normale im Weltraum
in vec2 Mixcoord;                           // Texturkoordinaten der Mixmap
in vec2 Texcoord;                           // Texturkoordinate im Weltraum

out vec4 FragColor;                         // Farbwert

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}


void main()
{
    vec3 N      = normalize(Normal);
    vec3 L      = normalize(LightPos); // light is treated as directional source
    vec3 D      = EyePos-Position;
    float Dist  = length(D);
    vec3 E      = D/Dist;
    vec3 R      = reflect(-L,N);
    
    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L));
    vec3 SpecularComponent = LightColor * SpecularColor * pow( sat(dot(R,E)), SpecularExp);
    
    // Exercise 3
    // TODO: Add texture blending code here..

    vec4 mixTex = texture(MixTex, Mixcoord);                    // texture mapping MixMap
    vec4 firstDetailTex = texture(DetailTex[0], Texcoord);      // texture mapping DetailTexture[0] (grass)
    vec4 secondDetailTex = texture(DetailTex[1], Texcoord);     // texture mapping DetailTexture[1] (rock)
    
    FragColor = vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1) * mix(firstDetailTex, secondDetailTex, mixTex); 
    // lineare Interpolation zwischen firstDetailTex und secondDetailTex bestimmt durch den Farbwert aus mixTex (je weiﬂer umso mehr Rock)
}
