#version 100

attribute highp vec4 a_position;
//attribute highp vec3 a_normal;
attribute lowp vec4 a_color0;

uniform mediump mat4 u_modelViewProj;
//uniform mediump mat4 u_modelView;

varying lowp vec4 v_color;

void main(void)
{	
	v_color = a_color0;
	gl_Position = u_modelViewProj * a_position;
}