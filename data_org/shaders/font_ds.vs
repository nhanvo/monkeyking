/////////////////////////////////////////////////////////////////

#ifdef USE_NORMAL_GLSL
	#define lowp
	#define mediump
	#define highp
#endif

#ifndef _WINDOWS
#define OUTPUT_POS(_x) gl_Position = vec4(_x.y, -_x.x, _x.z, _x.w);
#else
#define OUTPUT_POS(_x) gl_Position = _x;
#endif

#define CONTRAST_ADJUST(_x)
//#define CONTRAST_ADJUST(_x) _x = _x*0.9+0.1
 
uniform mat4 g_mTransform;

attribute vec3 pos_attr;
attribute lowp vec4 col_attr;
attribute vec2 uv_attr;

varying lowp vec4 col_var;
varying vec4 uv_var;

void main()
{
	vec4 pos = g_mTransform * vec4(pos_attr.x, pos_attr.y, pos_attr.z, 1.0);
	col_var = col_attr;
	uv_var = vec4(uv_attr.x,uv_attr.y,uv_attr.x-2.0/512.0,uv_attr.y-2.0/512.0);
	OUTPUT_POS(pos);
}

/////////////////////////////////////////////////////////////////
