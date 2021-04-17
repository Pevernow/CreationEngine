$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2DARRAY(s_texColor, 0);

vec3 toLinear(vec3 _rgb)
{
	return pow(abs(_rgb), vec3_splat(2.2) );
}

void main() {
    //gl_FragColor = texture2DArray(s_texColor, vec3(v_texcoord0.x,v_texcoord0.y,2.5));
    gl_FragColor = texture2DArray(s_texColor, v_texcoord0.xyz);
}
