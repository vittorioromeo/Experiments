attribute vec4 a_pos_tex_coords;
uniform mat4 u_projection_view_model;
varying vec2 v_tex_coords;

void main()
{
    v_tex_coords = a_pos_tex_coords.zw;
    gl_Position = u_projection_view_model * vec4(a_pos_tex_coords.xy, 0.0, 1.0);
}
