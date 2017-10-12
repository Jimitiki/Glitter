#version 330 core
in vec4 position;
in vec3 normal;
in vec2 uv;
in vec3 rawPosition;

uniform int phongExponent;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform vec3 lightAmbientColor;

uniform vec3 cameraPosition;

out vec4 fColor;

void ambient(in vec3 lightDiffuse, in vec3 lightAmbient, out vec3 colorAmbient);
void diffuse(in vec3 objDiffuse, in vec3 lightDiffuse, in vec3 normal, in vec3 lightDirection, out vec3 colorDiffuse);
void specular(in vec3 objSpecular, in vec3 viewDirection, in vec3 normal, in vec3 lightDirection, in int phongExponent, out vec3 colorSpecular);

void main()
{
	vec3 cAmbient, cDiffuse, cSpecular, objPosition, objectToViewDirection, objectToLightDirection, lightToObjectDirection;
	
	objPosition = rawPosition;//vec3(position.x, position.y, position.z);
	objectToViewDirection = normalize(cameraPosition - objPosition);
	objectToLightDirection = normalize(lightPosition - objPosition);
	lightToObjectDirection = normalize(objPosition - lightPosition);

	ambient(lightDiffuseColor, lightAmbientColor, cAmbient);
	diffuse(diffuseColor, lightDiffuseColor, normal, objectToLightDirection, cDiffuse);
	specular(specularColor, objectToViewDirection, normal, lightToObjectDirection, phongExponent, cSpecular);

	fColor = vec4(cDiffuse + cSpecular + cAmbient, 1.0f);
}

void ambient(in vec3 lightDiffuse, in vec3 lightAmbient, out vec3 colorAmbient)
{
	colorAmbient = vec3(
			lightDiffuse.x * lightAmbient.x,
			lightDiffuse.y * lightAmbient.y,
			lightDiffuse.z * lightAmbient.z
	);
}

void diffuse(in vec3 objDiffuse, in vec3 lightDiffuse, in vec3 normal, in vec3 lightDirection, out vec3 colorDiffuse)
{
	float orientation = max(0, dot(normal, lightDirection));
	colorDiffuse = lightDiffuse;
	colorDiffuse = vec3(
			objDiffuse.x * lightDiffuse.x * orientation,
			objDiffuse.y * lightDiffuse.y * orientation,
			objDiffuse.z * lightDiffuse.z * orientation
	);
}

void specular(in vec3 objSpecular, in vec3 viewDirection, in vec3 normal, in vec3 lightDirection, in int phongExponent, out vec3 colorSpecular)
{
	vec3 reflection = normalize(reflect(lightDirection, normal));
	float orientation = max(0, dot(viewDirection, reflection));
	float specularStrength = pow(orientation, phongExponent);
	colorSpecular = objSpecular * specularStrength;
}