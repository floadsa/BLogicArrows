#pragma once

inline const char* VxShader = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
uniform mat4 ortho;
out vec2 fragUV;
void main() {
	fragUV = aUV;
    gl_Position = ortho * vec4(aPos, 1);
}
)";

inline const char* FgShader = R"(
#version 330 core
out vec4 FragColor;
in vec2 fragUV;
uniform sampler2D tex;
void main() {
    FragColor = texture(tex, fragUV);;
}
)";
