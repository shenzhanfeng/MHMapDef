#include "stdafx.h"
#include "MSEnvelopObj.h"

//////////////////////////////////////////////////////////////////////////
//
// class 
//
//////////////////////////////////////////////////////////////////////////
MSEnvelopObj::MSEnvelopObj(void)
{
	m_eObjType = MS_OBJ_ENVELOP;

	m_dMinx = m_dMiny = MAXNUM_MSENVELOP;
	m_dMaxx = m_dMaxy = -MAXNUM_MSENVELOP;
}
MSEnvelopObj::MSEnvelopObj(double x1,double y1,double x2,double y2)
{
	m_dMinx = x1;
	m_dMiny = y1;
	m_dMaxx = x2;
	m_dMaxy = y2;
}
MSEnvelopObj::MSEnvelopObj(const MSEnvelopObj& srcObj)
{
	m_dMinx = srcObj.m_dMinx;
	m_dMaxx = srcObj.m_dMaxx;
	m_dMiny = srcObj.m_dMiny;
	m_dMaxy = srcObj.m_dMaxy;
}


MSEnvelopObj::~MSEnvelopObj(void)
{
	m_dMinx = m_dMiny = MAXNUM_MSENVELOP;
	m_dMaxx = m_dMaxy = -MAXNUM_MSENVELOP;
}


void MSEnvelopObj::SetMinx( double value )
{
	m_dMinx = value;
}


void MSEnvelopObj::SetMiny( double value )
{
	m_dMiny = value;
}


void MSEnvelopObj::SetMaxx( double value )
{
	m_dMaxx = value;
}


void MSEnvelopObj::SetMaxy( double value )
{
	m_dMaxy = value;
}


double MSEnvelopObj::GetMinx()
{
	return m_dMinx;
}


double MSEnvelopObj::GetMiny()
{
	return m_dMiny;
}


double MSEnvelopObj::GetMaxx()
{
	return m_dMaxx;
}


double MSEnvelopObj::GetMaxy()
{
	return m_dMaxy;
}

void MSEnvelopObj::Merge(MSEnvelopObj* pOther)
{
	if(!pOther)
		return;
	if (fabs(pOther->GetHeight()) < 1e-6 || fabs(pOther->GetWidth()) < 1e-6)
		return;
	if(m_dMinx > pOther->GetMinx())
	{
		m_dMinx = pOther->GetMinx();
	}
	if(m_dMiny > pOther->GetMiny())
	{
		m_dMiny = pOther->GetMiny();
	}
	if(m_dMaxx < pOther->GetMaxx())
	{
		m_dMaxx = pOther->GetMaxx();
	}
	if(m_dMaxy < pOther->GetMaxy())
	{
		m_dMaxy = pOther->GetMaxy();
	}
}

void MSEnvelopObj::Reset()
{
	m_dMinx = m_dMiny = MAXNUM_MSENVELOP;
	m_dMaxx = m_dMaxy = -MAXNUM_MSENVELOP;
}
bool MSEnvelopObj::IsReset()
{
	if (fabs(m_dMinx - MAXNUM_MSENVELOP) >1e-6 || fabs(m_dMiny - MAXNUM_MSENVELOP) > 1e-6 ||
		fabs(m_dMaxx + MAXNUM_MSENVELOP) >1e-6 || fabs(m_dMaxy + MAXNUM_MSENVELOP) > 1e-6)
	{
		return false;
	}
	return true;
}


double		MSEnvelopObj::GetWidth()
{
	return m_dMaxx - m_dMinx;
}
double		MSEnvelopObj::GetHeight()
{
	return m_dMaxy - m_dMiny;
}
bool		MSEnvelopObj::Within(MSEnvelopObj* pOther)
{
	if (m_dMinx < pOther->GetMinx())
		return false;
	if (m_dMaxx > pOther->GetMaxx())
		return false;
	if (m_dMiny < pOther->GetMiny())
		return false;
	if (m_dMaxy > pOther->GetMaxy())
		return false;
	return true;
}
bool		MSEnvelopObj::Contain(MSEnvelopObj* pOther)
{
	if (m_dMinx > pOther->GetMinx())
		return false;
	if (m_dMaxx < pOther->GetMaxx())
		return false;
	if (m_dMiny > pOther->GetMiny())
		return false;
	if (m_dMaxy < pOther->GetMaxy())
		return false;
	return true;
}
bool		MSEnvelopObj::Intersect(MSEnvelopObj* pOther)
{
	bool bXIntersect = true;
	bool bYIntersect = true;
	if (m_dMaxx < pOther->m_dMinx)
	{
		return false;
	}
	else if (m_dMinx > pOther->m_dMaxx)
	{
		return false;
	}
	if (m_dMaxy < pOther->m_dMiny)
	{
		return false;
	}
	else if (m_dMiny > pOther->m_dMaxy)
	{
		return false;
	}
	return true;

}
