$input v_texcoord0, v_data4, v_data5

#include <bgfx_shader.sh>

SAMPLER2DARRAY(s_texColor, 0);

vec3 toLinear(vec3 _rgb)
{
	return pow(abs(_rgb), vec3_splat(2.2) );
}

void main() {
    vec4 color = texture2DArray(s_texColor, v_texcoord0.xyz);
    
    //calc light
    //TODO: smooth light
    color.rgb *= pow(0.8,15-v_data4.y);

    gl_FragColor = color;
}
