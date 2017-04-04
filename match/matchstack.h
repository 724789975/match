#ifndef __MatchStack_H__
#define __MatchStack_H__

#include <vector>
#include "matchgroup.h"

class MatchStack
{
public:
	MatchStack();
	virtual ~MatchStack();

private:
	std::vector<const MatchGroup*> stack;
	unsigned int m_dwMatchingLevel;
};

#endif
