#include "stdafx.h"
#include "MSObj.h"




MSObj::MSObj(void)
{
	m_eObjType = MS_OBJ_BASE;
}


MSObj::MSObj( const MSObj& srcObj )
{
	m_eObjType = srcObj.m_eObjType;
}


MSObj::~MSObj(void)
{
}


MSObjType MSObj::GetObjType()
{
	return m_eObjType;
}


#ifdef GDOSMAP_TEST
void MSObj::SetObjType( MSObjType type )
{
	m_eObjType = type;
}
#endif
