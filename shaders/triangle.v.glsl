

attribute vec2 coord2d; 
attribute vec4 v_color;
varying vec4 f_color;
uniform vec2 transform2d;

void main(void) 
{   
	gl_Position = vec4(coord2d +transform2d, 0.0, 1.0);   
	f_color =  v_color;
}


