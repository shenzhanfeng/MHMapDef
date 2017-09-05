#include "stdafx.h"
#include "MSMapObj.h"

#include "pugixml.hpp"

using namespace pugi;
//////////////////////////////////////////////////////////////////////////
//
// class 
//
//////////////////////////////////////////////////////////////////////////
// #include "..\\TestSample\\shenzf.h"
#ifdef _USE_LINUX_
char* itoa(int value, char* result, int base) 
{
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do 
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) 
	{
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
#endif
MSMapObj::MSMapObj(void)
{
	m_eObjType = MS_OBJ_MAP;
	m_VctLayers.clear();
	m_pSpatialRefObj = NULL;
	m_pSpatialRefObj = new MSSpatialRefObj;
	m_pMapEnvelopObj_Whole = new MSEnvelopObj;
	m_pMapEnvelopObj_CurrentView = new MSEnvelopObj;
	m_bIsMapVisible = true;
	m_pSpatialRefObj->SetEpsg(-1);
	m_pSpatialRefObj->SetWkt("");
	m_pSpatialRefObj->SetUnit(MH_UNIT_UNKNOWN);
	m_nTmpLayerIndexForGetNextValidLayer = -1;
	m_sMapName = "Layers";
	m_nLabelCompactness = 1;
}
MSMapObj::MSMapObj(MSMapObj* pMap)
{
	if(!pMap)return;

	m_eObjType = MS_OBJ_MAP;

	m_pSpatialRefObj = new MSSpatialRefObj(*pMap->GetSpatialRefObj());
	m_pMapEnvelopObj_Whole = new MSEnvelopObj(*pMap->GetEnvelopObj_Whole());
	m_pMapEnvelopObj_CurrentView = new MSEnvelopObj(*pMap->GetEnvelopObj_CurrentView());

	for (int i=0;i<pMap->GetRootLayerCount();i++)
	{
		MSLayerObj* pLayerObj = new MSLayerObj(*pMap->GetRootLayer(i));
		AppendRootLayer(pLayerObj);
	}
	m_bIsMapVisible = pMap->m_bIsMapVisible;
	m_nTmpLayerIndexForGetNextValidLayer = -1;
	m_sMapName = pMap->GetName();
	m_nLabelCompactness = pMap->GetLabelCompactness();
}

void MSMapObj::ReleaseLayerMemory(MSLayerObj* pLayer)
{
	if(pLayer->IsGroupLayer())
	{
		MSGroupObj* pGroup = (MSGroupObj*)pLayer;
		for (int i=0;i<pGroup->GetLayerCount();i++)
		{
			MSLayerObj* pTmpLayer = pGroup->GetLayer(i);
			ReleaseLayerMemory(pTmpLayer);
		}
		delete pLayer;
		pLayer = NULL;

	}
	else
	{
		delete pLayer;
		pLayer = NULL;
	}
}
MSMapObj::~MSMapObj(void)
{
	int nSize = (int)m_VctLayers.size();
	for(int i=0; i<nSize; i++)
	{
		MSLayerObj* pLayer = m_VctLayers.at(i);
		if(pLayer)
			ReleaseLayerMemory(pLayer);
	}
	m_VctLayers.clear();
	if (m_pMapEnvelopObj_Whole)
	{
		delete m_pMapEnvelopObj_Whole;
		m_pMapEnvelopObj_Whole = NULL;
	}
	if (m_pMapEnvelopObj_CurrentView)
	{
		delete m_pMapEnvelopObj_CurrentView;
		m_pMapEnvelopObj_CurrentView = NULL;
	}
	if (m_pSpatialRefObj)
	{
		delete m_pSpatialRefObj;
		m_pSpatialRefObj = NULL;
	}
}


MSSpatialRefObj* MSMapObj::GetSpatialRefObj()
{
	return m_pSpatialRefObj;
}

MSEnvelopObj* MSMapObj::GetEnvelopObj_Whole()
{
	return m_pMapEnvelopObj_Whole;
}
void MSMapObj::SetEnvelopObj_CurrentView(double dLTX, double dLTY, double dUBX, double dUBY)
{
	m_pMapEnvelopObj_CurrentView->SetMinx(min(dLTX, dUBX));
	m_pMapEnvelopObj_CurrentView->SetMiny(min(dLTY, dUBY));
	m_pMapEnvelopObj_CurrentView->SetMaxx(max(dLTX, dUBX));
	m_pMapEnvelopObj_CurrentView->SetMaxy(max(dLTY, dUBY));
}
MSEnvelopObj* MSMapObj::GetEnvelopObj_CurrentView()
{
	return m_pMapEnvelopObj_CurrentView;
}

int	MSMapObj::GetRootLayerCount()
{
	return (int)m_VctLayers.size();
}

MSLayerObj*	MSMapObj::GetRootLayerByUUID(const string& strUid)
{
	int nLayerCnt = (int)m_VctLayers.size();
	for (int i = 0; i < nLayerCnt; i++)
	{
		if (m_VctLayers.at(i)->GetUuid() == strUid)
		{
			return m_VctLayers.at(i);
		}
	}
	return NULL;
}

MSLayerObj*	MSMapObj::GetRootLayer(int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt-1))
	{
		return NULL;
	}
	return m_VctLayers.at(iIndex);
}

int	MSMapObj::GetRootLayerIndex(const string& strUid)
{
	int nLayerCnt = (int)m_VctLayers.size();
	for (int i = 0; i < nLayerCnt; i++)
	{
		if (m_VctLayers.at(i)->GetUuid() == strUid)
		{
			return i;
		}
	}
	return -1;
}

int	MSMapObj::InsertRootLayerBefore(MSLayerObj* pLayer, int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt))
	{
		return -1;
	}
	m_VctLayers.insert(m_VctLayers.begin()+iIndex, pLayer);
	UpdateMapEnvelop();

	return 0;
}

int MSMapObj::InsertRootLayerAfter( MSLayerObj* pLayer, int iIndex )
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt-1))
	{
		return -1;
	}
	if(iIndex == GetRootLayerCount())
		m_VctLayers.push_back(pLayer);
	else
		m_VctLayers.insert(m_VctLayers.begin()+iIndex+1, pLayer);
	UpdateMapEnvelop();

	return 0;
}

int	MSMapObj::AppendRootLayer(MSLayerObj* pLayer)
{
	if(pLayer==0) return -1;
	m_VctLayers.push_back(pLayer);
// 	UpdateMapEnvelop();

	return 0;
}

int MSMapObj::ReplaceRootLayer( MSLayerObj* pLayer, int iIndex )
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt-1))
	{
		return -1;
	}

	//shenzf，由于列表中不再有原来的pLayer，因此需要先将其内在释放
	MSLayerObj* pTmpLayer = m_VctLayers.at(iIndex);
	ReleaseLayerMemory(pTmpLayer);

	m_VctLayers.at(iIndex) = pLayer;
	UpdateMapEnvelop();

	return 0;
}

int MSMapObj::SwapRootLayer( int iSrcIndex, int iDstIndex )
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iSrcIndex < 0) || (iSrcIndex > nLayerCnt-1) || (iDstIndex < 0) || (iDstIndex > nLayerCnt-1))
	{
		return -1;
	}
	MSLayerObj* pLayer1 = m_VctLayers.at(iSrcIndex);
	MSLayerObj* pLayer2 = m_VctLayers.at(iDstIndex);
	m_VctLayers.at(iSrcIndex) = pLayer2;
	m_VctLayers.at(iDstIndex) = pLayer1;

	return 0;
}

int MSMapObj::DeleteRootLayer(int iIndex,BOOL bReleaseMemory)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if (iIndex < 0||iIndex > nLayerCnt-1)
		return -1;

	if (bReleaseMemory)
	{
		MSLayerObj* pTmpLayer = m_VctLayers.at(iIndex);
		ReleaseLayerMemory(pTmpLayer);
	}

	m_VctLayers.erase(m_VctLayers.begin()+iIndex);
	UpdateMapEnvelop();

	return 0;
}

int MSMapObj::DeleteAllLayers(BOOL bReleaseMemory)
{
	int nSize = (int)m_VctLayers.size();
	for(int i=nSize-1; i>=0; i--)
	{
		MSLayerObj* pLayer = m_VctLayers.at(i);
		string sLayerIndex = GetLayerString(pLayer);
		int nReturn = DeleteLayer(sLayerIndex,bReleaseMemory);
		if(nReturn != 0)
			return nReturn;
	}
	return 0;
}

int	MSMapObj::MoveRootLayerUp(int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 1)||(iIndex > nLayerCnt-1))
	{
		return -1;
	}
	return SwapRootLayer(iIndex, iIndex-1);
}


int	MSMapObj::MoveRootLayerDown(int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt-2))
	{
		return -1;
	}
	return SwapRootLayer(iIndex, iIndex+1);
}


int MSMapObj::MoveRootLayerUpTo( int iIndex, int pos )
{
	int nLayerCnt = (int)m_VctLayers.size();
	if(iIndex<1 || iIndex>nLayerCnt-1 || pos>=iIndex || pos<0)
	{
		return -1;
	}

	for(int i=0; i<(iIndex-pos); i++)
	{
		if(SwapRootLayer((iIndex-i),(iIndex-1-i))!=0)
		{
			return -1;
		}
	}
	return 0;
}


int MSMapObj::MoveRootLayerDownTo( int iIndex, int pos )
{
	int nLayerCnt = (int)m_VctLayers.size();
	if(iIndex<0 || iIndex>nLayerCnt-2 || pos<=iIndex || pos>nLayerCnt-1)
	{
		return -1;
	}

	for(int i=0; i<(pos-iIndex); i++)
	{
		if(SwapRootLayer((iIndex-i),(iIndex+1-i))!=0)
		{
			return -1;
		}
	}
	return 0;
}


int	MSMapObj::MoveRootLayerToTop(int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 1)||(iIndex > nLayerCnt-1))
	{
		return -1;
	}
	for (int i = iIndex; i > 0; i--)
	{
		if(SwapRootLayer(i, i-1) != 0)
			return -1;
	}
	return 0;
}


int	MSMapObj::MoveRootLayerToBottom(int iIndex)
{
	int nLayerCnt = (int)m_VctLayers.size();
	if ((iIndex < 0)||(iIndex > nLayerCnt-2))
	{
		return -1;
	}
	for (int i = iIndex; i < nLayerCnt-1; i++)
	{
		if(SwapRootLayer(i, i+1) != 0)
			return -1;
	}
	return 0;
}

int MSMapObj::GetRootLayerCntInDepth( int nDepth )
{
	int nCount = 0;

	for (int i = 0; i < GetRootLayerCount() ; i++) {
		MSLayerObj* pLayer = GetRootLayer(i);
		if (pLayer != 0) {
			if (pLayer->HasDepth(nDepth)) {
				nCount++;
			}
		}
	}

	return nCount;
}


//改成了 2_12_5   12_19_7  8 9_6  的方式
MSLayerObj*	MSMapObj::GetLayer(string sLayerIndex)
{
	MSLayerObj* pLayer = NULL;
	while(sLayerIndex != "")
	{
		int nFind = (int)sLayerIndex.find("_");
		string sCur = sLayerIndex;
		if(nFind >= 0)
		{
			sCur = sLayerIndex.substr(0,nFind);
			sLayerIndex = sLayerIndex.substr(nFind+1);
		}
		else
			sLayerIndex = "";
		int nCur = atoi(sCur.c_str());
		if(!pLayer)
			pLayer = GetRootLayer(nCur);
		else
		{
			MSGroupObj* pGroupLayer = (MSGroupObj*)pLayer;
			if (!pLayer || pLayer->GetLayerType() != MS_LAYER_GROUP || !pGroupLayer || nCur<0 || nCur > pGroupLayer->GetLayerCount()-1)
				ASSERT_FALSE_AND_RETURN_NULL;
			pLayer = pGroupLayer->GetLayer(nCur);
			if(!pLayer)
				ASSERT_FALSE_AND_RETURN_NULL;
		}
	}
	return pLayer;
}
string MSMapObj::GetLayerString(MSLayerObj* pLayer)
{
	if(!pLayer)
		ASSERT_FALSE_AND_RETURN_SPACE;
	vector<string> sReturn;
	for (int i = 0; i < GetRootLayerCount(); i++)
	{
		MSLayerObj* pLayerObj = GetRootLayer(i);
		char tmp[255];
		itoa(i,tmp,10);
		string sLayer = string(tmp);
		sReturn.push_back(sLayer);

		string value = GetSubLayerString(pLayerObj, pLayer, sReturn);
		if(value != "")
			return value;
		sReturn.pop_back();
	}
	return "";
}
string MSMapObj::GetSubLayerString(MSLayerObj* pLayerObj,MSLayerObj* pLayerFind,vector<string>& sReturn)
{
	string sValue;
	MSLayerType type = pLayerObj->GetLayerType();
	if (type == MS_LAYER_GROUP)
	{
		MSGroupObj* pGroupObj = (MSGroupObj*)pLayerObj;
		if(!pGroupObj)
			ASSERT_FALSE_AND_RETURN_SPACE;
		for (int i=0;i<pGroupObj->GetLayerCount();i++)
		{
			char tmp[255];
			itoa(i,tmp,10);
			string ss = string(tmp);
			sReturn.push_back(ss);
			MSLayerObj* pTmpLayerObj = pGroupObj->GetLayer(i);
			string value = GetSubLayerString(pTmpLayerObj,pLayerFind,sReturn);
			if(value != "")
				return value;
			else
				sReturn.pop_back();
		}
	}
	if(pLayerObj == pLayerFind)
	{
		for (int i=0;i<sReturn.size();i++)
		{
			sValue += sReturn.at(i);
			sValue += "_";
		}
		sValue = sValue.substr(0,sValue.length()-1);
		return sValue;
	}
	// 	else
	// 	{
	// 		sReturn.pop_back();
	// 	}
	return "";
}

int	MSMapObj::InsertLayerBefore(MSLayerObj* pLayer, string sLayerIndex)
{
	if(!pLayer)
		return -1;
	MSLayerObj* pCurLayer = GetLayer(sLayerIndex);
	if(!pCurLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;
	int nFind = (int)sLayerIndex.rfind("_");
	
	MSLayerObj* pParentLayer = NULL;
	if(nFind > 0)
	{
		string sParent = sLayerIndex.substr(0,nFind);
		pParentLayer = GetLayer(sParent);
		if(!pParentLayer)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}

	int nReturn = -1;
	if (pParentLayer)
	{
		MSGroupObj* pGroupLayer = (MSGroupObj*)pParentLayer;
		if(!pGroupLayer || pParentLayer->GetLayerType() != MS_LAYER_GROUP)
			ASSERT_FALSE_AND_RETURN_ERROR;

		int nIndex = -1;
		for (int i=0;i<pGroupLayer->GetLayerCount();i++)
		{
			if (pGroupLayer->GetLayer(i) == pCurLayer)
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex < 0 || nIndex >= pGroupLayer->GetLayerCount())
			ASSERT_FALSE_AND_RETURN_ERROR;
		nReturn = pGroupLayer->InsertLayer(nIndex,pLayer);
		UpdateMapEnvelop();
	}
	else
	{
		//根目录插入
		int nIndex = atoi(sLayerIndex.c_str());
		nReturn = InsertRootLayerBefore(pLayer,nIndex);
	}

	return nReturn;
}
int	MSMapObj::InsertLayerAfter(MSLayerObj* pLayer, string sLayerIndex)
{
	if(!pLayer)
		return -1;
	MSLayerObj* pCurLayer = GetLayer(sLayerIndex);
	if(!pCurLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;
	int nFind = (int)sLayerIndex.rfind("_");

	MSLayerObj* pParentLayer = NULL;
	if(nFind > 0)
	{
		string sParent = sLayerIndex.substr(0,nFind);
		pParentLayer = GetLayer(sParent);
		if(!pParentLayer)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}

	int nReturn = -1;
	if (pParentLayer)
	{
		MSGroupObj* pGroupLayer = (MSGroupObj*)pParentLayer;
		if(!pGroupLayer || pParentLayer->GetLayerType() != MS_LAYER_GROUP)
			ASSERT_FALSE_AND_RETURN_ERROR;

		int nIndex = -1;
		for (int i=0;i<pGroupLayer->GetLayerCount();i++)
		{
			if (pGroupLayer->GetLayer(i) == pCurLayer)
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex < 0 || nIndex >= pGroupLayer->GetLayerCount())
			ASSERT_FALSE_AND_RETURN_ERROR;
		if(nIndex == pGroupLayer->GetLayerCount()-1)
			nReturn = pGroupLayer->AddLayer(pLayer);
		else
			nReturn = pGroupLayer->InsertLayer(nIndex+1,pLayer);
		UpdateMapEnvelop();
	}
	else
	{
		//根目录插入
		int nIndex = atoi(sLayerIndex.c_str());
		nReturn = InsertRootLayerAfter(pLayer,nIndex);
	}
	return nReturn;
}
int	MSMapObj::InsertLayerToGroup(MSLayerObj* pLayer,string sGroupIndex)
{
	if(!pLayer)
		return -1;
	MSLayerObj* pParentLayer = GetLayer(sGroupIndex);
	if(!pParentLayer || pParentLayer->GetLayerType() != MS_LAYER_GROUP)
		ASSERT_FALSE_AND_RETURN_ERROR;
	MSGroupObj* pGroupLayer = (MSGroupObj*)pParentLayer;
	return pGroupLayer->AddLayer(pLayer);
}
int	MSMapObj::ReplaceLayer(MSLayerObj* pLayer, string sLayerIndex)
{
	int nSuc = InsertLayerAfter(pLayer,sLayerIndex);
	if(nSuc != 0)
		return nSuc;
	//需要释放原来的内存
	nSuc = DeleteLayer(sLayerIndex,TRUE);
	UpdateMapEnvelop();

	return nSuc;
}
int	MSMapObj::SwapLayer(string sSrcLayerIndex, string sDstLayerIndex)
{
	MSLayerObj* pSrcLayerObj = GetLayer(sSrcLayerIndex);
	MSLayerObj* pDstLayerObj = GetLayer(sDstLayerIndex);
	if(!pDstLayerObj || !pSrcLayerObj)
		ASSERT_FALSE_AND_RETURN_ERROR;
	MSLayerObj* pTmpLayer = pSrcLayerObj;
	int nSuc = SetLayer(sSrcLayerIndex,pDstLayerObj);
	if(nSuc != 0)
		ASSERT_FALSE_AND_RETURN_ERROR;
	nSuc = SetLayer(sDstLayerIndex,pTmpLayer);
	return nSuc;
}
int	MSMapObj::MoveLayerUp(string sLayerIndex)
{
	string sCurLayerIndex = sLayerIndex;
	int nFind = (int)sLayerIndex.rfind("_");
	string sParent;
	if(nFind > 0)
	{
		sParent = sLayerIndex.substr(0,nFind);
		sLayerIndex = sLayerIndex.substr(nFind+1);
	}
	char cTmp[255];
	itoa(atoi(sLayerIndex.c_str())-1,cTmp,10);
	if(strcmp(cTmp,"-1") == 0)
		ASSERT_FALSE_AND_RETURN_ERROR;
	string sPreLayerIndex = sParent + string(cTmp);
	if(!GetLayer(sPreLayerIndex))
		ASSERT_FALSE_AND_RETURN_ERROR;
	return SwapLayer(sCurLayerIndex,sPreLayerIndex);
}
int	MSMapObj::MoveLayerDown(string sLayerIndex)
{
	string sCurLayerIndex = sLayerIndex;
	int nFind = (int)sLayerIndex.rfind("_");
	string sParent;
	if(nFind > 0)
	{
		sParent = sLayerIndex.substr(0,nFind);
		sLayerIndex = sLayerIndex.substr(nFind+1);
	}
	char cTmp[255];
	itoa(atoi(sLayerIndex.c_str())+1,cTmp,10);
	string sNextLayerIndex = sParent + string(cTmp);
	if(!GetLayer(sNextLayerIndex))
		ASSERT_FALSE_AND_RETURN_ERROR;
	return SwapLayer(sCurLayerIndex,sNextLayerIndex);
}
int	MSMapObj::MoveLayerToTop(string sLayerIndex)
{
	if (sLayerIndex == "0")
		return 0;
	MSLayerObj* pLayer = GetLayer(sLayerIndex);
	if(!pLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;
	int nSuc = DeleteLayer(sLayerIndex,FALSE);
	if(nSuc != 0)
		ASSERT_FALSE_AND_RETURN_ERROR;
	return InsertLayerBefore(pLayer,"0");
}
int	MSMapObj::MoveLayerToBottom(string sLayerIndex)
{
	MSLayerObj* pLayer = GetLayer(sLayerIndex);
	if(!pLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;
	int nSuc = DeleteLayer(sLayerIndex,FALSE);
	if(nSuc != 0)
		ASSERT_FALSE_AND_RETURN_ERROR;
	//把刚才的层插到最底
	return InsertRootLayerAfter(pLayer,GetRootLayerCount()-1);
}
int	MSMapObj::MoveLayerToPosition(string sSrcLayerIndex,string sParentLayerIndex,string sPreSiblingLayerIndex)
{
	int nSuc;
// 	CString str;
// 	str = "Move: " + CString(sSrcLayerIndex.c_str()) + " To Parent: " + CString(sParentLayerIndex.c_str()) + " Sibling: " + CString(sPreSiblingLayerIndex.c_str()) + "\n";
// 	TRACE0(str);

	if(sSrcLayerIndex == sParentLayerIndex || sSrcLayerIndex == sPreSiblingLayerIndex)
		return 0;
	if(sSrcLayerIndex == "0" && sParentLayerIndex == "" && sPreSiblingLayerIndex == "")
		return 0;
	//分析数据
	int nNumSrc = -100;
	int nNumPreSibling = -100;
	string sSrcParent = "",sDstParent = sParentLayerIndex;

	//判断是从下往上移动，还是从上往下移动,比较 sSrcLayerIndex 与 sPreSiblingLayerIndex
	bool bFromUpToDown = false;
	string sS = sSrcLayerIndex,sP = sPreSiblingLayerIndex;
	if (sP == "" && sParentLayerIndex != "")
		sP = sParentLayerIndex + "_";

	int nNum = 0;
	while(true)
	{
		int nSrcFind = sS.find("_");
		if(nSrcFind >= 0)
		{
			nNumSrc = atoi(sS.substr(0,nSrcFind).c_str());
			sS = sS.substr(nSrcFind+1);
		}
		else if(sS == "")
			nNumSrc = -1;
		else
		{
			nNumSrc = atoi(sS.c_str());
			sS = "";
		}

		int nPreSiblingFind = sP.find("_");
		if(nPreSiblingFind >= 0)
		{
			nNumPreSibling = atoi(sP.substr(0,nPreSiblingFind).c_str());
			sP = sP.substr(nPreSiblingFind+1);
		}
		else if(sP == "")
			nNumPreSibling = -1;
		else
		{
			nNumPreSibling = atoi(sP.c_str());
			sP = "";
		}

		if (nNumSrc > nNumPreSibling)
		{
			bFromUpToDown = false;
			break;
		}
		else if (nNumSrc < nNumPreSibling)
		{
			bFromUpToDown = true;
			break;
		}
		nNum++;
		if(nNum >= 10)break;
	}

	MSLayerObj* pSrcLayer = GetLayer(sSrcLayerIndex);

	if (bFromUpToDown)	//从上到下，先增后删
	{
		if (sPreSiblingLayerIndex == "")
		{
			if(sDstParent == "")
				nSuc = InsertLayerBefore(pSrcLayer,"0");
			else
			{
				MSGroupObj* pGroupL = (MSGroupObj*)GetLayer(sDstParent);
				if(!pGroupL)
					ASSERT_FALSE_AND_RETURN_ERROR;
				if(pGroupL->GetLayerCount() == 0)//如果里面没有 子
					nSuc = InsertLayerToGroup(pSrcLayer,sDstParent);
				else
					nSuc = InsertLayerBefore(pSrcLayer,sDstParent+"_0");
			}
		}
		else
		{
			nSuc = InsertLayerAfter(pSrcLayer,sPreSiblingLayerIndex);
		}
		if(nSuc != 0)
			ASSERT_FALSE_AND_RETURN_ERROR;
		nSuc = DeleteLayer(sSrcLayerIndex,FALSE);
		if(nSuc != 0)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}
	else					//从下到上，先删后增
	{
		nSuc = DeleteLayer(sSrcLayerIndex,FALSE);
		if(nSuc != 0)
			ASSERT_FALSE_AND_RETURN_ERROR;
		if (sPreSiblingLayerIndex == "")
		{
			if(sDstParent == "")
				nSuc = InsertLayerBefore(pSrcLayer,"0");
			else
			{
				MSGroupObj* pGroupL = (MSGroupObj*)GetLayer(sDstParent);
				if(!pGroupL)
					ASSERT_FALSE_AND_RETURN_ERROR;
				if(pGroupL->GetLayerCount() == 0)//如果里面没有 子
					nSuc = InsertLayerToGroup(pSrcLayer,sDstParent);
				else
					nSuc = InsertLayerBefore(pSrcLayer,sDstParent+"_0");
			}
		}
		else
		{
			nSuc = InsertLayerAfter(pSrcLayer,sPreSiblingLayerIndex);
		}
		if(nSuc != 0)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}
	UpdateMapEnvelop();
	return 0;
}
int	MSMapObj::DeleteLayer(string sLayerIndex,BOOL bReleaseMemory)
{
	MSLayerObj* pCurLayer = GetLayer(sLayerIndex);
	if(!pCurLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;

	MSLayerObj* pParentLayer = NULL;
	int nFind = (int)sLayerIndex.rfind("_");
	if(nFind > 0)
	{
		string sParent = sLayerIndex.substr(0,nFind);
		pParentLayer = GetLayer(sParent);
		if(!pParentLayer)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}

	if (pParentLayer)
	{
		MSGroupObj* pGroupLayer = (MSGroupObj*)pParentLayer;
		if(!pGroupLayer || pParentLayer->GetLayerType() != MS_LAYER_GROUP)
			ASSERT_FALSE_AND_RETURN_ERROR;

		int nIndex = -1;
		for (int i=0;i<pGroupLayer->GetLayerCount();i++)
		{
			if (pGroupLayer->GetLayer(i) == pCurLayer)
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex < 0 || nIndex >= pGroupLayer->GetLayerCount())
			ASSERT_FALSE_AND_RETURN_ERROR;
		return pGroupLayer->DeleteLayer(nIndex,bReleaseMemory);
	}
	else
	{
		//根目录删除
		int nIndex = atoi(sLayerIndex.c_str());
		int nReturn = DeleteRootLayer(nIndex,bReleaseMemory);//里面已经更新了 Envelop

		return nReturn;
	}
	return -1;
}
int	MSMapObj::SetLayer(string sLayerIndex,MSLayerObj* pLayer)
{
	//与ReplaceLayer的差别在于此函数不需要释放所占用的内存
	if(!pLayer)return -1;
	MSLayerObj* pCurLayer = GetLayer(sLayerIndex);
	if(!pCurLayer)
		ASSERT_FALSE_AND_RETURN_ERROR;

	MSLayerObj* pParentLayer = NULL;
	int nFind = (int)sLayerIndex.rfind("_");
	if(nFind > 0)
	{
		string sParent = sLayerIndex.substr(0,nFind);
		pParentLayer = GetLayer(sParent);
		if(!pParentLayer)
			ASSERT_FALSE_AND_RETURN_ERROR;
	}
	if (pParentLayer)
	{
		MSGroupObj* pGroupLayer = (MSGroupObj*)pParentLayer;
		if(!pGroupLayer || pParentLayer->GetLayerType() != MS_LAYER_GROUP)
			ASSERT_FALSE_AND_RETURN_ERROR;

		int nIndex = -1;
		for (int i=0;i<pGroupLayer->GetLayerCount();i++)
		{
			if (pGroupLayer->GetLayer(i) == pCurLayer)
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex < 0 || nIndex >= pGroupLayer->GetLayerCount())
			ASSERT_FALSE_AND_RETURN_ERROR;
		//如果SetLayer(***,NULL)，则只是将该位置原来的指针删除
		if(nIndex == pGroupLayer->GetLayerCount()-1)
			pGroupLayer->AddLayer(pLayer);
		else
			pGroupLayer->InsertLayer(nIndex+1,pLayer);
		pGroupLayer->m_layers.erase(pGroupLayer->m_layers.begin()+nIndex);
	}
	else
	{
		//根目录先插入，再从根上仅删除对应的指针，不删除数据
		int nIndex = atoi(sLayerIndex.c_str());
		InsertRootLayerAfter(pLayer,nIndex);
		m_VctLayers.erase(m_VctLayers.begin()+nIndex);
	}
	UpdateMapEnvelop();

	return 0;
}

MSLayerObj*		MSMapObj::GetFirstValidLayer()
{
	if (GetRootLayerCount() == 0)
		return NULL;
	//clear
	m_nTmpLayerIndexForGetNextValidLayer = -1;
	m_VctValidLayers.clear();
	//loop all the valid layers
	for (int i = 0; i < GetRootLayerCount();i++)
	{
		MSLayerObj* pLayer = GetRootLayer(i);
		MSLayerType type = pLayer->GetLayerType();
		if (type == MS_LAYER_POINT || type == MS_LAYER_LINE || type == MS_LAYER_POLYGON || type == MS_LAYER_RASTER)
		{
			m_VctValidLayers.push_back(pLayer);
		}
		else if (type == MS_LAYER_GROUP)
		{
			LoopValidLayer(pLayer);
		}
		else
			ASSERT_FALSE_AND_RETURN_NULL;
	}

	m_nTmpLayerIndexForGetNextValidLayer++;
	if (m_VctValidLayers.size() > m_nTmpLayerIndexForGetNextValidLayer)
	{
		return m_VctValidLayers.at(m_nTmpLayerIndexForGetNextValidLayer);
	}
	return NULL;
}
MSLayerObj*		MSMapObj::GetNextValidLayer()
{
	if (m_nTmpLayerIndexForGetNextValidLayer == -1)
		ASSERT_FALSE_AND_RETURN_NULL;

	m_nTmpLayerIndexForGetNextValidLayer++;
	if (m_VctValidLayers.size() > m_nTmpLayerIndexForGetNextValidLayer)
	{
		return m_VctValidLayers.at(m_nTmpLayerIndexForGetNextValidLayer);
	}
	return NULL;
}
void			MSMapObj::LoopValidLayer(MSLayerObj* pParent)
{
	if (pParent->GetLayerType() == MS_LAYER_GROUP)
	{
		MSGroupObj* pGroup = (MSGroupObj*)pParent;
		for (int i = 0; i < pGroup->GetLayerCount();i++)
		{
			MSLayerObj* pLayer = pGroup->GetLayer(i);
			MSLayerType type = pLayer->GetLayerType();
			if (type == MS_LAYER_POINT || type == MS_LAYER_LINE || type == MS_LAYER_POLYGON || type == MS_LAYER_RASTER)
			{
				m_VctValidLayers.push_back(pLayer);
			}
			else if (type == MS_LAYER_GROUP)
			{
				LoopValidLayer(pLayer);
			}
		}
	}
}
void			MSMapObj::UpdateMapEnvelop()
{
	m_pMapEnvelopObj_Whole->Reset();
	MSLayerObj* pLayer = GetFirstValidLayer();
	while (pLayer)
	{
		MSEnvelopObj* pEnvelop = pLayer->GetEnvelopObj();
		m_pMapEnvelopObj_Whole->Merge(pEnvelop);
		pLayer = GetNextValidLayer();
	}
// 	int nCount = GetRootLayerCount();
// 	for (int i = 0; i < nCount; i++)
// 	{
// 		MSLayerObj* pLayer = GetRootLayer(i);
// 		MSEnvelopObj* pEnvelop;
// 		if (pLayer->GetLayerType() == MS_LAYER_GROUP)
// 		{
// 			MSGroupObj* pGroup = (MSGroupObj*)pLayer;
// 			pEnvelop = pGroup->GetEnvelopObj_Whole();
// 		}
// 		else
// 			pEnvelop = pLayer->GetEnvelopObj_Whole();
// 		m_pMapEnvelopObj_Whole->Merge(pEnvelop);
// 	}
}
