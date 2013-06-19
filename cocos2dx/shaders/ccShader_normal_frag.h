"                                           \n\
#ifdef GL_ES                                \n\
precision lowp float;                       \n\
#endif                                      \n\
                                            \n\
varying vec2 v_texCoord;                    \n\
uniform sampler2D u_texture;                \n\
//varying lowp    float  LightIntensity;      \n\
                                            \n\
void main()                                                              \n\
{                                                                        \n\
//    gl_FragColor = texture2D(u_texture, v_texCoord) * LightIntensity;    \n\
    gl_FragColor = texture2D(u_texture, v_texCoord);                     \n\
}                                                                        \n\
";
