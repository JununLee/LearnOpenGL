#version 450 core
in vec4 vertexMC;
in vec2 tcoord;
out vec2 TCoord;
out vec4 worldPos;
out vec3 cpos;
uniform mat4 MCDCMatrix;
uniform mat4 MCWCMatrix;
uniform mat4 MCVCMatrix;
void main(){
	TCoord = tcoord;
	worldPos = MCWCMatrix * vertexMC;
	vec4 mcpos = inverse(MCVCMatrix) * vec4(0,0,0,1);
	cpos = (MCWCMatrix * mcpos).xyz;
	gl_Position = MCDCMatrix * vertexMC;
}