#version 100

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute lowp vec2 a_texcoord0;
attribute lowp vec4 a_color0;

uniform mediump mat4 u_modelViewProj;
uniform mediump mat4 u_modelView;
uniform highp vec3 u_lightPosition;

varying lowp vec4 v_color;
varying lowp vec2 v_texcoord0;

void main(void)
{	
	vec3 modelViewPos = vec3(u_modelView * a_position);
	vec3 lightVector = normalize(u_lightPosition - modelViewPos);
	vec3 normal = normalize(vec3(u_modelView * vec4(a_normal, 0.0)));
	float diffuse = max(0.0, dot(normal, lightVector));
	v_color.rgb = a_color0.rgb * diffuse;
	v_color.a = a_color0.a;
	v_texcoord0 = a_texcoord0;
	gl_Position = u_modelViewProj * a_position;
}