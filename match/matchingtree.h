#ifndef __TreeNode_H__
#define __TreeNode_H__

#include <vector>

template <class T>
class TreeNode
{
	public:
		TreeNode();
		TreeNode(TreeNode<T>* pFatherNode, const T& refData);
		virtual ~TreeNode();
		TreeNode<T> *GetChildNode (const T& crefKey);
		const TreeNode<T> *GetChildNode (const T& crefKey) const;
		const T& GetData() const { return m_tData; }
		TreeNode<T> *AddChildNode(const T& crefData);
		bool RemoveChildNode(const T& crefData);
		bool IsLeafNode() const { return m_vecNodes.empty(); }
		bool IsRootNode() const { return m_pFatherNode == NULL; }
	private:
		std::vector< TreeNode<T>* > m_vecNodes;
		TreeNode<T>* m_pFatherNode;
		T m_tData;
};

template <class T>
TreeNode<T>::TreeNode(): m_pFatherNode(NULL)
{
}

template <class T>
TreeNode<T>::TreeNode(TreeNode<T>* pFatherNode, const T& refData): m_pFatherNode(pFatherNode), m_tData(refData)
{
}

template <class T>
TreeNode<T>::~TreeNode()
{
	for (std::vector< TreeNode<T>* >::iterator iter = m_vecNodes.begin(); iter != m_vecNodes.end(); ++iter)
	{
		delete *iter;
	}
	m_vecNodes.clear();
}

template <class T>
TreeNode<T>* TreeNode<T>::GetChildNode(const T& crefKey)
{
	for (std::vector< TreeNode<T>* >::iterator iter = m_vecNodes.begin(); iter != m_vecNodes.end(); ++iter)
	{
		if ((*iter)->m_tData == crefKey)
		{
			return *iter;
		}
	}
	return NULL;
}

template <class T>
const TreeNode<T> *TreeNode<T>::GetChildNode (const T& crefKey) const
{
	for (std::vector< TreeNode<T>* >::const_iterator iter = m_vecNodes.begin(); iter != m_vecNodes.end(); ++iter)
	{
		if ((*iter)->m_tData == crefKey)
		{
			return *iter;
		}
	}
	return NULL;
}

template <class T>
TreeNode<T> *TreeNode<T>::AddChildNode(const T& crefData)
{
	if (GetChildNode(crefData) == NULL)
	{
		if (TreeNode<T> *pNode = new TreeNode<T>(this, crefData))
		{
			m_vecNodes.push_back(pNode);
			return pNode;
		}
	}
	return NULL;
}

template <class T>
bool TreeNode<T>::RemoveChildNode(const T& crefData)
{
	for (std::vector< TreeNode<T>* >::iterator iter = m_vecNodes.begin(); iter != m_vecNodes.end(); ++iter)
	{
		if ((*iter)->m_tData == crefData)
		{
			delete *iter;
			m_vecNodes.earse(iter);
			return true;
		}
	}
	return false;
}

#endif
