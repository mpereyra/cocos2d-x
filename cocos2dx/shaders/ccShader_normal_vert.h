"                                                    \n\
attribute vec4 a_position;                           \n\
attribute vec3 a_normal;                             \n\
attribute vec2 a_texCoord;                           \n\
                                                     \n\
uniform        mat4 u_MVPMatrix;                     \n\
                                                     \n\
#ifdef GL_ES                                         \n\
precision lowp float;                                \n\
uniform mediump vec3 LightDirection;                 \n\
varying mediump vec2 v_texCoord;                     \n\
#else                                                \n\
uniform vec3 LightDirection;                         \n\
varying vec2 v_texCoord;                             \n\
#endif                                               \n\
varying float LightIntensity;                        \n\
                                                     \n\
void main()                                            \n\
{                                                      \n\
    gl_Position = u_MVPMatrix * a_position;            \n\
    v_texCoord = a_texCoord;                           \n\
                                                       \n\
    vec3 myLight = vec3(0.0,0.0,1.0);                    \n\
    vec3 myLightNormal = myLight / length(myLight);      \n\
    LightIntensity = dot(a_normal, myLightNormal) + 0.6; \n\
}                                                        \n\
";