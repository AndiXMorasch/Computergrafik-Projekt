#version 400

layout(location=0) in vec4 VertexPos;       // Eingabeposition des Vertex
layout(location=1) in vec4 VertexNormal;    // Normale des Vertex
layout(location=2) in vec2 MixTexcoord;     // Texturkoordiante der Mixmap (Aufgabe 3)
layout(location=3) in vec2 VertexTexcoord;  // Texturkoordiante des Vertex (mit k einberechnet)

out vec3 Position;                          // Position im Weltraum
out vec3 Normal;                            // Normale im Weltraum
out vec2 Mixcoord;                          // Texturkoordinaten der Mixmap
out vec2 Texcoord;                          // Texturkoordinate im Weltraum

uniform mat4 ModelMat;                      // Matrix in den Welt-Raum (Position)
uniform mat4 ModelViewProjMat;              // Kombinierte Matrix in norm. Bildraum
uniform vec3 Scaling;                       // Skalierungsvektor

void main()
{
    // Exercise 1
    
    // Position des Vertex skalieren
    vec4 newPos = VertexPos;
    newPos.xyz *= Scaling.xyz;

    // Ausrichtung der Normalen skalieren (mit der multiplikativen Inversen des Scalings)
    vec4 newNormal = VertexNormal;
    newNormal.xyz /= Scaling.xyz;

    // Normalisieren, da es sein kann dass die Normale durch die Skalierung verkürzt oder verlängert wurde
    newNormal = normalize(newNormal);

    if(Scaling.y == 0) {
    newNormal.xyz = vec3(0, 1, 0);
    }

    // Weiterreichen an den Fragment-Shader
    Texcoord = VertexTexcoord;
    Mixcoord = MixTexcoord;
    Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(newNormal.xyz,0)).xyz; // Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
    gl_Position = ModelViewProjMat * newPos; // VertexPos; // Ausgabeposition des Vertex in norm. Bildkoordinaten
}
