#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec4 picking_color;


void main()
{

		gl_FragColor =  picking_color;


}
