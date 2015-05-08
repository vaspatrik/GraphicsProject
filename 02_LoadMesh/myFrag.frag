#version 130

in vec3 vs_out_col;
in vec2 vs_out_tex0;
out vec4 fs_out_col;

uniform sampler2D textureImg;

void main()
{

	if (texture(textureImg, vs_out_tex0.st).rgb != vec3(0,0,0)) {
		fs_out_col = texture(textureImg, vs_out_tex0.st);
	} else {
		fs_out_col = vec4(vs_out_col, 1);
	}
}