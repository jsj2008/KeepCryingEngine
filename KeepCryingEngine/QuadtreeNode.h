#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include "TreeNode.h"

class QuadtreeNode : public TreeNode
{
public:
	QuadtreeNode();

	virtual ~QuadtreeNode();

protected:
	virtual uint GetChildrenAmount() const override;

	virtual void Divide(AABB* aabbs) override;

	virtual void CreateChildren() override;

	virtual float GetMinDrawY() const override;

	virtual float GetMaxDrawY() const override;
};

#endif