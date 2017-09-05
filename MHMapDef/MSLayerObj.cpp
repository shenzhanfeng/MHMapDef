#include "stdafx.h"
#include "MSLayerObj.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#ifdef _USE_LINUX_
#include <uuid/uuid.h>
#endif // _USE_LINUX_

bool MSGenerateUID(string& strtgt)
{
#ifdef _USE_LINUX_
	uuid_t uuid;
	char str[36];
	uuid_generate(uuid);
	uuid_unparse(uuid, str);
	strtgt = (string)str;
#else
	HRESULT hr = ::CoInitialize(NULL);

	unsigned char pID[17] = {0};
	GUID guid;
	if(S_OK != ::CoCreateGuid(&guid))
	{
		::CoUninitialize();
		return false;
	}

	memcpy(pID, &guid, 16);
	::CoUninitialize();

	ostringstream oss;
	for(int i=0; i<16; ++i)
	{
		oss<<setw(2)<<hex<<setfill('0')<<(unsigned short)pID[i];
	}

	strtgt = oss.str();
#endif // _USE_LINUX_
	return true;
}


MSLayerObj::MSLayerObj()
{
	m_eObjType = MS_OBJ_LAYER;

	m_eLayerType = MS_LAYER_NULL;
	_eLayerChildType = MS_LAYER_CHILD_NULL;
	m_sName = "";
	m_bVisible = true;
	m_dTransper = 0;
	m_dScaleUpper = 1e+28;
	m_dScaleLower = 0;

	bool ret = MSGenerateUID(m_sUuid);
	if(ret==false)m_sUuid = "";

	m_pThematicObj = NULL;
	m_pDataSrcObj = new MSDataSourceObj;

	m_pOGRLayerPtrOrGDALDatasetPtr = NULL;
	m_pOGRDatasourcePtr = NULL;

	m_poCTBaseToLayer = NULL;
	m_poCTLayerToBase = NULL;

	m_bRoadModeLineLayer = false;
	m_pParent = NULL;
	m_sDefinitionQuery = "";
	m_nImgGDALDataType = 0;
}
MSLayerObj::MSLayerObj(const MSLayerObj& srcObj)
{
	m_eObjType = MS_OBJ_LAYER;

	m_pDataSrcObj = new MSDataSourceObj(*srcObj.m_pDataSrcObj);
	m_EnvelopObj = srcObj.m_EnvelopObj;
	m_pThematicObj = srcObj.m_pThematicObj;
	for (int i=0;i<srcObj.m_vVisDepthList.size();i++)
	{
		m_vVisDepthList.push_back(srcObj.m_vVisDepthList.at(i));
	}
	m_pOGRLayerPtrOrGDALDatasetPtr = srcObj.m_pOGRLayerPtrOrGDALDatasetPtr;
	m_pOGRDatasourcePtr = srcObj.m_pOGRDatasourcePtr;
	m_poCTBaseToLayer = srcObj.m_poCTBaseToLayer;
	m_poCTLayerToBase = srcObj.m_poCTLayerToBase;
// 	m_cacheBitmap = srcObj.m_cacheBitmap;
	m_bRoadModeLineLayer = srcObj.m_bRoadModeLineLayer;
	for (int i=0;i<srcObj.m_nFIDsNeedNotRender.size();i++)
	{
		m_nFIDsNeedNotRender.push_back(srcObj.m_nFIDsNeedNotRender.at(i));
	}
	m_sDefinitionQuery = srcObj.m_sDefinitionQuery;
	m_pParent = srcObj.m_pParent;

	m_eLayerType = srcObj.m_eLayerType;
	_eLayerChildType = srcObj._eLayerChildType;
	m_sName = srcObj.m_sName;
	m_bVisible = srcObj.m_bVisible;
	m_sUuid = srcObj.m_sUuid;
	m_dTransper = srcObj.m_dTransper;
	m_dScaleLower = srcObj.m_dScaleLower;
	m_dScaleUpper = srcObj.m_dScaleUpper;
}

MSLayerObj::~MSLayerObj(void)
{
	m_eLayerType = MS_LAYER_NULL;
	_eLayerChildType = MS_LAYER_CHILD_NULL;
	m_sName = "default";
	m_bVisible = true;
	m_dTransper = 0;
	m_dScaleUpper = 1e+28;
	m_dScaleLower = 0;
	m_sUuid = "";

	if(m_pThematicObj)
	{
		delete m_pThematicObj;
		m_pThematicObj = NULL;
	}
	if (m_pDataSrcObj)
	{
		delete m_pDataSrcObj;
		m_pDataSrcObj = NULL;
	}
		
	m_poCTBaseToLayer = NULL;
	m_poCTLayerToBase = NULL;

	for (int i = m_pThematicToDelete.size() - 1; i >= 0; i--)
	{
		MSThematicObj* pThematicObj = m_pThematicToDelete.at(i);
		if (pThematicObj)
		{
			delete pThematicObj;
			pThematicObj = NULL;
		}
	}
	m_pThematicToDelete.clear();
}


void MSLayerObj::SetName( const string& strName )
{
	m_sName = strName;
}
void MSLayerObj::SetLayerDescription(const string& strDesc )
{
	m_sLayerDesc = strDesc;
}

void MSLayerObj::SetVisible( bool bb )
{
	m_bVisible = bb;
}


void MSLayerObj::SetUuid(const string& uuid)
{
	m_sUuid = uuid;
}


void MSLayerObj::SetTransper( double trans )
{
	m_dTransper = trans;
}


void MSLayerObj::SetScaleUpper( double scaleUp )
{
	m_dScaleUpper = scaleUp;
}


void MSLayerObj::SetScaleLower( double scaleLo )
{
	m_dScaleLower = scaleLo;
}

void MSLayerObj::ClearSettedScale()
{
	m_dScaleUpper = 1e+28;
	m_dScaleLower = 0;
}


void MSLayerObj::SetThematicObj(MSThematicObj* ThematicObj/*, bool bDelOldThematic = true*/)
{
// 20170117 注释，如果删除以前的，按下Apply时而不关掉对话框，第2次再按时会出错，后续注意是否有内存泄漏
// 	if (m_pThematicObj && bDelOldThematic) 
// 	{
// 		delete m_pThematicObj;
// 		m_pThematicObj = NULL;
// 	}
	if (ThematicObj == m_pThematicObj)
		return;
	if (m_pThematicObj && find(m_pThematicToDelete.begin(), m_pThematicToDelete.end(), m_pThematicObj) == m_pThematicToDelete.end())
	{
		m_pThematicToDelete.push_back(m_pThematicObj);
	}
	vector<MSThematicObj*>::iterator it;
	if (ThematicObj && ( it = find(m_pThematicToDelete.begin(), m_pThematicToDelete.end(), ThematicObj)) != m_pThematicToDelete.end())
	{
		m_pThematicToDelete.erase(it);
	}

	m_pThematicObj = ThematicObj;
}


void MSLayerObj::SetLayerType( MSLayerType type )
{
	m_eLayerType = type;
}
void MSLayerObj::SetLayerChildType(MSLayerChildType type)
{
	_eLayerChildType = type;
}

MSLayerType MSLayerObj::GetLayerType()
{
	return m_eLayerType;
}
MSLayerChildType MSLayerObj::GetLayerChildType()
{
	return _eLayerChildType;
}


std::string MSLayerObj::GetName()
{
	return m_sName;
}
string	MSLayerObj::GetLayerDescription()
{
	return m_sLayerDesc;
}


bool MSLayerObj::GetVisible(bool bIgnoreParentLayer/* = false*/)
{
	//szf change 20160620,if some of the parent of a layer is invisible, it will return false if you call the function GetVisible() of the layer
	if (!m_bVisible)
		return false;
	bool bVisible = true;
	if (!bIgnoreParentLayer)
	{
		MSLayerObj* pParentLayer = GetParent();
		while (pParentLayer)
		{
			if (!pParentLayer->GetVisible())
			{
				bVisible = false;
				break;
			}
			pParentLayer = pParentLayer->GetParent();
		}
	}
	return bVisible;
}


std::string MSLayerObj::GetUuid()
{
	return m_sUuid;
}


double MSLayerObj::GetTransper()
{
	return m_dTransper;
}


double MSLayerObj::GetScaleUpper()
{
	return m_dScaleUpper;
}
bool MSLayerObj::HasSetScaleUpper()
{
	return m_dScaleUpper != 1e+28;
}

double MSLayerObj::GetScaleLower()
{
	return m_dScaleLower;
}
bool MSLayerObj::HasSetScaleLower()
{
	return m_dScaleLower != 0;
}


MSDataSourceObj* MSLayerObj::GetDataSrcObj()
{
	return m_pDataSrcObj;
}


MSEnvelopObj* MSLayerObj::GetEnvelopObj()
{
	if (IsGroupLayer())
	{
		MSGroupObj* pGroup = (MSGroupObj*)this;
		return pGroup->GetEnvelopObj();
	}
	return &m_EnvelopObj;
}

MSEnvelopObj* MSLayerObj::GetEnvelopObj_AfterCT()
{
	if (IsGroupLayer())
	{
		MSGroupObj* pGroup = (MSGroupObj*)this;
		return pGroup->GetEnvelopObj_AfterCT();
	}
	return &m_EnvelopObj_AfterCT;
}

MSThematicObj* MSLayerObj::GetThematicObj()
{
	return m_pThematicObj;
}

void MSLayerObj::AddVisDepth( int nDepth )
{
	m_vVisDepthList.push_back(nDepth);
}

void MSLayerObj::RemoveVisDepthAll()
{
	m_vVisDepthList.clear();
}

#include <algorithm>
using namespace std;
void MSLayerObj::RemoveVisDepthByValue(int nDepth)
{
	vector<int>::iterator itr = find(m_vVisDepthList.begin(), m_vVisDepthList.end(), nDepth);
	if (itr != m_vVisDepthList.end()) {
		m_vVisDepthList.erase(itr);
	}
}

void MSLayerObj::RemoveVisDepthByIndex(int nIndex)
{
	if (m_vVisDepthList.size() > nIndex) {
		m_vVisDepthList.erase(m_vVisDepthList.begin() + nIndex);
	}
}

int MSLayerObj::GetVisDepth( int nIndex )
{
	if (m_vVisDepthList.size() > nIndex && nIndex >= 0) {
		return m_vVisDepthList[nIndex];
	}
	return -1;
}

int MSLayerObj::GetVisDepthCount()
{
	return (int)m_vVisDepthList.size();
}

bool MSLayerObj::HasDepth( int nDepth )
{
	for (int i = 0; i < GetVisDepthCount(); ++i) {
		if (GetVisDepth(i) == nDepth) {
			return true;
		}
	}
	return false;
}

bool MSLayerObj::HasLabel()
{
	if (m_pThematicObj == 0) {
		return false;
	}

	if (dynamic_cast<MSFeatureThematicObj*>(m_pThematicObj)) {
		MSFeatureThematicObj* pFeatureThem = (MSFeatureThematicObj*)m_pThematicObj;

		return pFeatureThem->GetUseLabel();
	}

	return false;
}

int MSLayerObj::GetMinDepth()
{
	int nRes = 32;
	for (int i = 0; i < GetVisDepthCount() ; i++) {
		nRes = min(nRes, GetVisDepth(i));
	}
	return nRes;
}

int MSLayerObj::GetMaxDepth()
{
	int nRes = 0;
	for (int i = 0; i < GetVisDepthCount() ; i++) {
		nRes = max(nRes, GetVisDepth(i));
	}
	return nRes;
}
void	MSLayerObj::SetGroupLayer()
{
	SetLayerType(MS_LAYER_GROUP);
}
	
bool	MSLayerObj::IsGroupLayer()
{
	return GetLayerType() == MS_LAYER_GROUP;
}
bool 	MSLayerObj::GetRoadModeLineLayer() const 
{ 
	return m_bRoadModeLineLayer; 
}
void 	MSLayerObj::SetRoadModeLineLayer(bool val/* = true*/) 
{ 
	m_bRoadModeLineLayer = val; 
}
void	MSLayerObj::AddFIDsNeedNotRender(int val) 
{ 
	m_nFIDsNeedNotRender.push_back(val); 
}
void	MSLayerObj::ClearFIDNeedNotRender()
{
	m_nFIDsNeedNotRender.clear();
}
int		MSLayerObj::GetFIDsNeedNotRenderCount()
{
	return (int)m_nFIDsNeedNotRender.size();
}
int		MSLayerObj::GetFIDsNeedNotRenderAt(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetFIDsNeedNotRenderCount())
		return -1;
	return m_nFIDsNeedNotRender.at(nIndex);
}
void	MSLayerObj::RemoveFIDsNeedNotRenderAt(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetFIDsNeedNotRenderCount())
		return;
	m_nFIDsNeedNotRender.erase(m_nFIDsNeedNotRender.begin()+nIndex);
}
string	MSLayerObj::GetDefinitionQueryString()  
{ 
	return m_sDefinitionQuery; 
}
void MSLayerObj::SetParent(MSLayerObj* pLayer)
{ 
	m_pParent = pLayer;
}
MSLayerObj* MSLayerObj::GetParent()
{ 
	return m_pParent; 
}

void	MSLayerObj::SetGdalDataType(int nType)
{
	m_nImgGDALDataType = nType;
}
int		MSLayerObj::GetGdalDataType()
{
	return m_nImgGDALDataType;
}




//******************************************************************************************************************
MSGroupObj::MSGroupObj()
{
	SetLayerType(MS_LAYER_GROUP);
}
MSGroupObj::MSGroupObj(const MSGroupObj& srcObj)
{
	SetLayerType(MS_LAYER_GROUP);

	for (int i=0;i<srcObj.m_layers.size();i++)
	{
		MSLayerObj* pLayer = ((MSGroupObj)srcObj).GetLayer(i);
		MSLayerObj* pNewLayer = new MSLayerObj(*pLayer);
		m_layers.push_back(pNewLayer);
	}
}
MSGroupObj::~MSGroupObj()
{

}
int	 MSGroupObj::AddLayer(MSLayerObj* pLayer)
{
	if(!pLayer)return -1;
	m_layers.push_back(pLayer);
	pLayer->SetParent(this);
	return 0;
}
int  MSGroupObj::GetLayerCount()
{
	return (int)m_layers.size();
}
int	 MSGroupObj::DeleteLayer(int nIndex,BOOL bReleaseMemory)
{
	if(nIndex < 0 || nIndex >= GetLayerCount())
		return -1;
	MSLayerObj* pLayer = GetLayer(nIndex);
	if(!pLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;
	pLayer->SetParent(NULL);
	if (bReleaseMemory)
		delete pLayer;
	m_layers.erase(m_layers.begin()+nIndex);
	return 0;
}
int	 MSGroupObj::DeleteAllLayer(BOOL bReleaseMemory)
{
	for (int i=0;i<GetLayerCount();i++)
		DeleteLayer(0,bReleaseMemory);
	m_layers.clear();
	return 0;
}
int	 MSGroupObj::InsertLayer(int nIndex,MSLayerObj* pLayer)
{
	if(nIndex < 0 || nIndex >= GetLayerCount() || !pLayer)
		return -1;
	pLayer->SetParent(this);
	m_layers.insert(m_layers.begin()+nIndex,pLayer);
	return 0;
}
MSLayerObj* MSGroupObj::GetLayer(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetLayerCount())
		return NULL;
	return m_layers.at(nIndex);
}
MSEnvelopObj*	MSGroupObj::GetEnvelopObj()
{
	m_EnvelopObj.Reset();
	for (int i=0;i<m_layers.size();i++)
	{
		MSLayerObj* pLayer = m_layers[i];
		m_EnvelopObj.Merge(pLayer->GetEnvelopObj());
	}
	return &m_EnvelopObj;
}
MSEnvelopObj*	MSGroupObj::GetEnvelopObj_AfterCT()
{
	m_EnvelopObj_AfterCT.Reset();
	for (int i=0;i<m_layers.size();i++)
	{
		MSLayerObj* pLayer = m_layers[i];
		m_EnvelopObj_AfterCT.Merge(pLayer->GetEnvelopObj_AfterCT());
	}
	return &m_EnvelopObj_AfterCT;
}
