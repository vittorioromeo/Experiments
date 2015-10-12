precision mediump float;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_hue;

varying vec2 v_tex_coords;

const mat3 rgb2yiq = mat3(          // .
    0.299, 0.587, 0.114,            // .
    0.595716, -0.274453, -0.321263, // .
    0.211456, -0.522591, 0.311135   // .
    );

const mat3 yiq2rgb = mat3( // .
    1.0, 0.9563, 0.6210,   // .
    1.0, -0.2721, -0.6474, // .
    1.0, -1.1070, 1.7046   // .
    );

void main()
{
    vec4 tx_color = texture2D(u_texture, v_tex_coords);

    vec3 yColor = rgb2yiq * (u_color.rgb * tx_color.rgb);

    float originalHue = atan(yColor.b, yColor.g);

    float finalHue = originalHue + u_hue;

    float chroma = sqrt(yColor.b * yColor.b + yColor.g * yColor.g);

    vec3 yFinalColor =
        vec3(yColor.r, chroma * cos(finalHue), chroma * sin(finalHue));

    gl_FragColor = vec4(yiq2rgb * yFinalColor, u_color.w * tx_color.w);

    // vec4 color_before_hue = v_color * texture2D(u_texture, v_tex_coords);
    // gl_FragColor = color_after_hue;
}
