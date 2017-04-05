#ifndef __MatchGroup_H__
#define __MatchGroup_H__

#include "TByteSet.h"

#define MAXLEVEL 100
#define MINLEVEL 1

class MatchGroup
{
public:
	class CompareDes
	{
	public:
		bool operator() (const MatchGroup* l, const MatchGroup* r)
		{
			return l->GetWeights() > r->GetWeights();
		}

	};
	BeginEnumByteSet(ExtendFlag)
	{
		EndEnumByteSetWithCount(ExtendFlag, (MAXLEVEL - MINLEVEL + 1))
	};
	EnumByteSet(ExtendFlag)
public:
	MatchGroup();
	virtual ~MatchGroup();

public:
	void Initialize(unsigned int dwId, unsigned int dwLevel, unsigned int dwCount);
	void Reset();

	void Update(double fDeltaTime);
	unsigned int GetWeights() const { return m_dwWeights; }
	unsigned int GetCount() const { return m_dwCount; }
	unsigned int GetId()const { return m_dwId; }
	unsigned int GetMinLevel() const { return m_dwMinLevel; }
	unsigned int GetMaxLevel() const { return m_dwMaxLevel; }

	bool Matching(const MatchGroup& oMatchGroup);

private:
	bool Extend();

private:
	unsigned int m_dwId;

	unsigned int m_dwCount;

	unsigned int m_dwMinLevel;
	unsigned int m_dwMaxLevel;

	ExtendFlagByteSet m_oExtendFlag;

	unsigned int m_dwWeights;
	float m_fUpweightTimeOut;
	float m_fExtendTimeOut;
	unsigned int m_dwExtendTimes;
};

#endif
