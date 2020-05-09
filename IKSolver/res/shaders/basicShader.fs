#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{

	gl_FragColor = vec4(color0,1.0)*texture2D(texture1,texCoord0);

}
