#version 460 core

uniform vec4 lightColor;

uniform int layerId;

out vec4 FragColor;

in vec2 v_texcoord;

//float circleshape(vec2 position, float radius)
//{
//  return step(radius, length(position - vec2(0.5)));
//}
//
//
//void main()
//{
//  gl_FragDepth = (1.0f - (layerId / 100.0f));
//
//  vec2 position = v_texcoord;
//
//  vec3 color = vec3(0);
//
//  float outerCircle = circleshape(position, 0.5);
//
//  float innerCircle = circleshape(position, 0.4);
//
//  color += vec3(1 - outerCircle) / 2;
//
//  color += vec3(1 - innerCircle);
//
//  if (color.xyz == vec3(1 - outerCircle) / 2)
//  {
//    gl_FragDepth = (1.0f - ((layerId - 0.2f) / 100.0f));
//  }
//
//
//  if (color.xyz == vec3(0,0,0))
//  {
//    discard;
//  }
//
//  FragColor = vec4(color * lightColor.xyz, 0.5f);
//}


 void main()
 {
   gl_FragDepth = (1.0f - (layerId / 100.0f));

   vec2 st = v_texcoord;
   vec3 canvas = vec3(2);
   float circle = distance(st, vec2(0.5));

   canvas = canvas * circle;

   if (vec4(-canvas + 1, -canvas + 1).x < 0.01f)
   {
     discard;
   }
  
   FragColor = vec4(-canvas + 1, -canvas + 1) * lightColor;
 }