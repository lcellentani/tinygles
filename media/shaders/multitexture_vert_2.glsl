#version 100

attribute highp vec4 a_position;
//attribute highp vec3 a_normal;
attribute lowp vec2 a_texcoord0;
attribute lowp vec4 a_color0;

uniform mediump mat4 u_modelViewProj;
//uniform mediump mat4 u_modelView;

varying lowp vec4 v_color;
varying lowp vec2 v_texcoord0;

void main(void)
{	
	v_color = a_color0;
	v_texcoord0 = a_texcoord0;
	gl_Position = u_modelViewProj * a_position;
}