#shader compute
#version 430

layout(rgba32f, binding = 0) uniform image3D img_output;
layout(location = 0) uniform float size;

layout(std140, binding = 3) buffer particles_in
{
	vec4 part_in[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main() {

	int s = int(size);
	int count = 0;
	vec3 textCoord = gl_GlobalInvocationID.xyz;
	int i;
	for (i = 0; i < s; i++) {
		if (part_in[i].xyz == textCoord.xyz)
			count++;
	}
	imageStore(img_output, ivec3(textCoord.xyz), vec4(1,0,0,1));
}