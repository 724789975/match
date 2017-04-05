#include "matchmrocessor.h"
#include <vector>
#include <set>
#include <algorithm>

#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

/************************************************************************/
template <class T, unsigned int N>
class FixedArray
{
public:
	FixedArray() : dwSize(0)
	{
		memset(arrData, 0, sizeof(arrData));
	}
	T& operator[] (unsigned int i)
	{
		//assert(i < N);
		return arrData[i];
	}
	const T& operator[] (unsigned int i) const
	{
		return arrData[i];
	}
	bool operator == (const FixedArray<T, N>& refFixedArray) const
	{
		if (dwSize != refFixedArray.dwSize)
		{
			return false;
		}

		for (int i = 0; i < N; ++i)
		{
			if ((*this)[i] != refFixedArray[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator < (const FixedArray<T, N>& refFixedArray) const
	{
		if (dwSize > refFixedArray.dwSize)
		{
			return false;
		}

		if (dwSize < refFixedArray.dwSize)
		{
			return true;
		}

		for (unsigned int i = 0; i < dwSize; ++i)
		{
			if (arrData[i] < refFixedArray[i])
			{
				return true;
			}
			else if (arrData[i] > refFixedArray[i])
			{
				return false;
			}
		}
		return false;

	}
	template<unsigned int NN>
	void Append(const FixedArray<T, NN>& refFixedArray)
	{
		//assert(n + fixed_array.GetSize() <= N);

		memcpy(&arrData[dwSize], &refFixedArray[0], refFixedArray.GetSize() * sizeof(T));
		dwSize += refFixedArray.GetSize();

	}

	void SetSize(unsigned int dwSize) { this->dwSize = dwSize; }
	unsigned int GetSize() const { return dwSize; }
	unsigned int GetFixedSize() const { return N; }

	void Print() const
	{
		for (int i = 0; i < dwSize; ++i)
		{
			printf("%d ", arrData[i]);
		}
	}
private:
	unsigned int dwSize;
	T arrData[N];
};

template <class T, unsigned int N>
void array_u(FixedArray<T, N> x, int n, int m, std::vector< FixedArray<T, N> >& refvecOut)
{
	static std::set< FixedArray<T, N> > u;
	if (m == 1)
	{
		u.clear();
	}

	if (m == n)
	{

		if (u.find(x) != u.end())
		{
			return;
		}

		u.insert(x);
		refvecOut.push_back(x);

	}
	else
	{
		for (int i = m; i <= n; ++i)
		{
			std::swap(x[m - 1], x[i - 1]);
			array_u(x, n, m + 1, refvecOut);
			std::swap(x[i - 1], x[m - 1]);
		}
	}
}

template <class T, unsigned int N>
void split_n(int n, int m, std::vector< FixedArray<T, N> >& refvecOut, bool bIncludeN = false)
{
	static FixedArray<T, N + 1> x;

	for (int i = 1; i <= n; ++i)
	{
		if (i >= x[m - 1])
		{
			x[m] = i;
			int rest = n - i;
			if (rest == 0 && ((bIncludeN && m > 0) || (!bIncludeN && m > 1)))
			{
				FixedArray<T, N> fixed_array;
				for (int j = 1; j <= m; ++j)
				{
					fixed_array[j - 1] = x[j];
				}
				fixed_array.SetSize(m);
				refvecOut.push_back(fixed_array);
			}
			else
			{
				split_n<T, N>(rest, m + 1, refvecOut);
			}
			x[m] = 0;
		}
	}
}

template <class T, unsigned int N, unsigned int NN>
void combin_p(const FixedArray<T, N> &charset, const unsigned int keys[], const FixedArray<T, NN> &pattern, unsigned int num, unsigned int des, unsigned int loc, std::vector< FixedArray<unsigned int, NN> > &out)
{
	static FixedArray<unsigned int, NN> output;

	int len = charset.GetSize();

	if (num == des)
	{
		for (int i = 0; i < pattern.GetSize(); ++i)
		{
			if (charset[output[i]] != pattern[i])
			{
				return;
			}
		}
		output.SetSize(pattern.GetSize());
		out.push_back(output);
		return;
	}
	if (des - num > len - loc)
	{
		return;
	}

	output[num] = keys[loc];
	combin_p<T, N, NN>(charset, keys, pattern, num + 1, des, loc + 1, out);
	combin_p<T, N, NN>(charset, keys, pattern, num, des, loc + 1, out);
}
/************************************************************************/


MatchProcessor::MatchProcessor()
{
}


MatchProcessor::~MatchProcessor()
{
}

void MatchProcessor::Initialize()
{
	BuildMatchingTree();
	matching_pool.Init(64, 64);
}

void MatchProcessor::BuildMatchingTree()
{
	for (int i = 0; i < TREE_COUNT; ++i)
	{
		typedef FixedArray<unsigned char, TREE_COUNT> TeamFixedArray;
		typedef std::vector<TeamFixedArray> TeamFixedArrayVec;
		TeamFixedArrayVec vecTeamExpr, vecAllTeamExpr;

		split_n(i + 1, 1, vecTeamExpr, true);

		for (TeamFixedArrayVec::iterator iter = vecTeamExpr.begin(); iter != vecTeamExpr.end(); ++iter)
		{
			array_u(*iter, iter->GetSize(), 1, vecAllTeamExpr);
		}

		for (TeamFixedArrayVec::iterator iter = vecAllTeamExpr.begin(); iter != vecAllTeamExpr.end(); ++iter)
		{
			TreeNode<unsigned char> * pNode = &m_oMatchingTrees[i];

			TeamFixedArray &refFixedArray = *iter;
			for (unsigned int i = 0; i < refFixedArray.GetSize(); ++i)
			{
				TreeNode<unsigned char>* pChildNode = pNode->GetChildNode(refFixedArray[i]);
				if (pChildNode)
				{
					pNode = pChildNode;
				}
				else
				{
					pNode = pNode->AddChildNode(refFixedArray[i]);
				}
			}
		}
	}
}

#ifdef _WIN32
int	gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif
void MatchProcessor::Update()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	static double fLastTime = tv.tv_sec + (double)tv.tv_usec * 0.000001, diffs = 0;
	double diff = tv.tv_sec + (double)tv.tv_usec * 0.000001 - fLastTime;
	fLastTime = tv.tv_sec + (double)tv.tv_usec * 0.000001;

	std::multiset<MatchGroup*, MatchGroup::CompareDes> matching_queue2 = matching_queue;
	matching_queue.clear();

	std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator iter = matching_queue2.begin();
	while (iter != matching_queue2.end())
	{
		if (MatchGroup *matching_group = *iter)
		{
			matching_group->Update(diff);
			std::map<unsigned int, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator>::iterator it = matching_map.find(matching_group->GetId());
			if (it != matching_map.end())
			{
				it->second = matching_queue.insert(matching_group);
			}
		}
		++iter;
	}

	if (!matching_queue.empty())
	{
		bool ret = true;

		double start_matching_time = fLastTime;
		while (ret)
		{
			unsigned int matching_clients[TREE_COUNT * 2] = { 0 };

			if (ret = Matching(matching_clients))
			{
				for (int i = 0; i < TREE_COUNT; ++i)
				{
					if (matching_clients[i])
					{
						printf("%d\t", matching_clients[i]);
					}
				}
				printf("\n");
				for (int i = TREE_COUNT; i < TREE_COUNT * 2; ++i)
				{
					if (matching_clients[i])
					{
						printf("%d\t", matching_clients[i]);
					}
				}
				printf("end!!!!!!!\n");
			}
		}
	}
}

bool MatchProcessor::AddMatching(unsigned int dwId, unsigned int dwCount, unsigned int dwMatchingLevel)
{
	std::map<unsigned int, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator>::iterator iter = matching_map.find(dwId);
	if (iter == matching_map.end())
	{
		if (MatchGroup *matching_group = matching_pool.FetchObj())
		{
			IncTeamCount(dwCount);
			matching_group->Initialize(dwId, dwMatchingLevel, dwCount);
			std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator it = matching_queue.insert(matching_group);
			matching_map.insert(std::make_pair(dwId, it));

			m_dwConCurrentUsers += dwCount;

			printf("%s, %s, match count : %d\r\n", __FILE__, __FUNCTION__, (int)matching_queue.size());
			return true;
		}
	}
	return false;
}

bool MatchProcessor::RemoveMatching(unsigned int dwMatchingId)
{
	std::map<unsigned int, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator>::iterator iter = matching_map.find(dwMatchingId);
	if (iter != matching_map.end())
	{
		DecTeamCount((*iter->second)->GetCount());
		m_dwConCurrentUsers -= (*iter->second)->GetCount();
		(*iter->second)->Reset();
		matching_pool.ReleaseObj((*iter->second));
		matching_queue.erase(iter->second);
		matching_map.erase(iter);

		printf("%s, %s, match count : %d\r\n", __FILE__, __FUNCTION__, (int)matching_queue.size());
		return true;
	}
	return false;
}

void MatchProcessor::DecTeamCount(int dwCount)
{
	m_nTeamCounts[dwCount - 1]--;
	if (m_nTeamCounts[dwCount - 1] == 0 && dwCount == m_nMaxTeamCount)
	{
		if (dwCount <= 1)
		{
			m_nMaxTeamCount = 1;
		}
		else
		{
			for (int i = dwCount - 2; i >= 0; --i)
			{
				if (m_nTeamCounts[i] != 0)
				{
					m_nMaxTeamCount = i + 1;
					break;
				}
			}
			if (dwCount == m_nMaxTeamCount)
				m_nMaxTeamCount = 1;
		}
	}
}

void MatchProcessor::IncTeamCount(int dwCount)
{
	m_nTeamCounts[dwCount - 1]++;
	if (dwCount > m_nMaxTeamCount)
	{
		m_nMaxTeamCount = dwCount;
	}
}

bool MatchProcessor::Matching(unsigned int dwMatchingClients[], unsigned int dwMatchingLevel /*= 0*/, int nRTeamCount /*= 0*/, int nBTeamCount /*= 0*/)
{
	bool ret = true;

	std::set<unsigned int> setFilter;
	MatchStack oStack(dwMatchingLevel);

	unsigned int dwStackBottom = 0;
	const TreeNode<unsigned char>* node = GetMatchingTree(nRTeamCount);
	//while (ret = Matching(node, oStack, setFilter, matching_queue.begin(), matching_queue.end()))
	while (ret)
	{
		if (!(dwMatchingLevel != 0 && nRTeamCount == 0))
		{
			ret = Matching(node, oStack, setFilter, matching_queue.begin(), matching_queue.end());
		}
		dwStackBottom = oStack.Size();
		node = GetMatchingTree(nBTeamCount);
		if (!(dwMatchingLevel != 0 && nBTeamCount == 0))
		{
			ret &= Matching(node, oStack, setFilter, matching_queue.begin(), matching_queue.end());
		}
		ret &= oStack.Size() > 0;
		if (!ret)
		{
			while (!oStack.IsEmpty())
			{
				const MatchGroup* matching_group = oStack.Top();
				setFilter.insert(matching_group->GetId());
				oStack.Pop();
			}
		}
		else
		{
			break;
		}
	}

	if (ret)
	{
		DumpStackMatchingClient(oStack, dwStackBottom, &dwMatchingClients[TREE_COUNT]);
		DumpStackMatchingClient(oStack, 0, &dwMatchingClients[0]);
	}

	return ret;
}

bool MatchProcessor::Matching(const TreeNode<unsigned char> *node, MatchStack &stack, const std::set<unsigned int> &filter, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator begin, std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator end)
{
	std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator iter = begin;
	while (iter != end)
	{
		if (filter.find((*iter)->GetId()) == filter.end())
		{
			const TreeNode<unsigned char>* n = node == NULL ? GetMatchingTree(stack, *iter) : node;
			if (n)
			{
				if (n = n->GetChildNode((*iter)->GetCount()))
				{
					if (stack.Matching(*iter))
					{
						stack.Push(*iter);

						if (n->IsLeafNode())
							return true;

						if (Matching(n, stack, filter, ++std::multiset<MatchGroup*, MatchGroup::CompareDes>::iterator(iter), end))
							return true;

						stack.Pop();
					}
				}
			}
		}
		++iter;
	}
	return false;
}

int MatchProcessor::DumpStackMatchingClient(MatchStack& refStack, unsigned int dwStackBottom, unsigned int dwMatchingClients[])
{
	int nCount = 0;
	while (refStack.Size() != dwStackBottom)
	{
		const MatchGroup * matching_group = refStack.Top();
		dwMatchingClients[nCount] = matching_group->GetId();
		RemoveMatching(matching_group->GetId());
		refStack.Pop();
		nCount++;
	}
	return nCount;
}

const TreeNode<unsigned char> * MatchProcessor::GetMatchingTree(const MatchStack& refStack, const MatchGroup* pMatchGroup)
{
	return GetMatchingTree(GetMatchingTeamCount());
}

int MatchProcessor::GetMatchingTeamCount()
{
	int count = 0;

	if (m_nMaxTeamCount > (int)m_dwConCurrentUsers / 2)
	{
		m_dwConCurrentUsers -= m_nMaxTeamCount;
	}
	if (m_dwConCurrentUsers > 0 && m_dwConCurrentUsers <= 6)
	{
		count = (std::max)(1, (std::min)(m_nMaxTeamCount, (int)m_dwConCurrentUsers / 2));
	}
	else if (m_dwConCurrentUsers > 6 && m_dwConCurrentUsers <= 12)
	{
		count = (std::max)(2, (std::min)(m_nMaxTeamCount, (int)m_dwConCurrentUsers / 2));
	}
	else if (m_dwConCurrentUsers > 12 && m_dwConCurrentUsers <= 50)
	{
		count = (std::max)(3, (std::min)(m_nMaxTeamCount, (int)m_dwConCurrentUsers / 2));
	}
	else if (m_dwConCurrentUsers > 50 && m_dwConCurrentUsers <= 200)
	{
		count = (std::max)(4, (std::min)(m_nMaxTeamCount, (int)m_dwConCurrentUsers / 2));
	}
	else
	{
		count = 8;
	}

	// 每队最多8人
	if (count > 8)
	{
		count = 8;
	}

	if (count > (int)m_dwConCurrentUsers / 2)
	{
		count = m_dwConCurrentUsers / 2;
	}

	return count;
}
