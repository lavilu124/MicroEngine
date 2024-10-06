#version 330 core

uniform vec2 lightCenter; 
uniform float lightRadius;
uniform vec4 lightColor;
uniform float windowHeight;
uniform float lightIntensity;

void main() {
    vec2 fragPos = vec2(gl_FragCoord.x, windowHeight - gl_FragCoord.y); // Get the current fragment position

    
    float distance = length(fragPos - lightCenter);

    
    float attenuation = 1 / (distance * distance / (lightRadius * lightRadius) + 1.0);

    
    float intensity = attenuation * attenuation * lightIntensity;

    
    gl_FragColor = vec4(lightColor.rgb * intensity, lightColor.a * intensity);
}
