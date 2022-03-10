#version 450 core
out vec4 FragColor;
in  vec4 worldPos;

void main(){
	
	if(worldPos.z>0){
		
		FragColor = vec4(1,0,0,0.6);
	}
	else{
	
		FragColor = vec4(0,1,0,0.2);
	}
}