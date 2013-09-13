"                                            \n\
#ifdef GL_ES                                 \n\
precision lowp float;                        \n\
#endif                                       \n\
                                             \n\
varying vec4 v_fragmentColor;                \n\
varying vec2 v_texCoord;                     \n\
uniform sampler2D u_texture;                 \n\
uniform float u_highlight;                   \n\
                                             \n\
void main()                                  \n\
{                                            \n\
gl_FragColor = v_fragmentColor * texture2D(u_texture, v_texCoord) * vec4(u_highlight,u_highlight,u_highlight,1.0);            \n\
}                                            \n\
";
