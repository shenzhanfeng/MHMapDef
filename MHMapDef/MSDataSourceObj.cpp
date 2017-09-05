#include "stdafx.h"
#include "MSDataSourceObj.h"
#include "MSLayerObj.h"

MSDataSourceObj::MSDataSourceObj()
{
	m_eObjType = MS_OBJ_DATASOURCE;

	m_sDataSrc = "";
	m_eDataSrcType = MH_SOURCE_NULL;

// 	_pGDALDataSetPtrOrOGRDatasourcePtr = NULL;
}
MSDataSourceObj::MSDataSourceObj(const MSDataSourceObj& srcObj)
{
	m_eObjType = MS_OBJ_DATASOURCE;

	m_sDataSrc = srcObj.m_sDataSrc;
// 	_pGDALDataSetPtrOrOGRDatasourcePtr = srcObj._pGDALDataSetPtrOrOGRDatasourcePtr;
	m_eDataSrcType = srcObj.m_eDataSrcType;
	m_sDataSetID = srcObj.m_sDataSetID;
	m_sDataDescription = srcObj.m_sDataDescription;
}

MSDataSourceObj::~MSDataSourceObj(void)
{
}


void MSDataSourceObj::SetDataSrc( const string& src )
{
	m_sDataSrc = src;
}


std::string MSDataSourceObj::GetDataSrc()
{
	return m_sDataSrc;
}

// void MSDataSourceObj::SetGDALDataSetPtrOrOGRDatasourcePtr(void* pDS)
// {
// 	_pGDALDataSetPtrOrOGRDatasourcePtr = pDS;
// }
// 
// void* MSDataSourceObj::GetGDALDataSetPtrOrOGRDatasourcePtr()
// {
// 	return _pGDALDataSetPtrOrOGRDatasourcePtr;
// }

void MSDataSourceObj::SetDataSrcType(MSDataSourceType SrcType)
{
#ifdef GDOSMAP_EXCEPTION
	if(SrcType<0 || SrcType>5)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSDataSourceType");
	}
#endif

	m_eDataSrcType = SrcType;
}


MSDataSourceType MSDataSourceObj::GetDataSrcType()
{
	return m_eDataSrcType;
}

void MSDataSourceObj::SetDataSetID( const string& ds )
{
	m_sDataSetID = ds;
}

string MSDataSourceObj::GetDataSetID()
{
	return m_sDataSetID;
}

void MSDataSourceObj::SetDataDescription(string str)
{
	m_sDataDescription = str;
}
string MSDataSourceObj::GetDataDescription()
{
	return m_sDataDescription;
}
