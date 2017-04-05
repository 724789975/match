#include "matchstack.h"



MatchStack::MatchStack(unsigned int dwMatchLevel)
	:m_dwMatchLevel(dwMatchLevel)
{
}


MatchStack::~MatchStack()
{
}

bool MatchStack::Matching(MatchGroup *matching_group)
{
	if (m_dwMatchLevel)
	{
		if (m_dwMatchLevel > matching_group->GetMaxLevel() || m_dwMatchLevel < matching_group->GetMinLevel())
		{
			return false;
		}
	}

	std::vector< const MatchGroup* >::iterator iter = m_vecStack.begin();
	while (iter != m_vecStack.end())
	{
		if (matching_group->GetId() == (*iter)->GetId())
		{
			return false;
		}
		if ((*iter)->GetMinLevel() > matching_group->GetMaxLevel() || (*iter)->GetMaxLevel() < matching_group->GetMinLevel())
		{
			return false;
		}

		++iter;
	}
	return true;
}

void MatchStack::Push(const MatchGroup* pMatchGroup)
{
	m_vecStack.push_back(pMatchGroup);
	m_dwSize += pMatchGroup->GetCount();
	//std::vector< const MatchGroup* >::iterator iter = m_vecStack.begin();
	//while (iter != m_vecStack.end())
	//{
	//	if ((*iter)->GetProgress() < m_dwSize)
	//	{
	//		(*iter)->SetProgress(m_dwSize);
	//	}
	//	++iter;
	//}
}

void MatchStack::Pop()
{
	if (!m_vecStack.empty())
	{
		m_dwSize -= m_vecStack.back()->GetCount();
		m_vecStack.pop_back();
	}
}
