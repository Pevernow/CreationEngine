$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor,  0);

vec3 toLinear(vec3 _rgb)
{
	return pow(abs(_rgb), vec3_splat(2.2) );
}

void main() {
    gl_FragColor = texture2D(s_texColor, v_texcoord0);
}
