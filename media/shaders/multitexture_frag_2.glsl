#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying lowp vec4 v_color;
varying lowp vec2 v_texcoord0;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

void main(void)
{
	vec4 tex0 = texture2D(u_texture0, v_texcoord0);
	vec4 tex1 = texture2D(u_texture1, v_texcoord0);
	gl_FragColor = v_color * mix(tex0, tex1, tex1.a);
}
