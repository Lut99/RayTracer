/* OBJECT TREE.hpp
 *   by Lut99
 *
 * Created:
 *   3/15/2020, 5:02:00 PM
 * Last edited:
 *   11/04/2020, 18:01:25
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ObjectTree class is meant to store a list of objects in an
 *   efficiently-searchable way. To facilitate moving spheres, the adding
 *   of the spheres is done in a dumb way, but with optimize() they are
 *   shuffled in the most optimal way.
 *   The most optimal way here is defined as //TBD
**/

#include <limits>
#include <iostream>
#include <algorithm>

#include <iostream>

#include "ObjectTree.hpp"
#include "BoundingBox.hpp"

using namespace std;
using namespace RayTracer;


void print_vec(string name, const vector<RenderObject*>& vec) {
    cout << name << " {";
    for (size_t i = 0; i < vec.size(); i++) {
        if (i > 0) { cout << ", "; }
        cout << vec.at(i);
    }
    cout << "}" << endl;
}

void print_bbox(string name, const BoundingBox& bbox) {
    cout << name << " has size " << bbox.size() << endl;
}


/***** TOOLS *****/

/* Sorting function for the x-axis. */
bool sort_x(RenderObject* obj_1, RenderObject* obj_2) { return obj_1->center.x < obj_2->center.x; }
/* Sorting function for the y-axis. */
bool sort_y(RenderObject* obj_1, RenderObject* obj_2) { return obj_1->center.y < obj_2->center.y; }
/* Sorting function for the z-axis. */
bool sort_z(RenderObject* obj_1, RenderObject* obj_2) { return obj_1->center.z < obj_2->center.z; }

/* Clusters a given list of RenderObjects into two new lists in such a way that each list is a cluster of objects that are closest to each other. */
void cluster(const vector<RenderObject*>& total, vector<RenderObject*>& left, vector<RenderObject*>& right) {
    // Nothing to be done if empty
    if (total.size() == 0) { return; }

    // Sort according to the three axis'
    vector<RenderObject*> ordered_x(total);
    vector<RenderObject*> ordered_y(total);
    vector<RenderObject*> ordered_z(total);
    sort(ordered_x.begin(), ordered_x.end(), sort_x);
    sort(ordered_y.begin(), ordered_y.end(), sort_y);
    sort(ordered_z.begin(), ordered_z.end(), sort_z);

    // For each of them, compute the largest boundingbox sizes
    size_t const half_size = total.size() / 2;
    BoundingBox bx1, bx2, by1, by2, bz1, bz2;
    for (size_t i = 0; i < half_size; i++) {
        bx1.merge(ordered_x.at(i)->box);
        by1.merge(ordered_y.at(i)->box);
        bz1.merge(ordered_z.at(i)->box);
    }
    for (size_t i = half_size; i < total.size(); i++) {
        bx2.merge(ordered_x.at(i)->box);
        by2.merge(ordered_y.at(i)->box);
        bz2.merge(ordered_z.at(i)->box);
    }

    // Set the splits with the smallest total BB
    double sx = bx1.size() + bx2.size();
    double sy = by1.size() + by2.size();
    double sz = bz1.size() + bz2.size();
    if (sx <= sy && sx <= sz) {
        // x is smallest
        vector<RenderObject*>::iterator halfway = ordered_x.begin() + half_size;
        left = vector<RenderObject*>(ordered_x.begin(), halfway);
        right = vector<RenderObject*>(halfway, ordered_x.end());
    } else if (sy <= sx && sy <= sz) {
        // y is smallest
        vector<RenderObject*>::iterator halfway = ordered_y.begin() + half_size;
        left = vector<RenderObject*>(ordered_y.begin(), halfway);
        right = vector<RenderObject*>(halfway, ordered_y.end());
    } else {
        // z is smallest
        vector<RenderObject*>::iterator halfway = ordered_z.begin() + half_size;
        left = vector<RenderObject*>(ordered_z.begin(), halfway);
        right = vector<RenderObject*>(halfway, ordered_z.end());
    }
}



/***** OBJECT TREE BRANCH *****/

ObjectTreeBranch::ObjectTreeBranch(const std::vector<RenderObject*> elements)
    : ObjectTreeNode(false, BoundingBox())
{
    // First, construct the nodes by assigning them half each cluster
    std::vector<RenderObject*> left;
    std::vector<RenderObject*> right;
    cluster(elements, left, right);
    
    // Construct leaves instead if only one element is left
    if (left.size() == 1) {
        this->left = (ObjectTreeNode*) new ObjectTreeLeaf(left.at(0));
    } else {
        this->left = (ObjectTreeNode*) new ObjectTreeBranch(left);
    }
    if (right.size() == 1) {
        this->right = (ObjectTreeNode*) new ObjectTreeLeaf(right.at(0));
    } else {
        this->right = (ObjectTreeNode*) new ObjectTreeBranch(right);
    }

    // Update the bounding box to be a merge of those
    this->box.merge(this->left->box);
    this->box.merge(this->right->box);
}

ObjectTreeBranch::ObjectTreeBranch(const ObjectTreeBranch& other)
    : ObjectTreeNode(false, other.box),
    left(other.left->clone()),
    right(other.right->clone())
{}

ObjectTreeBranch::ObjectTreeBranch(ObjectTreeBranch&& other)
    : ObjectTreeNode(false, other.box),
    left(other.left),
    right(other.right)
{
    // Fill the other's nodes with nullptr
    other.left = nullptr;
    other.right = nullptr;
}

ObjectTreeBranch::~ObjectTreeBranch() {
    // Delete the two children nodes
    if (this->left != nullptr) { delete this->left; }
    if (this->right != nullptr) { delete this->right; }
}



bool ObjectTreeBranch::quick_hit(const Ray& ray, double t_min, double t_max) const {
    // Only check if the BB is hit
    return this->box.hit(ray, t_min, t_max);
}

bool ObjectTreeBranch::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    HitRecord record_r;
    bool hit_left = this->left->quick_hit(ray, t_min, t_max) && this->left->hit(ray, t_min, t_max, record);
    bool hit_right = this->right->quick_hit(ray, t_min, t_max) && this->right->hit(ray, t_min, t_max, record_r);
    // Note the record if it was a hit
    if (hit_left && hit_right) {
        // Set record to the smallest one
        if (record_r.t < record.t) {
            record = record_r;
        }
    }
    
    // Return if we actually hit
    return hit_left || hit_right;
}



ObjectTreeBranch& ObjectTreeBranch::operator=(ObjectTreeBranch other) {
    // Copy the stuff by swapping
    if (this != &other) {
        // Copy by swapping
        swap(*this, other);
    }
    return *this;
}
ObjectTreeBranch& ObjectTreeBranch::operator=(ObjectTreeBranch&& other) {
    // Copy the stuff directly
    if (this != &other) {
        this->left = other.left;
        this->right = other.right;
        other.left = nullptr;
        other.right = nullptr;
    }
    return *this;
}



ObjectTreeNode* ObjectTreeBranch::clone() const {
    // Allocate a new tree leaf
    return (ObjectTreeNode*) (new ObjectTreeBranch(*this));
}



/***** OBJECT TREE LEAF *****/

ObjectTreeLeaf::ObjectTreeLeaf(RenderObject* obj)
    : ObjectTreeNode(true, obj->box),
    obj(obj)
{}

ObjectTreeLeaf::ObjectTreeLeaf(const ObjectTreeLeaf& other)
    : ObjectTreeNode(true, obj->box),
    obj(other.obj)
{}

ObjectTreeLeaf::ObjectTreeLeaf(ObjectTreeLeaf&& other)
    : ObjectTreeNode(true, obj->box),
    obj(other.obj)
{}

ObjectTreeLeaf::~ObjectTreeLeaf() {
    // Nothing
}


bool ObjectTreeLeaf::quick_hit(const Ray& ray, double t_min, double t_max) const {
    // Only check if the BB is hit
    return this->obj->quick_hit(ray, t_min, t_max);
}

bool ObjectTreeLeaf::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    // Check if the internal object has been quick hit and, if it has, store it
    return this->obj->hit(ray, t_min, t_max, record);
}



ObjectTreeLeaf& ObjectTreeLeaf::operator=(ObjectTreeLeaf other) {
    // Copy the stuff by swapping
    if (this != &other) {
        // Copy by swapping
        swap(*this, other);
    }
    return *this;
}
ObjectTreeLeaf& ObjectTreeLeaf::operator=(ObjectTreeLeaf&& other) {
    // Copy the stuff directly
    if (this != &other) {
        this->obj = other.obj;
        other.obj = nullptr;
    }
    return *this;
}



ObjectTreeNode* ObjectTreeLeaf::clone() const {
    // Allocate a new tree leaf
    return (ObjectTreeNode*) (new ObjectTreeLeaf(*this));
}


/***** OBJECT TREE *****/

ObjectTree::ObjectTree()
    : root(nullptr),
    is_optimised(false),
    uid(0)
{}

ObjectTree::ObjectTree(const ObjectTree& other)
    : objects(other.objects),
    root(other.root),
    is_optimised(other.is_optimised),
    uid(other.uid)
{}

ObjectTree::ObjectTree(ObjectTree&& other)
    : objects(std::move(other.objects)),
    root(std::move(other.root)),
    is_optimised(other.is_optimised),
    uid(other.uid)
{}

ObjectTree::~ObjectTree() {
    if (this->root != nullptr) { delete this->root; }

    // ALso delete all objects
    for (size_t i = 0; i < this->objects.size(); i++) {
        delete this->objects.at(i);
    }
}



bool ObjectTree::add(RenderObject* obj) {
    // Crash if the object has no hitbox
    if (!obj->has_hitbox) {
        return false;
    }

    this->objects.push_back(obj);   

    // Be sure to mark the tree as non-optimised anymore
    this->is_optimised = false;

    return true;
}

bool ObjectTree::remove(RenderObject* obj) {
    // Check if a node with this pointer is present
    for (size_t i = 0; i < this->objects.size(); i++) {
        if (this->objects.at(i) == obj) {
            // Remove it and then quit
            this->objects.erase(this->objects.begin() + i);

            this->is_optimised = false;
            
            return true;
        }
    }
    return false;
}



void ObjectTree::optimize() {
    // Make sure there are things to optimize
    if (this->size() == 0 || this->is_optimised) { return; }

    // Start by removing any existing trees
    if (this->root != nullptr) {
        delete this->root;
    }

    // Make sure there are things to optimize
    if (this->size() == 0) { return; }

    if (this->size() == 1) {
        // If there is only one element, make a leaf
        this->root = (ObjectTreeNode*) new ObjectTreeLeaf(this->objects.at(0));
    } else {
        // Else, make a branch which splits itself
        this->root = (ObjectTreeNode*) new ObjectTreeBranch(this->objects);
    }

    this->is_optimised = true;
}



bool ObjectTree::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    // Do different things depending if we're optimised or not
    if (this->is_optimised) {
        // Simply use the tree structure
        return this->root->hit(ray, t_min, t_max, record);
    }

    cerr << "WARNING: ObjectTree @ " << this << " is not optimised." << endl;

    // Otherwise, do it loop-wise
    double t_best = t_max;
    bool hit = false;
    for (size_t i = 0; i < this->objects.size(); i++) {
        // Fetch the correct item from the list
        RenderObject* obj = this->objects.at(i);
        if (obj->quick_hit(ray, t_min, t_best) && obj->hit(ray, t_min, t_best, record)) {
            t_best = record.t;
            hit = true;
        }
    }

    // Return if we hit or not
    return hit;
}



ObjectTree& ObjectTree::operator=(ObjectTree other) {
    if (this != &other) {
        // Copy by swapping
        swap(*this, other);
    }
    return *this;
}

ObjectTree& ObjectTree::operator=(ObjectTree&& other) {
    if (this != &other) {
        // Steal the elements
        this->objects = std::move(other.objects);
        this->root = std::move(other.root);
        this->is_optimised = other.is_optimised;
        this->uid = other.uid;

        other.objects = vector<RenderObject*>();
        other.root = nullptr;
    }
    return *this;
}



RenderObject* ObjectTree::operator[](size_t index) {
    if (index >= this->objects.size()) {
        throw std::out_of_range(string("Index ") + to_string(index) + string(" is out of range for ObjectTree with size ") + to_string(this->objects.size()));
    }
    return this->objects.at(index);
}



size_t ObjectTree::size() const {
    return this->objects.size();
}



/***** SWAP OPERATORS *****/

void RayTracer::swap(ObjectTreeBranch& first, ObjectTreeBranch& second) {
    using std::swap;

    swap(first.left, second.left);
    swap(first.right, second.right);
}
void RayTracer::swap(ObjectTreeLeaf& first, ObjectTreeLeaf& second) {
    using std::swap;

    swap(first.obj, second.obj);
}
void RayTracer::swap(ObjectTree& first, ObjectTree& second) {
    using std::swap;

    swap(first.objects, second.objects);
    swap(first.root, second.root);
    swap(first.is_optimised, second.is_optimised);
    swap(first.uid, second.uid);
}