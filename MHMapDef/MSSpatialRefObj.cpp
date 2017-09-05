#include "stdafx.h"
#include "MSSpatialRefObj.h"


MSSpatialRefObj::MSSpatialRefObj(void)
{
	m_eObjType = MS_OBJ_SPATIALREF;

	m_nEPSG = -1;	
	m_sWKT = "";
	m_eUnit = MH_UNIT_UNKNOWN;
}
MSSpatialRefObj::MSSpatialRefObj(MSSpatialRefObj& right)
{
	m_nEPSG = right.GetEspg();
	m_sWKT = right.GetWkt();
	m_eUnit = right.GetUnit();
}


MSSpatialRefObj::~MSSpatialRefObj(void)
{
}


void MSSpatialRefObj::SetEpsg( int epsg )
{
#ifdef GDOSMAP_EXCEPTION
	if(epsg<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "epsg");
	}
#endif
	m_nEPSG = epsg;
}


void MSSpatialRefObj::SetUnit( MSUnitType unit )
{
#ifdef GDOSMAP_EXCEPTION
	if(unit<1 || unit>8)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
	}
#endif
	m_eUnit = unit;
}


void MSSpatialRefObj::SetWkt( const string& wkt )
{
	m_sWKT = wkt;
}


int MSSpatialRefObj::GetEspg()
{
	return m_nEPSG;
}


MSUnitType  MSSpatialRefObj::GetUnit()
{
	return m_eUnit;
}


std::string MSSpatialRefObj::GetWkt()
{
	return m_sWKT;
}
