/* OBJECT TREE.hpp
 *   by Lut99
 *
 * Created:
 *   3/15/2020, 5:02:00 PM
 * Last edited:
 *   3/16/2020, 4:57:25 PM
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

#ifndef OBJECTTREE_HPP
#define OBJECTTREE_HPP

#include <vector>

#include "RenderObject.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"

namespace RayTracer {
    class ObjectTreeNode {
        protected:
            ObjectTreeNode(bool is_leaf) :is_leaf(is_leaf) {};
        public:
            /* Signifies if the node is a leaf node or not. */
            bool is_leaf;

            /* Overridable deconstructor */
            virtual ~ObjectTreeNode() = 0;

            /* Virtual for the hit function of a node. Returns the RenderObject it hits, or nullptr if nothing is hit. */
            virtual RenderObject* hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const = 0;

            /* Virtual clone function. Note that this allocates a new ObjectTreeNode. */
            virtual ObjectTreeNode* clone() const = 0;
    };

    class ObjectTreeBranch: public ObjectTreeNode {
        private:
            /* The left node in the tree. */
            ObjectTreeNode* left;
            /* The right node in the tree. */
            ObjectTreeNode* right;
        public:
            /* The ObjectTreeBranch is a node that branches into two subtrees. More concretely, it represents a hitbox over multiple RenderObject. */
            ObjectTreeBranch(const ObjectTreeNode& left, const ObjectTreeNode& right);
            /* Copy constructor for ObjectTreeBranch. */
            ObjectTreeBranch(const ObjectTreeBranch& other);
            /* Move constructor for ObjectTreeBranch. */
            ObjectTreeBranch(ObjectTreeBranch&& other);
            /* Deconstructor for the ObjectTreeBranch. */
            ~ObjectTreeBranch();

            /* Returns the hit object in one of the leaf nodes. Returns nullptr if nothing in this node is hit. */
            virtual RenderObject* hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Copy assignment operator for the ObjectTreeBranch. */
            ObjectTreeBranch& operator=(ObjectTreeBranch other);
            /* Move assignment operator for the ObjectTreeBranch. */
            ObjectTreeBranch& operator=(ObjectTreeBranch&& other);
            /* Swap operator for ObjectTreeBranch. */
            friend void swap(ObjectTreeBranch& first, ObjectTreeBranch& second);

            /* Clones this ObjectTreeBranch and allocates a new one. */
            virtual ObjectTreeNode* clone() const;
    };
    
    class ObjectTreeLeaf: public ObjectTreeNode {
        private:
            /* The RenderObject represented by this node. */
            RenderObject* obj;
        public:
            /* The ObjectTreeLeaf is the node that stores the RenderObjects. More concretely, it represents only the hitbox of one RenderObject. */
            ObjectTreeLeaf(RenderObject* obj);
            /* Copy constructor for ObjectTreeLeaf. */
            ObjectTreeLeaf(const ObjectTreeLeaf& other);
            /* Move constructor for ObjectTreeLeaf. */
            ObjectTreeLeaf(ObjectTreeLeaf&& other);
            /* Deconstructor for ObjectTreeLeaf. */
            ~ObjectTreeLeaf();

            /* Returns the pointer to the inner object if it is hit, or nullptr otherwise. */
            virtual RenderObject* hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Copy assignment operator for the ObjectTreeLeaf. */
            ObjectTreeLeaf& operator=(ObjectTreeLeaf other);
            /* Move assignment operator for the ObjectTreeLeaf. */
            ObjectTreeLeaf& operator=(ObjectTreeLeaf&& other);
            /* Swap operator for ObjectTreeLeaf. */
            friend void swap(ObjectTreeLeaf& first, ObjectTreeLeaf& second);

            /* Clones this ObjectTreeBranch and allocates a new one. */
            virtual ObjectTreeNode* clone() const;
    };

    class ObjectTree {
        private:
            /* A linear vector of RenderObject that allow the repeated construction of the tree. */
            std::map<size_t, RenderObject*> objects;

            /* The root of the tree. */
            ObjectTreeNode* root;

            /* Represents if the tree is optimised or not. */
            bool is_optimised;

            /* Keeps track of the uids. */
            size_t uid;
        public:
            /* The ObjectTree provides optimized ray hitting with the objects inside. This is the default constructor. */
            ObjectTree();
            /* Copy constructor for the ObjectTree */
            ObjectTree(const ObjectTree& other);
            /* Move constructor for the ObjectTree */
            ObjectTree(ObjectTree&& other);
            /* Deconstructor for the ObjectTree. */
            ~ObjectTree();

            /* Add a new object to the tree (in a dumb way). The return value is a unique ID of that RenderObject. */
            size_t add(RenderObject* obj);
            /* Remove a RenderObject. Returns true if succesfull, or false if not (if the obj doesn't exist, for example). This overload finds the object by pointer. */
            bool remove(RenderObject* obj);
            /* Remove a RenderObject. Returns true if succesfull, or false if not (if the obj doesn't exist, for example). This overload finds the object by uid. */
            bool remove(const size_t obj);

            /* Optimize the current tree. */
            void optimize();

            /* Hit a ray with the tree, and return a reference to the closest hit. Returns nullptr upon no hit. Note that it performs a dready loop-traversal if the tree is not optimised. */
            RenderObject* hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Copy assignment operator for the ObjectTreeLeaf. */
            ObjectTree& operator=(ObjectTree other);
            /* Move assignment operator for the ObjectTreeLeaf. */
            ObjectTree& operator=(ObjectTree&& other);

            /* Returns the number of elements in the tree. */
            size_t size() const;
    };

    /* Swap operator for ObjectTreeBranch. */
    void swap(ObjectTreeBranch& first, ObjectTreeBranch& second);
    /* Swap operator for ObjectTreeLeaf. */
    void swap(ObjectTreeLeaf& first, ObjectTreeLeaf& second);
}

#endif