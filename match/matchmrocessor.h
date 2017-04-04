#ifndef __MatchProcessor_H__
#define __MatchProcessor_H__

#include "matchingtree.h"

#define TREE_COUNT 8

class MatchProcessor
{
public:
	MatchProcessor();
	virtual ~MatchProcessor();

	void BuildMatchingTree();

private:
	TreeNode<unsigned char> m_oMatchingTrees[TREE_COUNT];
};

#endif
