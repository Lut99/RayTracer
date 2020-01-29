/* DIELECTRIC.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:14:41 PM
 * Last edited:
 *   1/29/2020, 7:16:29 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Dielectric material describes a material that light can pass
 *   through. Not only that, it passes through while refracting, and
 *   sometimes even reflects it (like glass or water). This particular file
 *   is the implementaton file for Dielectric.hpp.
**/

#include "../include/Random.hpp"
#include "../include/materials/Dielectric.hpp"

using namespace std;
using namespace RayTracer;


Dielectric::Dielectric(const Vec3& colour_absorption, double refrective_index)
    : Material(dielectric),
    albedo(colour_absorption),
    ref_idx(refrective_index)
{}

bool Dielectric::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 outward_normal;
    double ni_over_nt;
    attenuation = Vec3(1.0, 1.0, 1.0);//this->albedo;
    Vec3 refracted;

    double reflect_prob;
    double cosine;

    // I think this decides on what side of the sphere we hit; in to out or out to in.
    /*
    if (dot(ray_in.direction, record.normal) > 0) {
        outward_normal = -record.normal;
        ni_over_nt = this->ref_idx;
        cosine = this->ref_idx * dot(ray_in.direction, record.normal) / ray_in.direction.length();
    } else {
        outward_normal = record.normal;
        ni_over_nt = 1.0 / this->ref_idx;
        cosine = -dot(ray_in.direction, record.normal) / ray_in.direction.length();
    }
    */
   if (dot(ray_in.direction, record.normal) > 0) {
       outward_normal = -record.normal;
       ni_over_nt = ref_idx;
   } else {
       outward_normal = record.normal;
       ni_over_nt = 1.0 / ref_idx;
   }

    /*
    if (this->refract(ray_in.direction, outward_normal, ni_over_nt, refracted)) {
        reflect_prob = this->schlick(cosine);
    } else {
        reflect_prob = 1.0;
    }

    if (random_double() < reflect_prob) {
        ray_out = Ray(record.hitpoint, this->reflect(ray_in.direction, record.normal));
    } else {
        ray_out = Ray(record.hitpoint, refracted);
    }
    */
   if (this->refract(ray_in.direction, outward_normal, ni_over_nt, refracted)) {
       ray_out = Ray(record.hitpoint, refracted);
   } else {
       //ray_out = Ray(record.hitpoint, this->reflect(ray_in.direction, record.normal));
       ray_out = ray_in;
       return false;
   }

    return true;
}

Vec3 Dielectric::reflect(const Vec3& v, const Vec3& n) const {
    return v - 2 * dot(n, v) * n;
}

bool Dielectric::refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted) const {
    Vec3 v_normalized = v.normalize();
    double dt = dot(v_normalized, n);
    double D = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    // Only refract if there is a real solution to Snell's law
    if (D > 0) {
        refracted = ni_over_nt * (v_normalized - n * dt) - n * sqrt(D);
        return true;
    }
    return false;
}

double Dielectric::schlick(double cosine) const {
    double r0 = (1 - this->ref_idx) / (1 + this->ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}