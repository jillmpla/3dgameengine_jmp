#include "SceneGraphNode.hpp"

SceneGraphNode::SceneGraphNode() {}

SceneGraphNode::~SceneGraphNode() { 
    Destroy(); }

// release object from memory
    void SceneGraphNode::Release() { 
        delete this; }

// update node
void SceneGraphNode::Update()
{
    // loop through list, update children
    for (std::list<SceneGraphNode*>::iterator i = Child.begin(); i != Child.end(); i++) {
        (*i)->Update();
    }
}

// destroy all children
void SceneGraphNode::Destroy() {
    for (std::list<SceneGraphNode*>::iterator i = Child.begin();
        i != Child.end(); i++)
        (*i)->Release();
        Child.clear();
}

// add a child
void SceneGraphNode::AddChild(SceneGraphNode* Node) {
    Child.push_back(Node);
}

// list children
std::list<SceneGraphNode*> Child;
