#version 330
//fragment shader
in vec3 vertPos;

out vec4 FragColor;

void main() {

    vec3 posAbs  = abs(vertPos);
    vec3 color   = step(posAbs.yzx, posAbs) * step(posAbs.zxy, posAbs);
    color       += (1.0 - step(color.zxy * vertPos.zxy, vec3(0.0)));

    FragColor = vec4(color, 1.0);
}
