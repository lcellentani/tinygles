#version 100

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute lowp vec4 a_color0;

uniform mediump mat4 u_modelViewProj;
uniform mediump mat4 u_modelView;
uniform mediump vec3 u_materialAmbient;
uniform mediump vec3 u_lightAmbient;
uniform mediump vec3 u_materialDiffuse;
uniform mediump vec3 u_lightDiffuse;
uniform mediump vec3 u_materialSpecular;
uniform mediump vec3 u_lightSpecular;
uniform mediump float u_shininessFactor;
uniform highp vec3 u_lightPosition;

varying lowp vec4 v_color;

void main(void)
{
	vec3 modelViewPos = vec3(u_modelView * a_position);
	vec3 lightVector = normalize(u_lightPosition - modelViewPos);
	vec3 normal = normalize(vec3(u_modelView * vec4(a_normal, 0.0)));
				
	vec3 V = normalize(-modelViewPos);
	vec3 R = reflect(-lightVector, normal);
	//vec3 H = normalize(lightVector + V);

	float spec = dot(R, V);
	//float spec = dot(H, normal);

	vec3 ambient = u_materialAmbient * u_lightAmbient;
	vec3 diffuse = (u_materialDiffuse * u_lightDiffuse) * max(0.0, dot(normal, lightVector));
	vec3 specular = (u_materialSpecular * u_lightSpecular) * pow(max(0.0, spec), u_shininessFactor);
				
	v_color.rgb = ambient + diffuse + specular;
	v_color.a = 1.0;
	gl_Position = u_modelViewProj * a_position;
}