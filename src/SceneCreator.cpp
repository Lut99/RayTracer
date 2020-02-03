/* SCENE CREATOR.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 4:36:39 PM
 * Last edited:
 *   2/3/2020, 5:34:11 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The SceneCreator file is not meant to render, but rather to easily
 *   write RenderWorlds that have been created in code to a JSON file. Note
 *   that this is rather inefficient, as this has to be recompiled
 *   everytime a new scene has to be written.
**/

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>

#include "lib/include/cxxopts.hpp"
#include "lib/include/json.hpp"

#include "lib/include/RenderWorld.hpp"

#include "lib/include/objects/Sphere.hpp"
#include "lib/include/objects/RenderObjectCollection.hpp"

#include "lib/include/materials/Lambertian.hpp"
#include "lib/include/materials/Metal.hpp"
#include "lib/include/materials/Dielectric.hpp"

#include "lib/include/WorldIO.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;
using namespace nlohmann;



int main(int argc, char** argv) {
    string json_file;

    Options arguments("RayTracer", "Renders images using a custom-written RayTracer.");
    arguments.add_options()
        ("f,filename", "The name of the output JSON file (relative to the run directory)", value<string>())
        ;
    
    auto result = arguments.parse(argc, argv);

    try {
        json_file = result["filename"].as<string>();
    } catch (domain_error& opt) {
        json_file = "scenes/default.json";
    } catch (OptionParseException& opt) {
        cerr << "Could not parse JSON file path: " << opt.what() << endl;
        exit(-1);
    }

    cout << endl << "*** SceneCreator for Lut99's RayTracer ***" << endl << endl;

    cout << "Using options:" << endl;
    cout << "  Output file: " << json_file << endl;

    cout << endl << "Creating World object..." << endl;
    RenderWorld world;

    cout << "Appending objects..." << endl;

    /* APPEND FROM HERE */

    world.add_object(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5))));
    world.add_object(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
    world.add_object(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0)));
    world.add_object(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));

    /* UNTIL HERE */

    cout << "Encoding to JSON..." << endl;
    json j = WorldIO::to_json(world);

    /* ENCODE CAMERA */
    j["camera"] = WorldIO::to_json(Camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, 0));
    
    cout << "Writing to file..." << endl;
    ofstream out(json_file);
    if (!out.is_open()) {
        cerr << "Could not open file: " << strerror(errno) << endl;
        exit(-1);
    }
    out << j.dump(4) << endl;
    out.close();

    cout << endl << "Done." << endl << endl;

    return 0;
}