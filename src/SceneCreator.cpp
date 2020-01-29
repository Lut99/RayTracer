/* SCENE CREATOR.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 4:36:39 PM
 * Last edited:
 *   1/29/2020, 4:53:17 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The SceneCreator file is not meant to render, but rather to easily
 *   write RenderWorlds that have been created in code to a JSON file. Note
 *   that this is rather inefficient, as this has to be recompiled
 *   everytime a new scene has to be written.
**/

#include "lib/include/cxxopts.hpp"
#include "lib/include/json.hpp"

#include "lib/include/RenderObject.hpp"
#include "lib/include/Sphere.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;
using namespace nlohmann;


class SceneWriter {
    private:
        vector<RenderObject*> objects;
        vector<int*> lights;
    public:
        /* The SceneWriter object is meant to write a scene specification to a JSON file. The scene can be constructed as if it's a RenderWorld, except that it isn't. */
        SceneWriter();
        ~SceneWriter() {
            // Undeclare everything in the vectors
            for (std::size_t i = 0; i < this->objects.size(); i++) {
                delete this->objects.at(i);
            }
            for (std::size_t i = 0; i < this->lights.size(); i++) {
                delete this->lights.at(i);
            }
        }

        /* Adds an object to the SceneWriter. Note that this object will be copied. */
        void add_object(RenderObject obj) {
            this->objects.push_back(new RenderObject(obj));
        }
        /* Adds an object to the SceneWriter. Note that the object given will be deallocated automatically. */
        void add_object(RenderObject* obj) {
            this->objects.push_back(obj);
        }

        /* Adds a light object to the SceneWriter. Note that this object will be copied. */
        void add_light(int light) {

        }
        /* Adds a light object to the SceneWriter. Note that the object given will be deallocated automatically. */
        void add_light(int* light) {
            
        }

        /* Writes the given scene to a JSON file using the JSON library. */
        void write(string file_name) {
            json j;
            json& sj = j["scene"];
            
            /* Add all objects */
            json& oj = sj["objects"];
            for (size_t i = 0; i < this->objects.size(); i++) {
                
            }
        }
};



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
        json_file = "bin/linux/scenes/default.json";
    } catch (OptionParseException& opt) {
        cerr << "Could not parse JSON file path: " << opt.what() << endl;
        exit(-1);
    }

    cout << endl << "*** SceneCreator for Lut99's RayTracer ***" << endl << endl;

    cout << "Using options:" << endl;
    cout << "  Output file: " << json_file << endl;

    cout << endl << "Creating SceneWriter object..." << endl;
    SceneWriter scene;

    cout << "Appending objects..." << endl;

    /* APPEND FROM HERE */

    scene.add_object(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5))));
    scene.add_object(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
    scene.add_object(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0)));
    scene.add_object(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));

    /* UNTIL HERE */

    cout << "Saving to JSON file..." << endl;
    scene.write(json_file);

    cout << endl << "Done." << endl << endl;

    return 0;
}