#include "BVHTree.h"

using namespace std;


BVHTree::BVHTree()
{
	root = NULL;
}


BVHTree::~BVHTree()
{
	deleteTree(root);
}

void BVHTree::deleteTree(BVHTreeNode* rootnode)
{
	BVHTreeNode* temp = rootnode;
	if (temp->leftChild != NULL)
	{
		deleteTree(temp->leftChild);
		deleteTree(temp->rightChild);
		delete temp;
	}
	//delete temp;
}

void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
	BVHTreeNode *newNode = new BVHTreeNode(objectArea, name, true);
	map[name] = newNode;		// insert newNode to our dictionary
	names.push_back(name);		// push all the names to a vector to have the names of all leaves

	if (root == NULL)		// if the tree is empty
	{
		root = newNode;
	}
	else if (root->leftChild == NULL)	// if the tree has only 1 node(root)
	{
		AABB newAABB = createParentAABB(root->aabb, newNode->aabb);
		BVHTreeNode* newRoot = new BVHTreeNode(newAABB, "branch", false); //?
		newRoot->leftChild = newNode;
		newRoot->rightChild = root;
		root->parent = newRoot;
		newNode->parent = newRoot;
		root = newRoot;
	}
	else	// tree has multiple nodes
	{
		BVHTreeNode* branchNode = root;
		while (branchNode->leftChild != NULL)		// until reaching a leaf, iterate
		{
			int increaseInRightTreeSize = AABB::unionArea(newNode->aabb,
				branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb,
				branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();
			if (increaseInRightTreeSize < increaseInLeftTreeSize)
			{
				branchNode->aabb = createParentAABB(branchNode->aabb, objectArea);	// update the parent's area with new coming node
				branchNode = branchNode->rightChild;
			}
			else
			{
				branchNode->aabb = createParentAABB(branchNode->aabb, objectArea);
				branchNode = branchNode->leftChild;
			}
		}
		AABB newAABB = createParentAABB(branchNode->aabb, newNode->aabb);
		BVHTreeNode* newBranch = new BVHTreeNode(newAABB, "branch", false); 
		newBranch->leftChild = newNode;
		newBranch->rightChild = branchNode;
		if (branchNode->parent->leftChild == branchNode)
			branchNode->parent->leftChild = newBranch;
		else
			branchNode->parent->rightChild = newBranch;
		newBranch->parent = branchNode->parent;
		branchNode->parent = newBranch;
		newNode->parent = newBranch;
	}
}


void BVHTree::moveBVHMember(std::string name, AABB newLocation)
{
	BVHTreeNode* nodeToBeChanged = map[name];
	BVHTreeNode* parentNode = nodeToBeChanged->parent == NULL ? nodeToBeChanged : nodeToBeChanged->parent;
	
	if (AABB::unionArea(parentNode->aabb, newLocation) == parentNode->aabb.getArea())
		// if new location is in parent node's area, just change the location without any modification
		nodeToBeChanged->aabb = newLocation;
	else
	{
		removeBVHMember(name);
		addBVHMember(newLocation, name);
	}

}


void BVHTree::removeBVHMember(std::string name)
{
	BVHTreeNode* nodeToBeDeleted = map[name];
	BVHTreeNode* sibling = NULL;
	BVHTreeNode* parent = nodeToBeDeleted->parent;
	BVHTreeNode* grandParent = parent != NULL ? parent->parent: NULL;
	
	string side = "";
	int a;
	for (a = 0; names[a] != name; a++) {}
	string temp = names[names.size() - 1];
	names[a] = temp;
	names.pop_back();	// remove the name of the node to be removed

	if (parent == NULL)
	{
		delete root;
		root = NULL;
	}
	else {
		if (parent->leftChild == nodeToBeDeleted)	// To decide which side of grandparent to place the sibling member
		{
			side = "right";
			sibling = parent->rightChild;
		}
		else	//if (parent->rightChild == nodeToBeDeleted)
		{
			side = "left";
			sibling = parent->leftChild;
		}
		if (grandParent == NULL)
		{
			if (side == "right")
				root = root->rightChild;
			else if (side == "left")
				root = root->leftChild;
		}
		else
		{
			if (grandParent->leftChild == parent)	// to see which side of grandparent the parent is
			{
				grandParent->leftChild = sibling;
				sibling->parent = grandParent;
			}
			else if (grandParent->rightChild == parent)
			{
				grandParent->rightChild = sibling;
				sibling->parent = grandParent;
			}
		}
		delete parent;
		delete nodeToBeDeleted;

		BVHTreeNode* x = sibling;
		while (x != root)
		{
			x->parent->aabb = createParentAABB(x->parent->leftChild->aabb, x->parent->rightChild->aabb);
			x = x->parent;
		}
	}
}


vector<string> BVHTree::getCollidingObjects(AABB object)
{
	vector<string> collidingNames;

	for (int a = 0; a < names.size(); a++)
	{
		BVHTreeNode* temp = map[names[a]];

		if ((temp->aabb.minX <= object.maxX && temp->aabb.maxX >= object.maxX) ||
			(temp->aabb.maxX >= object.minX && temp->aabb.minX <= object.minX))
		{
			if ((temp->aabb.minY <= object.maxY && temp->aabb.maxY >= object.maxY) ||
				(temp->aabb.maxY >= object.minY && temp->aabb.minY <= object.minY))
				// if that object's area is covered(partially or fully) by current parent node
				collidingNames.push_back(names[a]);
		}
	}
	return collidingNames;
}


void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}


std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

////////////////////////////////////////////////////////////////////////////
/*--------------------------Free Functions-------------------------------*/
//////////////////////////////////////////////////////////////////////////

int getMaxi(int first, int second)
{
	return (first > second) ? first : second;
}

int getMini(int first, int second)
{
	return (first < second) ? first : second;
}

AABB createParentAABB(AABB first, AABB second)
{
	int minx, miny, maxx, maxy;
	minx = getMini(first.minX, second.minX);
	miny = getMini(first.minY, second.minY);
	maxx = getMaxi(first.maxX, second.maxX);
	maxy = getMaxi(first.maxY, second.maxY);
	return AABB(minx, miny, maxx, maxy);
}
