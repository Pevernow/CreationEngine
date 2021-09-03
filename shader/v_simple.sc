$input a_position, a_texcoord0, i_data0, i_data1, i_data2, i_data3, i_data4, a_texcoord2
$output v_texcoord0, v_data4, v_data5

#include <bgfx_shader.sh>

void main() {
    mat4 model;
	model[0] = i_data0;
	model[1] = i_data1;
	model[2] = i_data2;
	model[3] = i_data3;
    vec4 worldPos = instMul(model, vec4(a_position, 1.0) );
    gl_Position = mul(u_modelViewProj, worldPos);
    v_texcoord0 = vec3(a_texcoord0.x,a_texcoord0.y,a_texcoord0.z+i_data4.x*6);
    v_data4 = i_data4;
    v_data5 = a_texcoord2;
}
