#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying lowp vec4 v_color;
varying lowp vec2 v_texcoord0;

uniform sampler2D u_texture0;

void main(void)
{
	vec4 tex0 = texture2D(u_texture0, v_texcoord0);
	tex0 = vec4(v_texcoord0.x, v_texcoord0.y, 0, 1);
	gl_FragColor = v_color * tex0;
	//gl_FragColor = v_color * vec4(v_texcoord0.x, v_texcoord0.y, 0, 1);
}
