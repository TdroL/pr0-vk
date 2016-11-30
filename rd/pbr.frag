#define GLSLIFY 1

struct FragData {
  vec3 color;
  vec3 albedo;
  float opacity;
  float roughness;
  vec3 specularity;
  vec3 position;
  vec3 positionWorld;
  vec3 positionView;
  vec3 normalWorld;
  vec3 normalView;
  vec2 texCoord;
  vec3 eyePosView;
  vec3 eyeDirView;
  vec3 lightColor;
  float lightAtten;
  vec3 lightPosView;
  vec3 lightPosWorld;
  vec3 lightDirView;
  vec3 lightDirWorld;
  vec3 reflectionColor;
  vec3 irradianceColor;
  float exposure;
};

varying vec3 vPosition;
varying vec3 vPositionWorld;
varying vec3 vPositionView;
varying vec3 vNormalWorld;
varying vec3 vNormalView;
varying vec2 vTexCoord;

varying vec3 vLightPosView;

uniform vec3 lightPos;  //world coordinates
uniform vec4 lightColor;

uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform float exposure;

//implementation specific uniforms

uniform vec4 albedoColor;
uniform sampler2D albedoMap;
uniform sampler2D glossinessMap;
uniform sampler2D specularityMap;
uniform float roughness;
uniform float specularity;

uniform samplerCube irradianceMap;
uniform samplerCube reflectionMap;
uniform float triplanarTextureScale;
//

float cubemapSize = 256.0;
float maxMipMapLevel = 8.0;

#define LOD_ENABLED

#ifdef LOD_ENABLED
#ifdef GL_ES
  #extension GL_EXT_shader_texture_lod : require
  #define textureCubeLod textureCubeLodEXT
  #else
  #extension GL_ARB_shader_texture_lod : require
  #endif
#endif

vec3 correctGammaInput_1_0(vec3 color) {
  return pow(color, vec3(2.2));
  }


vec3 correctGammaOutput_2_1(vec3 color) {
  return pow(color, vec3(1.0/2.2));
  }


vec4 sampleTexture2D_3_2(inout FragData data_3_3, sampler2D texture, float scale_3_4) {
  vec3 blending_3_5 = abs( normalize(data_3_3.normalWorld) );
  blending_3_5 = normalize(max(blending_3_5, 0.00001)); // Force weights to sum to 1.0
  float b = (blending_3_5.x + blending_3_5.y + blending_3_5.z);
  blending_3_5 /= vec3(b, b, b);

  vec4 xaxis = texture2D( texture, data_3_3.positionWorld.zy * scale_3_4);
  vec4 yaxis = texture2D( texture, data_3_3.positionWorld.xz * scale_3_4);
  vec4 zaxis = texture2D( texture, data_3_3.positionWorld.xy * scale_3_4);

  //blend the results of the 3 planar projections.
  vec4 tex = xaxis * blending_3_5.x + yaxis * blending_3_5.y + zaxis * blending_3_5.z;

  return tex;
  }


/*
//thi-ng shadergraph
float schlick(float r0, float smooth, vec3 normal, vec3 view) {
  float d = clamp(1.0f - dot(normal, -view), 0.0f, 1.0f);
  float d2 = d * d;
  return mix(r0, 1.0, smooth * d2 * d2 * d);
  }
*/

/*
//disney principled brdf
float SchlickFresnel(float u) {
  float m = clamp(1-u, 0, 1);
  float m2 = m*m;
  return m2*m2*m; // pow(m,5)
  }
vec3 fresnelSchlickWithRoughness = vec3(SchlickFresnel(NdotV));
specularIBL *= fresnelSchlickWithRoughness * (1.0 - metallic);
*/

/*
//playcanvas
// Schlick's approximation
uniform float material_fresnelFactor; // unused
void getFresnel(inout psInternalData data) {
    float fresnel = 1.0 - max(dot(data.normalW, data.viewDirW), 0.0);
    float fresnel2 = fresnel * fresnel;
    fresnel *= fresnel2 * fresnel2;
    fresnel *= data.glossiness * data.glossiness;
    data.specularity = data.specularity + (1.0 - data.specularity) * fresnel;
    }
*/

void getFresnel_4_6(inout FragData data_4_7) {
  float glossiness = 1.0 - data_4_7.roughness;
  float NdotV = max(0.0, dot(data_4_7.normalView, data_4_7.eyeDirView));
  float d = 1.0 - NdotV;
  float d2 = d * d;
  float fresnel = d2 * d2 * d * glossiness; //TODO: glossiness^2 like in Unreal Engine?
  data_4_7.specularity = data_4_7.specularity + (1.0 - data_4_7.specularity) * fresnel;
  }


float lambertDiffuse_10_8(
  vec3 lightDirection,
  vec3 surfaceNormal) {
  return max(0.0, dot(lightDirection, surfaceNormal));
  }



float getLightDiffuse_5_9(inout FragData data_5_10) {
  return lambertDiffuse_10_8(data_5_10.normalView, data_5_10.lightDirView);
  }


float phongSpecular_9_11(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float shininess) {

  //Calculate Phong power
  vec3 R = -reflect(lightDirection, surfaceNormal);
  return pow(max(0.0, dot(viewDirection, R)), shininess);
  }



float getLightSpecular_6_12(inout FragData data_6_13) {
  float glossiness = 1.0 - data_6_13.roughness;
  float specPower = pow(2.0, glossiness * 11.0);

  return phongSpecular_9_11(data_6_13.lightDirView, data_6_13.eyeDirView, data_6_13.normalView, specPower);
  }


vec3 fixSeams_7_14(vec3 vec, float mipmapIndex, float cubemapSize) {
  float scale = 1.0 - exp2(mipmapIndex) / cubemapSize;
  float M = max(max(abs(vec.x), abs(vec.y)), abs(vec.z));
  if (abs(vec.x) != M) vec.x *= scale;
  if (abs(vec.y) != M) vec.y *= scale;
  if (abs(vec.z) != M) vec.z *= scale;
  return vec;
  }


vec3 tonemapReinhard_8_15(vec3 color, float exposure) {
  vec3 c = color * exposure;
  return c / (1.0 + c);
  }



void getIrradiance(inout FragData data) {
  data.irradianceColor = textureCubeLod(irradianceMap, fixSeams_7_14(data.normalWorld, 0.0, cubemapSize), 0.0).rgb;
  }

void getReflection(inout FragData data) {
  vec3 eyeDirWorld = vec3(invViewMatrix * vec4(data.eyeDirView, 0.0));
  vec3 reflectionWorld = reflect(-eyeDirWorld, data.normalWorld); //eye coordinates reflection vector

  float lod = data.roughness * maxMipMapLevel;
  float upLod = floor(lod);
  float downLod = ceil(lod);
  vec4 a = textureCubeLod(reflectionMap, fixSeams_7_14(reflectionWorld, upLod, cubemapSize), upLod);
  vec4 b = textureCubeLod(reflectionMap, fixSeams_7_14(reflectionWorld, downLod, cubemapSize), downLod);

  data.reflectionColor = data.specularity * mix(a, b, lod - upLod).rgb;
  }

void main() {
  FragData data;
  data.color = vec3(0.0);
  data.albedo = vec3(1.0);
  data.opacity = 1.0;
  data.position = vPosition;
  data.positionWorld = vPositionWorld;
  data.positionView = vPositionView;
  data.normalWorld = normalize(vNormalWorld);
  data.normalView = normalize(vNormalView);
  data.texCoord = vTexCoord;
  data.eyePosView = vec3(0.0, 0.0, 0.0);
  data.eyeDirView = normalize(data.eyePosView - data.positionView);
  data.lightAtten = 1.0;
  data.lightColor = correctGammaInput_1_0(lightColor.rgb);
  data.lightPosWorld = lightPos;
  data.lightPosView = vLightPosView;
  //data.lightDirWorld = normalize(-data.lightPosWorld);
  //data.lightDirView = normalize(data.lightPosView - vec3(viewMatrix * vec4(0.0, 0.0, 0.0, 1.0)));
  data.lightDirWorld = normalize(data.lightPosWorld - data.positionWorld);
  data.lightDirView = normalize(data.lightPosView - data.positionView);
  data.reflectionColor = vec3(0.0);
  data.exposure = exposure;

  //computation
  data.albedo = correctGammaInput_1_0(albedoColor.rgb);
  data.albedo = correctGammaInput_1_0(sampleTexture2D_3_2(data, albedoMap, triplanarTextureScale).rgb);
  data.roughness = roughness;
  data.roughness = 1.0 - correctGammaInput_1_0(sampleTexture2D_3_2(data, glossinessMap, triplanarTextureScale).rgb).r;
  data.specularity = vec3(specularity);
  data.specularity = correctGammaInput_1_0(sampleTexture2D_3_2(data, specularityMap, triplanarTextureScale).rgb);
  getFresnel_4_6(data);
  data.lightAtten *= getLightDiffuse_5_9(data);
  vec3 lightDiffuse = vec3(0.0);
  lightDiffuse = data.lightAtten * data.albedo * lightColor.rgb;
  data.lightAtten *= getLightSpecular_6_12(data);
  vec3 lightSpecular = vec3(0.0);
  lightSpecular = data.lightAtten * lightColor.rgb;

  data.color = mix(lightDiffuse, lightSpecular, data.specularity);

  getIrradiance(data);
  data.color = data.albedo * data.irradianceColor * (1.0 - data.specularity);

  getReflection(data);
  data.color += data.reflectionColor;

  data.color = tonemapReinhard_8_15(data.color, data.exposure);

  data.color = correctGammaOutput_2_1(data.color);


  gl_FragColor.rgb = data.color;
  gl_FragColor.a = data.opacity;
  }
