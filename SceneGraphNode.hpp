#include <list>
using namespace std;

class SceneGraphNode {
public:
	SceneGraphNode();
	virtual ~SceneGraphNode();

	void Release();
	virtual void Update();
	void Destroy();
	void AddChild(SceneGraphNode* Node);

protected:
	list<SceneGraphNode*> Child;
};
