

attribute vec4 coord; 
varying vec2 texpos; 
uniform vec2 transform2d;

void main(void) 
{   
	gl_Position = vec4(coord.xy +transform2d, 0, 1);   
	texpos = coord.zw; 
}


