#version 450 core
in vec4 vertexMC;
out vec4 worldPos;
uniform mat4 MCDCMatrix;
uniform mat4 MCWCMatrix;
void main(){
	vec3 dir = mat3(inverse(MCWCMatrix)) * vec3(0,0,1);
	vec3 vertex = vertexMC.xyz + dir*100;
	worldPos = MCWCMatrix * vec4(vertex,1);
	gl_Position = MCDCMatrix * vec4(vertex,1);
}