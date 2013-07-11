"                                           \n\
#ifdef GL_ES                                \n\
precision lowp float;                       \n\
#endif                                      \n\
                                            \n\
varying vec2 v_texCoord;                    \n\
uniform sampler2D u_texture;                \n\
varying float LightIntensity;               \n\
                                            \n\
void main()                                                              \n\
{                                                                        \n\
    vec4 light = vec4(LightIntensity,LightIntensity,LightIntensity,1.0); \n\
    gl_FragColor = texture2D(u_texture, v_texCoord);             \n\
}                                                                        \n\
";
