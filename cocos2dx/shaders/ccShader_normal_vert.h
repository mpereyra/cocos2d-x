"                                                    \n\
attribute vec4 a_position;                           \n\
attribute vec3 a_normal;                             \n\
attribute vec2 a_texCoord;                           \n\
                                                     \n\
uniform        mat4 u_MVPMatrix;                     \n\
uniform        vec4 LightDirection;                  \n\
uniform        mat4 MVMatrix;                        \n\
                                                     \n\
#ifdef GL_ES                                         \n\
precision lowp float;                                \n\
varying mediump vec2 v_texCoord;                     \n\
#else                                                \n\
varying vec2 v_texCoord;                             \n\
#endif                                               \n\
varying float LightIntensity;                        \n\
                                                     \n\
void main()                                                 \n\
{                                                           \n\
    gl_Position = u_MVPMatrix * a_position;                 \n\
    v_texCoord = a_texCoord;                                \n\
                                                            \n\
    LightIntensity = clamp(dot(a_normal, vec3(MVMatrix * LightDirection))+0.25, 0.25, 1.25);  \n\
}                                                           \n\
";