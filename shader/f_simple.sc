$input v_texcoord0, v_data4

#include <bgfx_shader.sh>

SAMPLER2DARRAY(s_texColor, 0);

vec3 toLinear(vec3 _rgb)
{
	return pow(abs(_rgb), vec3_splat(2.2) );
}

void main() {
    //gl_FragColor = texture2DArray(s_texColor, vec3(v_texcoord0.x,v_texcoord0.y,2.5));
    vec4 color = texture2DArray(s_texColor, v_texcoord0.xyz);
    color.rgb *= v_data4.y/15;
    gl_FragColor = color;
}
