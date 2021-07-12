#version 330 core
out vec4 FragColor;
  
struct Material
{
    sampler2D baseColorMap;
    sampler2D specularMap;
    sampler2D emissiveMap;
    float shineness;
};

uniform vec3 objectColor;

uniform float ambientIntensity;
uniform vec3 uLightPosWS;
uniform float shineness;
uniform vec3 lightColor;

uniform Material cubeMaterial;

float rgb2brightness(vec3 color)
{
    //Gray = R*0.299 + G*0.587 + B*0.114
    float brightness = dot(color, vec3(0.299, 0.587, 0.114));
    return brightness;
}

in vec3 aNormalWS;
in vec3 aPosWS;
in vec2 aUV;
void main()
{
    vec3 baseColor =  texture(cubeMaterial.baseColorMap, aUV).rgb;
    

    vec3 normalWS = normalize(aNormalWS);
    vec3 lightDirWS = normalize(uLightPosWS - aPosWS);

    float ambientFactor = ambientIntensity * 0.05;
    vec3 reflectWS = reflect(-lightDirWS, normalWS);
    float diffuseFactor = max(dot(normalWS, lightDirWS), 0.0);
    float specStrength = 5.0f;
    // float specularFactor = pow(max(dot(normalWS,reflectWS), 0.0), shineness);
    specStrength *= ( 1.0 - texture(cubeMaterial.specularMap, aUV).r);
    float specularFactor = specStrength * pow(max(dot(normalWS,reflectWS), 0.0), cubeMaterial.shineness);
    // float specularFactor = specStrength * pow(max(dot(normalWS,reflectWS), 0.0), 2.0);


    //specularFactor *= diffuseFactor;


    float lightness = ambientFactor  + diffuseFactor + specularFactor;
    lightness = max(dot(normalWS,reflectWS), 0.0);
    //lightness = clamp(lightness, 0.0, 1.0);
    //baseColor = objectColor;
    vec3 resColor = ambientFactor * baseColor + 
        (diffuseFactor + specularFactor) * (lightColor * baseColor);


    // 注意 这里emissive的实现不是基于物理的，正常的应该是将emissive和nonemissive相加
    //  这里是为了强调emissive效果
    // emisive colors
    vec3 emissiveMap = texture(cubeMaterial.emissiveMap, aUV).rgb;
    const float emissiveStrngth = 2.0;
    // 用emissive 的亮度判断当前是否用emissive
    float emissiveFactor = step(0.1, rgb2brightness(emissiveMap));
    resColor = mix(resColor, emissiveStrngth * emissiveMap, emissiveFactor);

   // FragColor = lightness * vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(resColor, 1.0);

    // vec3 test = texture(cubeMaterial.specularMap, aUV).rgb;
    //FragColor = vec4(test, 1.0);
    //FragColor = vec4(lightness, lightness, lightness, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0);
}