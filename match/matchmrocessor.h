#ifndef __MatchProcessor_H__
#define __MatchProcessor_H__

#include <map>
#include <set>

#include "dynamicpoolex.h"
#include "matchingtree.h"
#include "matchstack.h"

#define TREE_COUNT 8

class MatchProcessor
{
public:
	MatchProcessor();
	virtual ~MatchProcessor();

public:
	void Initialize();
	void BuildMatchingTree();

	//void SetConCurrentUser(unsigned int dwUsers) { m_dwConCurrentUsers = dwUsers; }

	void Update();
	bool AddMatching(unsigned int dwId, unsigned int dwCount, unsigned int dwMatchingLevel);
	bool RemoveMatching(unsigned int dwMatchingId);

	bool Matching(unsigned int dwMatchingClients[], unsigned int dwMatchingLevel = 0, int nRTeamCount = 0, int nBTeamCount = 0);

private:
	void DecTeamCount(int dwCount);
	void IncTeamCount(int dwCount);

	bool Matching(const TreeNode<unsigned char> *node, MatchStack &stack, const std::set<unsigned int> &filter, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator begin, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator end);

	int DumpStackMatchingClient(MatchStack& refStack, unsigned int dwStackBottom, unsigned int dwMatchingClients[]);

	const TreeNode<unsigned char> * GetMatchingTree(int n) { return n > 0 && n <= TREE_COUNT ? &m_oMatchingTrees[n - 1] : NULL; }
	const TreeNode<unsigned char> * GetMatchingTree(const MatchStack& refStack, const MatchGroup* pMatchGroup);

	int GetMatchingTeamCount();

private:
	unsigned int m_dwConCurrentUsers;
	TreeNode<unsigned char> m_oMatchingTrees[TREE_COUNT];

	int m_nMaxTeamCount;
	int m_nTeamCounts[TREE_COUNT];

	TDynamicPoolEx<MatchGroup> matching_pool;
	std::multiset<MatchGroup*, MatchGroup::CompareDes> matching_queue;
	std::map<unsigned int, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator> matching_map;
};

#endif
