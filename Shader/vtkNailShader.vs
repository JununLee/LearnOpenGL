#version 450 core
in vec4 vertexMC;
out vec4 worldPos;
uniform mat4 MCDCMatrix;
uniform mat4 MCWCMatrix;
void main(){
	worldPos = MCWCMatrix * vertexMC;
	gl_Position = MCDCMatrix * vertexMC;
}