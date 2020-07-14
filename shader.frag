#version 400

uniform float time;
uniform ivec2 resolution;

out vec4 fragColor;
void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5*vec2(resolution)) / float(min(resolution.x, resolution.y));
    fragColor = vec4(vec3(length(uv)), 1.);
}
