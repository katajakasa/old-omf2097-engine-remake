uniform vec2 textureSize;

void main() {
  float x = 0.5 * (1.0 / textureSize.x);
  float y = 0.5 * (1.0 / textureSize.y);
  vec2 dg1 = vec2( x, y);
  vec2 dg2 = vec2(-x, y);
  vec2 dx = vec2(x, 0.0);
  vec2 dy = vec2(0.0, y);

  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_TexCoord[1].xy = gl_TexCoord[0].xy - dg1;
  gl_TexCoord[1].zw = gl_TexCoord[0].xy - dy;
  gl_TexCoord[2].xy = gl_TexCoord[0].xy - dg2;
  gl_TexCoord[2].zw = gl_TexCoord[0].xy + dx;
  gl_TexCoord[3].xy = gl_TexCoord[0].xy + dg1;
  gl_TexCoord[3].zw = gl_TexCoord[0].xy + dy;
  gl_TexCoord[4].xy = gl_TexCoord[0].xy + dg2;
  gl_TexCoord[4].zw = gl_TexCoord[0].xy - dx;
}