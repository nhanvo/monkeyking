/////////////////////////////////////////////////////////////////
// OpenGL ES require that precision is defined for a fragment shader

#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform lowp sampler2D g_texture;

varying lowp vec4 col_var;
varying vec2 uv_var;

void main()
{
	lowp vec4 col	= texture2D(g_texture, uv_var);
	gl_FragColor	= col * col_var;
}
