#shader compute
#version 430

layout(rgba32f, binding = 0) uniform image3D img_output;

layout(std140, binding = 3) buffer particles_in
{
	vec4 part_in[];
};

layout(location = 0) uniform float dt;

layout(std140, binding = 4) buffer particles_vel
{
	vec4 part_vel[];
};

float solveQuadPositive(float a, float b, float c) {

	return (-b + sqrt(b*b - 4 * a*c)) / 2 / a;
}
float solveQuadNegative(float a, float b, float c) {

	return (-b - sqrt(b * b - 4 * a * c)) / 2 / a;
}

layout(local_size_x = 256) in;
void main()
{
	ivec3 dims = imageSize(img_output);
	vec4 circleCenter = vec4(dims.x / 2, dims.y / 2, dims.z / 2, 0);
	float circleRadius = dims.x / 2;
	uint id = gl_GlobalInvocationID.x;
	vec4 pos = part_in[id];
	vec4 nextPos = vec4(pos.xyz + dt * part_vel[id].xyz, pos.w);
	vec4 osculation = circleCenter;
	imageStore(img_output, ivec3(pos.xyz), vec4(0, 0, 0, 0));


	float A = part_vel[id].x;
	float B = part_vel[id].y;
	float C = part_vel[id].z;
	float a = (A * A) + (B * B) + (C * C);
	float b = (2 * pos.x * A) + (2 * pos.y * B) + (2 * pos.z * C) - (2 * circleCenter.x * A) - (2 * circleCenter.y * B) - (2 * circleCenter.z * C);
	float c = (pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z) + (circleCenter.x * circleCenter.x) + (circleCenter.y * circleCenter.y) 
			  + (circleCenter.z * circleCenter.z) - (circleRadius * circleRadius) - (2 * pos.x * circleCenter.x) - (2 * pos.y * circleCenter.y)
			  - (2 * pos.z * circleCenter.z);
	vec4 pixel = vec4(1, 1, 1, 1);


	if (((b * b) - (4 * a * c)) >= 0) {

		float t1 = solveQuadPositive(a, b, c);
		vec4 p1 = vec4(pos.x + (A * t1), pos.y + (B * t1), pos.z + (C * t1), 1);


		if (length(nextPos.xyz - circleCenter.xyz) >= circleRadius) {
			osculation = vec4(p1.x, p1.y, p1.z, 0);
		}
	}

	if (osculation.x == circleCenter.x && osculation.y == circleCenter.y && osculation.z == circleCenter.z)
		part_in[id] = nextPos;
	else {
		vec3 normal = (circleCenter.xyz - osculation.xyz) / length(circleCenter.xyz - osculation.xyz);
		part_vel[id].xyz = (part_vel[id].xyz - 2 * (dot(part_vel[id].xyz, normal)) * normal);
		part_in[id] = vec4(osculation.xyz + dt * part_vel[id].xyz, pos.w);
	};

	imageStore(img_output, ivec3(part_in[id].xyz), pixel);
}




layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {

	// base pixel colour for image
	vec4 pixel = vec4(1.0, 0.0, 0.0, 1.0);
	//get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	//output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
}