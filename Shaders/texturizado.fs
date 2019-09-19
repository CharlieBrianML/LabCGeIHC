#version 330 core

in vec2 out_uv;
out vec4 color;
//Unidad de textura del tiepo smapler2D
uniform sampler2D ourTexture;

void main(){
	//Se obtiene el color de la textura en base a la unidad de la textura y las coordenadas
	color = texture(ourTexture, out_uv);
	//color = vec4(0.3, 0.5, 0.4, 1.0);
}