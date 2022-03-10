#version 450 core
out vec4 FragColor;
in  vec4 worldPos;

void main(){
	if(worldPos.z>100){
		
		discard;
	}
	else{
	
		FragColor = vec4(0,0,1,1);
	}
}