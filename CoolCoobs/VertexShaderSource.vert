#version 330 core
layout(location=0) in vec3 positionData;
layout(location=1) in vec3 colorData;
out vec3 fragmentColor;
void main(){
	gl_Position=vec4(positionData, 1.0f);
	fragmentColor=colorData;
}