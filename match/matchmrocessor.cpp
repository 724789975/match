#include "matchmrocessor.h"
#include <vector>
#include <set>

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
