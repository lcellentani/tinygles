#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying lowp vec4 v_color;
varying highp vec3 v_normal;
varying highp vec3 v_eyeCoord;

uniform mediump vec3 u_materialAmbient;
uniform mediump vec3 u_materialDiffuse;
uniform mediump vec3 u_materialSpecular;
uniform mediump float u_shininessFactor;
uniform mediump vec3 u_lightColor;
uniform highp vec3 u_lightPosition;

vec3 PhongShading()
{
	vec3 lightVector = normalize(u_lightPosition - v_eyeCoord);
	vec3 normal = normalize(v_normal);

	vec3 V = normalize(-v_eyeCoord);
	vec3 R = reflect(-lightVector, normal);
	//vec3 H = normalize(lightVector + V);

	float spec = dot(R, V);
	//float spec = dot(H, normal);

	vec3 diffuse = u_materialDiffuse * max(0.0, dot(normal, lightVector));
	vec3 specular = u_materialSpecular * pow(max(0.0, spec), u_shininessFactor);
				
	return (u_materialAmbient + diffuse + specular) * u_lightColor;
}

void main(void)
{
	gl_FragColor = vec4(PhongShading(), 1.0);
}
