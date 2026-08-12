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
    FragColor = texture(tex, fragUV);
}
)";

inline const char* BrushVxShader = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
uniform mat4 ortho;
uniform vec2 mospos;
out vec2 fragUV;
void main() {
	fragUV = aUV;
    gl_Position = ortho * vec4(aPos.xy + mospos, 0, 1);
}
)";

inline const char* BrushFgShader = R"(
#version 330 core
out vec4 FragColor;
in vec2 fragUV;
uniform sampler2D tex;
void main() {
    FragColor = texture(tex, fragUV) * 0.8f;
}
)";

inline const char* NetVxShader = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 ortho;
uniform vec2 cameraPos;
out vec2 worldPos;
void main() 
{
	vec2 pos = aPos.xy + cameraPos;
	worldPos = pos;
	gl_Position = ortho * vec4(pos, 0.0, 1.0);
}
)";

inline const char* NetFgShader = R"(
#version 330 core
out vec4 FragColor;
in vec2 worldPos;
uniform float zoom;

void main() {

	vec3 colorFar = vec3(0.1175f, 0.1175f, 0.1175f);

	if(zoom < 5.0f)
	{
    	vec2 grid = abs(fract(worldPos-0.5)-0.5); 
		float thickness = zoom/ 70;
		
		if (grid.x > thickness && grid.y > thickness)
		{
		    FragColor = vec4(colorFar, 1.0);
		    return;
		}
		else
		{
			vec2 ChunkGrid = abs(fract((worldPos-vec2(0,1))/16.0f + 0.5f) - 0.5f); 
			vec3 colorNear = vec3(0.2, 0.2, 0.2);
		
			if(ChunkGrid.x < thickness/15 || ChunkGrid.y < thickness/15)
			{
				colorNear = vec3(0.4, 0.4, 0.4);
			}
			FragColor = vec4(mix(colorNear, colorFar, zoom/5),1);
		}
	}
	else
	{
	    FragColor = vec4(colorFar,1);
	}
}
)";

