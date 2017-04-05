#ifndef __MatchStack_H__
#define __MatchStack_H__

#include <vector>
#include "matchgroup.h"

class MatchStack
{
public:
	MatchStack(unsigned int dwMatchLevel);
	virtual ~MatchStack();

	bool Matching(MatchGroup *matching_group);

	bool IsEmpty() const { return m_vecStack.empty(); }
	const MatchGroup* Top() const { return m_vecStack.back(); }
	void Push(const MatchGroup* pMatchGroup);
	void Pop();
	unsigned int Size() { return (unsigned int)m_vecStack.size(); }
	void Clear() { m_vecStack.clear(); }

private:
	std::vector<const MatchGroup*> m_vecStack;
	unsigned int m_dwMatchLevel;

	unsigned int m_dwSize;
};

#endif
