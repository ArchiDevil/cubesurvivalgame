#pragma once

#include "ISceneNode.h"

namespace ShiftEngine
{
	class QuadTreeNode : public ISceneNode
	{
	public:
		QuadTreeNode(float x1, float x2, float y1, float y2);
		~QuadTreeNode();

		virtual void AddChild( ISceneNode * node ) override;
		virtual void PushToRQ( RenderQueue & rq ) override;
		virtual D3DXMATRIX GetWorldMatrix() const override;

	private:
		void CreateChildSubtrees();
		bool AddNode(ISceneNode * node);
		void PushFull(RenderQueue & rq);
		void SetSubparent(QuadTreeNode * _parent);

		unsigned int GetChildsCount() const;

		virtual int CheckVisibility( CameraSceneNode * activeCam ) const;

		QuadTreeNode * subtrees[4];

	};
}
