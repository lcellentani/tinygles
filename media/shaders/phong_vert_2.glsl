#version 100

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute lowp vec4 a_color0;

uniform mediump mat4 u_modelViewProj;
uniform mediump mat4 u_modelView;

varying lowp vec4 v_color;
varying highp vec3 v_normal;
varying highp vec3 v_eyeCoord;

void main()
{
	v_eyeCoord = vec3(u_modelView * a_position);
	v_normal = normalize(vec3(u_modelView * vec4(a_normal, 0.0)));
	v_color = a_color0;
	gl_Position = u_modelViewProj * a_position;
}
