#version 330 core

// Inputs
in vec2 UV;
in vec3 fragmentPosition;
in vec3 normal;


// Outputs
out vec3 colour;
out vec3 fragmentColour;
out vec3 Normal;

// Uniforms
uniform sampler2D textureMap;
uniform sampler2D diffuseMap;
uniform float ka;
uniform float kd;
uniform vec3 lightColour;
uniform vec3 lightPosition;


void main()
{
    colour = vec3(texture(textureMap, UV));

    vec3 objectColour = vec3(texture(diffuseMap, UV));

    //ambient reflection
    vec3 ambient = ka* objectColour;

    //diffuse REFLection
    vec3 light = normalize(lightPosition - fragmentPosition);
    vec3 normal    = normalize(Normal);
    float cosTheta = max(dot(normal, light), 0);
    vec3 diffuse   = kd * lightColour * objectColour * cosTheta;

    fragmentColour = ambient;
}
