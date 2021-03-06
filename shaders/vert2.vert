#version 150

#define PI 3.141592653589793
#define PI_2 6.2831853072

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform int vertex_num;
uniform vec3 r_seed0;
in vec4 position;


vec3 rnd(float n, vec3 s){
    return vec3((fract(sin(dot(vec2(n, s.x), vec2(12.9898, 4.1414))) * 43758.5453) - 0.5) * 20.,
                (fract(sin(dot(vec2(n, s.y), vec2(12.9898, 4.1414))) * 43758.5453) - 0.5) * 20.,
                (fract(sin(dot(vec2(n, s.z), vec2(12.9898, 4.1414))) * 43758.5453) - 0.5) * 20.);
}

float rnd(vec2 n){
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

void main() {
    float th = (gl_VertexID + time) / float(vertex_num) * 100. - time;
    float r = 500. * sin(2 * th);
    vec3 v_position = vec3(exp(th / PI_2) * cos(th) + sin(th * 2.), exp(th / PI_2) * sin(th) + cos(th * 2.), rnd(vec2(gl_VertexID, r_seed0.x)));
    gl_Position = modelViewProjectionMatrix * vec4(v_position, 1.);
}