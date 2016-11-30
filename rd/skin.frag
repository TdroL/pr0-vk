// http://c0de517e.blogspot.com/2012/11/addendum-to-mathematica-and-skin.html
vec3 PSSFitFunction(float NdL, float r)
{
   vec3 a0 = { 0.0605,  0.2076,  0.2243};
   vec3 a1 = { 0.0903,  0.1687,  0.2436};
   vec3 a2 = {-0.0210, -0.0942, -0.1116};
   vec3 a3 = { 0.6896,  0.6762,  0.6480};
   vec3 a4 = {-0.1110, -0.5023, -0.6703};
   vec3 a5 = { 0.8177,  0.9119,  0.9209};

   vec3 t = NdL * (a0 * r + a1) + (a2 * r + a3);
   vec3 fade = clamp(a4 * r + a5, 0.0, 1.0);

   return t*t*t * fade + clamp(NdL, 0.0, 1.0) * (1.0 - fade);
}