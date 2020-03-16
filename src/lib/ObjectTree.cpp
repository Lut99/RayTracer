/* OBJECT TREE.hpp
 *   by Lut99
 *
 * Created:
 *   3/15/2020, 5:02:00 PM
 * Last edited:
 *   3/16/2020, 6:05:22 PM
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

#include <map>
#include <limits>
#include <iostream>

#include "ObjectTree.hpp"

using namespace std;
using namespace RayTracer;


/***** TOOLS *****/

/* Clusters a given list of RenderObjects into two new lists in such a way that each list is a cluster of objects that are closest to each other. */
void cluster(const vector<RenderObject*>& total, vector<RenderObject*>& left, vector<RenderObject*> right) {
    
}



/***** OBJECT TREE BRANCH *****/

ObjectTreeBranch::ObjectTreeBranch(const ObjectTreeBranch& other)
    : left(other.left->clone()),
    right(other.right->clone()),
    ObjectTreeNode(false)
{}

ObjectTreeBranch::ObjectTreeBranch(ObjectTreeBranch&& other)
    : left(other.left),
    right(other.right),
    ObjectTreeNode(false)
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



bool ObjectTreeBranch::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    // Check if it hits in either, and use shortcircuit boolean logic to make
    //   sure we get the correct one
    return this->left->hit(ray, t_min, t_max, record) || this->right->hit(ray, t_min, t_max, record);
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

ObjectTreeLeaf::ObjectTreeLeaf(const ObjectTreeLeaf& other)
    : obj(other.obj),
    ObjectTreeNode(true)
{}

ObjectTreeLeaf::ObjectTreeLeaf(ObjectTreeLeaf&& other)
    : obj(other.obj),
    ObjectTreeNode(true)
{}

ObjectTreeLeaf::~ObjectTreeLeaf() {
    // Nothing
}



bool ObjectTreeLeaf::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    // Check if the internal object has been quick hit and, if it has, store it
    return this->obj->quick_hit(ray, t_min, t_max) && this->obj->hit(ray, t_min, t_max, record);
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
}



size_t ObjectTree::add(RenderObject* obj) {
    this->objects[this->uid] = obj;

    // Be sure to mark the tree as non-optimised anymore
    this->is_optimised = false;

    return this->uid++;
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
    // Start by removing any existing trees
    if (this->root != nullptr) {
        delete this->root;
    }

    // We do this by clustering our list of elements into two and then letting
    //   the ObjectTreeBranch do the rest
    vector<RenderObject*> left;
    vector<RenderObject*> right;
    cluster(this->objects, left, right);
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