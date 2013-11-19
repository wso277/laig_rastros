uniform float delta; 
uniform sampler2D heightmap; 

void main() { 
	vec2 tex_displace = vec2(0.0, delta); 
	vec4 hight_component = texture2D(heightmap, gl_MultiTexCoord0.st + tex_displace); 
	vec4 pt_displace = vec4(0.0, hight_component.r, 0.0, 0.0); 
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * (pt_displace + gl_Vertex); 
	gl_TexCoord[0].st = gl_MultiTexCoord0.st + tex_displace; 
}