#include "Light.h"

using namespace std;
using namespace glm;

Light *Light::mainLight;

Light::Light(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular) : ambient(ambient), diffuse(diffuse), specular(specular) {

}

Light::~Light() {

}
