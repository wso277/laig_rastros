uniform float delta; 
uniform sampler2D heightmap; 
uniform sampler2D texturemap; 

void main() { 
	gl_FragColor = texture2D(texturemap, gl_TexCoord[0].st); 
}