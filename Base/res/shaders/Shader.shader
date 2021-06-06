#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
void main() {

		gl_Position = position;
}

#shader fragment
#version 430 core
out vec4 color;
layout(binding = 0) uniform sampler3D img_output;
void main() {
	 //floadensity = texture(img_output, text_Coord);
	color = vec4(0, 1, 0, 1);
}
