/////////////////////////////////////////////////////////////////
// OpenGL ES require that precision is defined for a fragment shader
#ifndef USE_NORMAL_GLSL
#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif
#else
	#define lowp
	#define mediump
	#define highp
#endif

uniform lowp sampler2D g_texture;
uniform lowp vec4 g_modcol;

varying lowp vec4 col_var;
varying vec4 uv_var;

void main()
{
	lowp vec4 col		= texture2D(g_texture, uv_var.xy);
	lowp vec4 col2		= texture2D(g_texture, uv_var.zw);
	gl_FragColor	= vec4(col.a,col.a,col.a,max(col.a, col2.a*0.5)) * g_modcol * col_var;
	
}
