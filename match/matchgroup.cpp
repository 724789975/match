#include "matchgroup.h"

#define EXTENDTIME 5.0f


MatchGroup::MatchGroup()
{
}

MatchGroup::~MatchGroup()
{
}

void MatchGroup::Initialize(unsigned int dwId, unsigned int dwLevel)
{
	m_dwId = dwId;
	m_dwMaxLevel = m_dwMinLevel = dwLevel;
	m_oExtendFlag.Reset();
	m_dwWeights = 0;
	m_fUpweightTimeOut = 0.0f;
	m_fExtendTimeOut = 0.0f;
	m_dwExtendTimes = 0;
}

void MatchGroup::Reset()
{
	m_oExtendFlag.Reset();
	m_dwWeights = 0;
	m_fUpweightTimeOut = 0.0f;
	m_fExtendTimeOut = 0.0f;
	m_dwExtendTimes = 0;
}

void MatchGroup::Update(double fDeltaTime)
{
	m_fUpweightTimeOut += (float)fDeltaTime;
	if (m_fUpweightTimeOut >= 1)
	{
		m_dwWeights++;
		m_fUpweightTimeOut = 0;
	}

	m_fExtendTimeOut += (float)fDeltaTime;

	if (m_fExtendTimeOut >= EXTENDTIME)
	{
		if (Extend())
		{
			m_dwExtendTimes++;
		}
		m_fExtendTimeOut = 0;
	}
}

bool MatchGroup::Matching(const MatchGroup& oMatchGroup)
{
	if (m_dwMinLevel < oMatchGroup.m_dwMinLevel && m_dwMaxLevel < oMatchGroup.m_dwMinLevel)
	{
		return false;
	}

	if (m_dwMinLevel > oMatchGroup.m_dwMaxLevel && m_dwMaxLevel > oMatchGroup.m_dwMaxLevel)
	{
		return false;
	}

	return true;
}

bool MatchGroup::Extend()
{
	if (m_dwMinLevel <= MINLEVEL && m_dwMaxLevel >= MAXLEVEL)
		return false;

	if (m_dwMinLevel > MINLEVEL)
	{
		m_dwMinLevel--;
		m_oExtendFlag.SetByte((ExtendFlag)(m_dwMinLevel - 1), true);
	}

	if (m_dwMaxLevel < MAXLEVEL)
	{
		m_dwMaxLevel++;
		m_oExtendFlag.SetByte((ExtendFlag)(m_dwMaxLevel - 1), true);
	}

	return true;
}
