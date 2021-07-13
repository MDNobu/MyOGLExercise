#version 330 core
out vec4 FragColor;
  
struct Material
{
    sampler2D baseColorMap;
    sampler2D specularMap;
    sampler2D emissiveMap;
    float shineness;
};

struct LightStruct
{
    vec3 PositionWS; //Point light only
    float FalloffStart; //Point and Spot light
    vec3 Color; //all
    float FalloffEnd; //Point and Spot light
    vec3 DirectionWS;  //DirectionalLight and SpotLight Only
    float SpotPower;   // SpotLight only
    float Intensity; // all

    int LightType; // 这里用这个实现分支，0是方向光， 1是点光源， 2是spotlight， 应该有更好的实现，暂时这样   
};

uniform vec3 objectColor;

uniform float ambientIntensity;
uniform vec3 uLightPosWS;
uniform float shineness;
uniform vec3 lightColor;

uniform Material cubeMaterial;
uniform LightStruct lightObj;

in vec3 aNormalWS;
in vec3 aPosWS;
in vec2 aUV;

float rgb2brightness(vec3 color)
{
    //Gray = R*0.299 + G*0.587 + B*0.114
    float brightness = dot(color, vec3(0.299, 0.587, 0.114));
    return brightness;
}

// 注意要求  End > start
float CalAttenuationByDistance(float distance, in float falloffStart, in float falloffEnd)
{
    float res = (distance - falloffStart) / (falloffEnd - falloffStart);
    
    res = clamp(res, 0.0, 1.0);
    res = 1.0 - res;
    return res;
}

void CalDirectionalLightStrength(in LightStruct curLight,in vec3 curPosWS, 
    out float strength, out vec3 direction)
{
    direction = normalize(-curLight.DirectionWS);
    strength = curLight.Intensity;
}

void  CalPointLightStrength(in LightStruct curLight,in vec3 curPosWS,
    out float strength, out vec3 direction)
{
    direction = curLight.PositionWS - curPosWS;
    float distance = length(direction);
    direction = normalize(direction);
    strength = CalAttenuationByDistance(distance, curLight.FalloffStart, curLight.FalloffEnd);
}

void CalSpotLightStrength(in LightStruct curLight,in vec3 curPosWS,
    out float strength, out vec3 direction)
{
    direction = curLight.PositionWS - curPosWS;
    float distance = length(direction);
    direction = normalize(direction);
    strength = CalAttenuationByDistance(distance, curLight.FalloffStart, curLight.FalloffEnd);

    vec3 lightPropogateDir = normalize(curLight.DirectionWS);
    //strength *= pow(clamp(dot(lightPropogateDir, -direction), 0.0, 1.0 ),  curLight.SpotPower);
    strength = clamp(dot(lightPropogateDir, -direction), 0.0, 1.0 );
}



void main()
{
    vec3 baseColor =  texture(cubeMaterial.baseColorMap, aUV).rgb;

    vec3 normalWS = normalize(aNormalWS);
    vec3 lightDirWS;
    float lightStrength = 0.0f;
    do
    {
        if(0 == lightObj.LightType){
            CalDirectionalLightStrength(lightObj, aPosWS, lightStrength, lightDirWS );
            break;
        }
        if(1 == lightObj.LightType){
            CalPointLightStrength(lightObj, aPosWS,lightStrength, lightDirWS);
            break;
        }
        if(2 == lightObj.LightType){
            CalSpotLightStrength(lightObj, aPosWS,lightStrength, lightDirWS);
            break;
        }
    } while(false);
    

    float ambientFactor = ambientIntensity * 0.05;
    vec3 reflectWS = reflect(-lightDirWS, normalWS);
    float diffuseFactor = max(dot(normalWS, lightDirWS), 0.0) ;
    float specStrength = 5.0f;
    // float specularFactor = pow(max(dot(normalWS,reflectWS), 0.0), shineness);
    specStrength *= ( 1.0 - texture(cubeMaterial.specularMap, aUV).r);
    float specularFactor = specStrength * pow(max(dot(normalWS,reflectWS), 0.0), cubeMaterial.shineness);
    // float specularFactor = specStrength * pow(max(dot(normalWS,reflectWS), 0.0), 2.0);


    //float lightness = ambientFactor  + diffuseFactor * lightStrength + specularFactor * lightStrength;
    //lightness = max(dot(normalWS,reflectWS), 0.0);
    //lightness = clamp(lightness, 0.0, 1.0);
    //baseColor = objectColor;
    // vec3 resColor = ambientFactor * baseColor + 
        // (diffuseFactor + specularFactor) * lightStrength * (lightColor * baseColor);

    vec3 resColor = ambientFactor * baseColor + 
        (diffuseFactor + specularFactor) * lightStrength * (lightObj.Color * baseColor);
    // 注意 这里emissive的实现不是基于物理的，正常的应该是将emissive和nonemissive相加
    //  这里是为了强调emissive效果
    // emisive col
    vec3 emissiveMap = texture(cubeMaterial.emissiveMap, aUV).rgb;
    const float emissiveStrngth = 2.0;
    // 用emissive 的亮度判断当前是否用emissive
    float emissiveFactor = step(0.1, rgb2brightness(emissiveMap));
    //resColor = mix(resColor, emissiveStrngth * emissiveMap, emissiveFactor);

   // FragColor = lightness * vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(resColor, 1.0);



    float test = lightStrength;
    // vec3 tmp = lightColor;
    //FragColor = vec4(vec3(diffuseFactor), 1.0) ;
}