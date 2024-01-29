#version 330 core
layout(location=0) in vec3 positionData;
layout(location=1) in vec3 colorData;
uniform mat4 model, view, projection;
out vec3 fragmentColor;
void main(){
	gl_Position=projection*view*model*vec4(positionData, 1.0f);
	fragmentColor=colorData;
}