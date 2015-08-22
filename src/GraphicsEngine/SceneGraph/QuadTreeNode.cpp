#include "QuadTreeNode.h"

#include "../RenderQueue.h"
#include "CameraSceneNode.h"

ShiftEngine::QuadTreeNode::QuadTreeNode(float x1, float x2, float y1, float y2)
    : ISceneNode()
    , bbox({ x1, y1, std::numeric_limits<float>::lowest() }, { x2, y2, std::numeric_limits<float>::max() })
{
    for (auto & elem : subtrees)
        elem = nullptr;
}

ShiftEngine::QuadTreeNode::~QuadTreeNode()
{
    for (auto elem : subtrees)
        delete elem;
}

void ShiftEngine::QuadTreeNode::AddChild(ISceneNode * node)
{
    // check that child can be added in this node
    // if not -> return false

    if (!node)
        return;

    if (!AddNode(node) && !GetParent())
    {
        //UNDONE: CURRENTLY GET PARENT WILL NOT WORK FOR QUAD TREE NODES

        //this is root node
        //must be resized to fit new node
        //2 strategies:
        //1 - resize to fit (current size + bbox of new node)
        //    not good for moving object
        //2 - resize to be a subtree of NEW node (4x size)
        //    difficult to signal to scene graph
    }
}

void ShiftEngine::QuadTreeNode::PushFull(RenderQueue & rq)
{
    for (auto elem : GetChilds())
        elem->Draw(rq);

    if (!subtrees[0])
        return;

    for (auto elem : subtrees)
        elem->PushFull(rq);
}

void ShiftEngine::QuadTreeNode::PushToRQ(RenderQueue & rq)
{
    if (!subtrees[0])
        return;

    for (auto elem : subtrees)
    {
        int visibilityStatus = elem->CheckVisibility(rq.GetActiveCamera());

        if (visibilityStatus == 0) //out
            continue;
        if (visibilityStatus == 1) //intersect
            elem->Draw(rq);
        if (visibilityStatus == 2) //in
            elem->PushFull(rq);
    }

    //childs will be drawn by Draw method, inherited from parent
}

bool ShiftEngine::QuadTreeNode::AddNode(ISceneNode * node)
{
    // check first sizes
    auto nodebox = node->GetBBox();

    if (bbox.bMin.x >= nodebox.bMin.x ||
        bbox.bMin.y >= nodebox.bMin.y ||
        bbox.bMin.z >= nodebox.bMin.z ||
        bbox.bMax.x <= nodebox.bMax.x ||
        bbox.bMax.y <= nodebox.bMax.y ||
        bbox.bMax.z <= nodebox.bMax.z)
    {
        return false;
    }

    //current bbox includes node box
    //and we can process current node to their child nodes
    if (!subtrees[0])
    {
        //need to create subtrees or ignore
        //this magic constant defines that we should create subnodes instead of adding node to this leaf
        if (GetChilds().size() >= 4)
        {
            float x1 = bbox.bMin.x;
            float x2 = bbox.bMax.x;
            float y1 = bbox.bMin.y;
            float y2 = bbox.bMax.y;

            float xCenter = (x2 - x1) / 2.0f + x1;
            float yCenter = (y2 - y1) / 2.0f + y1;

            //need to calculate new size
            subtrees[0] = new QuadTreeNode(x1, xCenter, y1, yCenter);
            subtrees[0]->SetParent(this);
            subtrees[1] = new QuadTreeNode(xCenter, x2, y1, yCenter);
            subtrees[1]->SetParent(this);
            subtrees[2] = new QuadTreeNode(x1, xCenter, yCenter, y2);
            subtrees[2]->SetParent(this);
            subtrees[3] = new QuadTreeNode(xCenter, x2, yCenter, y2);
            subtrees[3]->SetParent(this);

            //and now, just move all childs into subnodes
            ChildsList childs = GetChilds();

            for (auto child : childs)
            {
                child->addRef();
                child->RemoveParent();
                bool added = false;
                for (auto elem : subtrees)
                {
                    if (elem->AddNode(child))
                    {
                        added = true;
                        child->release();
                        break;
                    }
                }
                if (!added)
                {
                    ISceneNode::AddChild(child);
                    child->release();
                }
            }
            AddNode(node);
            return true;
        }
        else
        {
            ISceneNode::AddChild(node);
            return true;
        }
    }

    for (auto elem : subtrees)
    {
        if (elem && elem->AddNode(node))
        {
            return true;
        }
    }
    //no one subnode cannot include this node
    //simple add this node to childs
    ISceneNode::AddChild(node);
    return true;
}

MathLib::mat4f ShiftEngine::QuadTreeNode::GetWorldMatrix() const
{
    return MathLib::matrixIdentity<float>();
}

int ShiftEngine::QuadTreeNode::CheckVisibility(CameraSceneNode * activeCam) const
{
    MathLib::mat4f matWorld = GetWorldMatrix();
    MathLib::Vector4F vecMin = { bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f };
    MathLib::Vector4F vecMax = { bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f };
    vecMin = MathLib::vec4Transform(vecMin, matWorld);
    vecMax = MathLib::vec4Transform(vecMax, matWorld);
    MathLib::AABB newBbox({ vecMin.x, vecMin.y, vecMin.z }, { vecMax.x, vecMax.y, vecMax.z });

    return activeCam->GetFrustumPtr()->CheckQTreeNode(newBbox);
}

unsigned int ShiftEngine::QuadTreeNode::GetChildsCount() const
{
    unsigned int count = GetChilds().size();

    if (!subtrees[0])
        return count;

    for (auto elem : subtrees)
        count += elem->GetChildsCount();

    return count;
}

MathLib::AABB ShiftEngine::QuadTreeNode::GetBBox() const
{
    return bbox;
}
