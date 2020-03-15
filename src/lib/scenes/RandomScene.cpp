/* RANDOM SCENE.cpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 12:12:03 PM
 * Last edited:
 *   07/03/2020, 11:51:00
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RandomScene class is a derived class of a RenderWorld. Instead of
 *   letting the user specify the object themselves, this class generates a
 *   random world upon initialization as specified of the end of the
 *   tutorial. This particular file is the implementation file for
 *   RandomScene.hpp.
**/

#include "Random.hpp"
#include "RandomScene.hpp"

#include "Sphere.hpp"

#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Dielectric.hpp"

using namespace std;
using namespace RayTracer;


RandomScene::RandomScene()
    : RenderWorld()
{
    // Add the objects required
    this->add_object(new Sphere(Vec3(0, -1000, -0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            Vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // Add a mat sphere
                    this->add_object(new Sphere(center, 0.2,
                        new Lambertian(Vec3(random_double() * random_double(),
                                            random_double() * random_double(),
                                            random_double() * random_double()))));
                } else if (choose_mat < 0.95) {
                    // Add a reflecting sphere
                    this->add_object(new Sphere(center, 0.2,
                        new Metal(Vec3(random_double() * random_double(),
                                       random_double() * random_double(),
                                       random_double() * random_double()),
                                  0.5 * random_double())));
                } else {
                    // Add a refracting sphere
                    this->add_object(new Sphere(center, 0.2,
                        new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));
                }
            }
        }
    }

    // Finally, add the three big spheres
    this->add_object(new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));
    this->add_object(new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(1.0, 0.3, 0.0))));
    this->add_object(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.75, 0.75, 0.75), 0.2)));
}