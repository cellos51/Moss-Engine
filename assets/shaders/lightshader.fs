#version 410 core

uniform vec4 lightColor;

out vec4 FragColor;

varying vec2 v_texcoord;

float circleshape(vec2 position, float radius){
  return step(radius, length(position - vec2(0.5)));
}

void main(){
  vec2 position = v_texcoord;

  vec3 color = vec3(0);

  float outerCircle = circleshape(position, 0.5);

  float innerCircle = circleshape(position, 0.4);

  color += vec3(1 - outerCircle) / 2;

  color += vec3(1 - innerCircle);

  gl_FragColor = vec4(color * lightColor.xyz, 0.5f);
}

// void main()
// {
//     vec2 st = v_texcoord;
//     vec3 canvas = vec3(2);
//     float circle = distance(st, vec2(0.5));

//     canvas = canvas * circle;

//     FragColor = vec4(-canvas + 1, -canvas + 1) * lightColor;
// }