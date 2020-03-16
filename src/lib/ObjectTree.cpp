/* OBJECT TREE.hpp
 *   by Lut99
 *
 * Created:
 *   3/15/2020, 5:02:00 PM
 * Last edited:
 *   3/16/2020, 5:09:10 PM
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

#include "ObjectTree.hpp"

using namespace std;
using namespace RayTracer;


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



/***** OBJECT TREE LEAF *****/

ObjectTreeLeaf::ObjectTreeLeaf(const ObjectTreeLeaf& other)
    : obj(other.obj),
    ObjectTreeNode(true)
{}

ObjectTreeLeaf::ObjectTreeLeaf(ObjectTreeLeaf&& other)
    : obj(other.obj),
    ObjectTreeNode(true)
{}



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



size_t ObjectTree::add(RenderObject* obj) {
    this->objects[this->uid] = obj;

    // Be sure to mark the tree as non-optimised anymore
    this->is_optimised = false;

    return this->uid++;
}

bool ObjectTree::remove(RenderObject* obj) {
    // Check if a node with this pointer is present
    for (map<size_t, RenderObject*>::iterator iter = this->objects.begin(); iter != this->objects.end(); ++iter) {
        if (iter->second == obj) {
            // Remove it and then quit
            this->objects.erase(iter);

            this->is_optimised = false;
            
            return true;
        }
    }
    return false;
}

bool ObjectTree::remove(const size_t obj) {
    // Check if a node with this key is present
    map<size_t, RenderObject*>::iterator it;
    it = this->objects.find(obj);
    if (it == this->objects.end()) {
        return false;
    }

    // Remove it
    this->objects.erase(it);

    // Update the optimised flag
    this->is_optimised = false;

    return true;
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