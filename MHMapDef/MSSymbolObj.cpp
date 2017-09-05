#include "stdafx.h"
#include "MSSymbolObj.h"
#include <algorithm>

//************************************************************************
//
// class MSSymbolObj
//
//************************************************************************
MSSymbolObj::MSSymbolObj(void)
{
	m_eObjType = MS_OBJ_SYMBOL;
	_symbolType = MS_NULL_SYMBOL;
	_visible = true;
}


MSSymbolObj::MSSymbolObj(const MSSymbolObj& srcObj) : MSObj(srcObj)
{
	m_eObjType = srcObj.m_eObjType;
	_symbolType = srcObj._symbolType;
	_visible = srcObj._visible;
}


MSSymbolObj::~MSSymbolObj(void)
{

}


void MSSymbolObj::SetSymbolType( MSSymbolType type )
{
	_symbolType = type;
}


MSSymbolType MSSymbolObj::GetSymbolType()
{
	return _symbolType;
}
bool		MSSymbolObj::GetVisible() const 
{ 
	return _visible; 
}
void		MSSymbolObj::SetVisible(bool val/* = true*/) 
{ 
	_visible = val; 
}


//************************************************************************
//
//class MSTextSymbolObj
//
//************************************************************************
//MSTextSymbolObj::MSTextSymbolObj( void )
//{
//	((MSSymbolObj*)(this))->_symbolType = MS_SIMPLE_TEXT_SYMBOL;
//
//	_color._colorR = 0; _color._colorG = 0;
//	_color._colorB = 0; _color._colorA = 255;
//
//	_font._fontName[0] = '\0'; 
//	_font._blod = _font._italic = _font._underline = false;
//
//	_size = 0;
//	_interval = 0;
//	_horizontalAlignment = HORIZONTAL_LEFT_ALIGMENT;
//	_verticalAlignment = VERTICAL_TOP_ALIGMENT;
//	_offsetX = 0;
//	_offsetY = 0;
//}
//
//
//MSTextSymbolObj::~MSTextSymbolObj( void )
//{
//
//}
//
//
//void MSTextSymbolObj::SetColor(MSColor dstColor)
//{
//	_color = dstColor;
//}
//
//
//void MSTextSymbolObj::SetSize( double dSize )
//{
//#ifdef GDOSMAP_EXCEPTION
//	if(nSize<0)
//	{
//		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
//	}
//#endif
//	_size = dSize;
//}
//
//
//void MSTextSymbolObj::SetFontName( const char* strName )
//{
//	assert(strName!=0);
//	if(strName==0)return;
//
//	strcpy(_font._fontName, strName);
//}
//
//
//void MSTextSymbolObj::SetFontItalic( bool bb )
//{
//	_font._italic = bb;
//}
//
//
//void MSTextSymbolObj::SetFontBold( bool bb )
//{
//	_font._blod = bb;
//}
//
//void MSTextSymbolObj::SetFontUnderline( bool bb )
//{
//	_font._underline = bb;
//}
//
//void MSTextSymbolObj::SetInterval( double interval )
//{
//#ifdef GDOSMAP_EXCEPTION
//	if(interval<0)
//	{
//		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
//	}
//#endif
//	_interval = interval;
//}
//
//
//void MSTextSymbolObj::SetHorizAllig( MSHorizontalAligmentType type )
//{
//	_horizontalAlignment = type;
//}
//
//
//void MSTextSymbolObj::SetVertiAllig( MSVerticalAligmentType type )
//{
//	_verticalAlignment = type;
//}
//
//
//void MSTextSymbolObj::SetOffsetX( double offsetX )
//{
//	_offsetX = offsetX;
//}
//
//
//void MSTextSymbolObj::SetOffsetY( double offsetY )
//{
//	_offsetY = offsetY;
//}
//
//
//MSColor MSTextSymbolObj::GetColor()
//{
//	return _color;
//}
//
//
//double MSTextSymbolObj::GetSize()
//{
//	return _size;
//}
//
//
//char* MSTextSymbolObj::GetFontName()
//{
//	return _font._fontName;
//}
//
//
//bool MSTextSymbolObj::GetFontItalic()
//{
//	return _font._italic;
//}
//
//
//bool MSTextSymbolObj::GetFontBold()
//{
//	return _font._blod;
//}
//
//
//bool MSTextSymbolObj::GetFontUnderline()
//{
//	return _font._underline;
//}
//
//
//double MSTextSymbolObj::GetInterval()
//{
//	return _interval;
//}
//
//
//MSHorizontalAligmentType MSTextSymbolObj::GetHorizAllig()
//{
//	return _horizontalAlignment;
//}
//
//
//MSVerticalAligmentType MSTextSymbolObj::GetVertiAllig()
//{
//	return _verticalAlignment;
//}
//
//
//double MSTextSymbolObj::GetOffsetX()
//{
//	return _offsetX;
//}
//
//
//double MSTextSymbolObj::GetOffsetY()
//{
//	return _offsetY;
//}

//************************************************************************
//
// class MSMarkerSymbolBaseObj
//
//************************************************************************
MSMarkerSymbolBaseObj::MSMarkerSymbolBaseObj( void )
{
	SetSymbolType(MS_NULL_SYMBOL);

	m_colorEdge._colorR = 0; m_colorEdge._colorG = 0;
	m_colorEdge._colorB = 0; m_colorEdge._colorA = 255;

	m_colorFill._colorR = 255; m_colorFill._colorG = 255;
	m_colorFill._colorB = 255; m_colorFill._colorA = 255;

	//默认情况下，高亮的为透明，即没有高亮
	m_colorHighlight._colorR = 0; m_colorHighlight._colorG = 0;
	m_colorHighlight._colorB = 0; m_colorHighlight._colorA = 0;

	m_dAngle = 0; 
	m_dSize = 8;
	m_dOffsetX = 0;
	m_dOffsetY = 0;

}
MSMarkerSymbolBaseObj::MSMarkerSymbolBaseObj(const MSMarkerSymbolBaseObj& srcObj)
{
	SetSymbolType(MS_NULL_SYMBOL);
	m_colorEdge = srcObj.m_colorEdge;
	m_colorFill = srcObj.m_colorFill;
	m_colorHighlight = srcObj.m_colorHighlight;
	m_dAngle = srcObj.m_dAngle;
	m_dSize = srcObj.m_dSize;
	m_dOffsetX = srcObj.m_dOffsetX;
	m_dOffsetY = srcObj.m_dOffsetY;
}
MSMarkerSymbolBaseObj::~MSMarkerSymbolBaseObj( void )
{

}


void MSMarkerSymbolBaseObj::SetEdgeColor( MSColor dstColor )
{
	m_colorEdge = dstColor;
}


void MSMarkerSymbolBaseObj::SetFillColor( MSColor dstColor )
{
	m_colorFill = dstColor;
}


void MSMarkerSymbolBaseObj::SetHightLightColor( MSColor dstColor )
{
	m_colorHighlight = dstColor;
}

void MSMarkerSymbolBaseObj::SetAngle(double angel)
{
	m_dAngle = angel;
}


void MSMarkerSymbolBaseObj::SetOffsetX(double offsetX)
{
	m_dOffsetX = offsetX;
}


void MSMarkerSymbolBaseObj::SetOffsetY(double offsetY)
{
	m_dOffsetY = offsetY;
}


void MSMarkerSymbolBaseObj::SetSize(double dSize)
{
#ifdef GDOSMAP_EXCEPTION
	if(dSize<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
	}
#endif
	m_dSize = dSize;
}


MSColor	MSMarkerSymbolBaseObj::GetEdgeColor()
{
	return m_colorEdge;
}


MSColor	MSMarkerSymbolBaseObj::GetFillColor()
{
	return m_colorFill;
}


MSColor	MSMarkerSymbolBaseObj::GetHightLightColor()
{
	return m_colorHighlight;
}


double	MSMarkerSymbolBaseObj::GetSize()
{
	return m_dSize;
}


double	MSMarkerSymbolBaseObj::GetAngle()
{
	return m_dAngle;
}


double	MSMarkerSymbolBaseObj::GetOffsetX()
{
	return m_dOffsetX;
}


double	MSMarkerSymbolBaseObj::GetOffsetY()
{
	return m_dOffsetY;
}




//************************************************************************
//
// class MSSimpleMarkerSymbolObj
//
//************************************************************************
MSSimpleMarkerSymbolObj::MSSimpleMarkerSymbolObj( void )
{
	SetSymbolType(MS_SIMPLE_MARKER_SYMBOL);

	m_eMarkerType = MS_MARKER_CIRCLE;
	m_nMultiMarkerType = 0;
	m_dEdgeLineWidth = 1;
}
MSSimpleMarkerSymbolObj::MSSimpleMarkerSymbolObj(const MSSimpleMarkerSymbolObj& srcObj)
{
	SetSymbolType(MS_SIMPLE_MARKER_SYMBOL);
	m_eMarkerType = srcObj.m_eMarkerType;
	m_nMultiMarkerType = srcObj.m_nMultiMarkerType;
	m_dEdgeLineWidth = srcObj.m_dEdgeLineWidth;
	
	MSSimpleMarkerSymbolObj* pSourceSimpleMarkerSymbolObj = (MSSimpleMarkerSymbolObj*)&srcObj;
	SetMarkerType(pSourceSimpleMarkerSymbolObj->GetMarkerType());
	SetSize(pSourceSimpleMarkerSymbolObj->GetSize());
	SetEdgeLineWidth(pSourceSimpleMarkerSymbolObj->GetEdgeLineWidth());
	SetEdgeColor(pSourceSimpleMarkerSymbolObj->GetEdgeColor());
	SetFillColor(pSourceSimpleMarkerSymbolObj->GetFillColor());
	SetAngle(pSourceSimpleMarkerSymbolObj->GetAngle());
	SetHightLightColor(pSourceSimpleMarkerSymbolObj->GetHightLightColor());
	SetOffsetX(pSourceSimpleMarkerSymbolObj->GetOffsetX());
	SetOffsetY(pSourceSimpleMarkerSymbolObj->GetOffsetY());

}
MSSimpleMarkerSymbolObj::~MSSimpleMarkerSymbolObj( void )
{

}
void MSSimpleMarkerSymbolObj::SetMarkerType(MSMarkerType type )
{
	m_eMarkerType = type;
}
MSMarkerType MSSimpleMarkerSymbolObj::GetMarkerType()
{
	return m_eMarkerType;
}
void MSSimpleMarkerSymbolObj::SetMultiMarkerType(int type)
{
	m_nMultiMarkerType = type;
}
int	MSSimpleMarkerSymbolObj::GetMultiMarkerType()
{
	return m_nMultiMarkerType;
}
double		MSSimpleMarkerSymbolObj::GetEdgeLineWidth() const 
{ 
	return m_dEdgeLineWidth; 
}
void	MSSimpleMarkerSymbolObj::SetEdgeLineWidth(double val) 
{ 
	m_dEdgeLineWidth = val; 
}



//************************************************************************
//
// class MSPictureMarkerSymbolObj
//
//************************************************************************
MSPictureMarkerSymbolObj::MSPictureMarkerSymbolObj( void )
{
	SetSymbolType(MS_PICTURE_MARKER_SYMBOL);
	_picture._imageBuf[0] = '\0';
}
MSPictureMarkerSymbolObj::MSPictureMarkerSymbolObj(const MSPictureMarkerSymbolObj& srcObj)
{
	SetSymbolType(MS_PICTURE_MARKER_SYMBOL);
	_picture = srcObj._picture;
}

MSPictureMarkerSymbolObj::~MSPictureMarkerSymbolObj( void )
{

}


void MSPictureMarkerSymbolObj::SetPictureFilename(const char* buffer)
{
	assert(buffer!=0);
	if(buffer==0)return;

	strcpy(_picture._imageBuf, buffer);
}


// void MSPictureMarkerSymbolObj::SetPictureBufferLen(int len)
// {
// 	_picture._imageLen = len;
// }
// 
// 
// void MSPictureMarkerSymbolObj::SetPictureType(MSPictureType type)
// {
// 	_picture._pictureType = type;
// }


char* MSPictureMarkerSymbolObj::GetPictureFilename()
{
	return _picture._imageBuf;
}


// int	MSPictureMarkerSymbolObj::GetPictureBufferLen()
// {
// 	return _picture._imageLen;
// }
// 
// 
// MSPictureType MSPictureMarkerSymbolObj::GetPictureType()
// {
// 	return _picture._pictureType;
// }


void MSPictureMarkerSymbolObj::SetScaleX( double scalex )
{
#ifdef GDOSMAP_EXCEPTION
	if(scalex<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
	}
#endif
	_picture._scaleX = scalex;
}


void MSPictureMarkerSymbolObj::SetScaleY( double scaleY )
{
#ifdef GDOSMAP_EXCEPTION
	if(scaleY<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
	}
#endif
	_picture._scaleY = scaleY;
}


double MSPictureMarkerSymbolObj::GetScaleX()
{
	return _picture._scaleX;
}


double MSPictureMarkerSymbolObj::GetScaleY()
{
	return _picture._scaleY;
}




//************************************************************************
//
// class MSCharacterMarkerSymbolObj
//
//************************************************************************
MSCharacterMarkerSymbolObj::MSCharacterMarkerSymbolObj( void )
{
	SetSymbolType(MS_CHARACTER_MARKER_SYMBOL);

	m_fontCharacter._fontName[0] = '\0'; 
	m_fontCharacter._blod = m_fontCharacter._italic = m_fontCharacter._underline = false;

	m_nCharacter = 'G';

	m_colorEdge = _MSColor(0,0,0,0);
	m_bUseBigEdge = false;
}
MSCharacterMarkerSymbolObj::MSCharacterMarkerSymbolObj(const MSCharacterMarkerSymbolObj& srcObj)
{
	SetSymbolType(MS_CHARACTER_MARKER_SYMBOL);

	m_fontCharacter = srcObj.m_fontCharacter;
	m_nCharacter = srcObj.m_nCharacter;
	m_colorEdge = srcObj.m_colorEdge;
	m_bUseBigEdge = srcObj.m_bUseBigEdge;
}

MSCharacterMarkerSymbolObj::~MSCharacterMarkerSymbolObj( void )
{

}


void MSCharacterMarkerSymbolObj::SetFontName(const char* strName)
{
	assert(strName!=0);
	if(strName==0)return;

	strcpy(m_fontCharacter._fontName, strName);
}


void MSCharacterMarkerSymbolObj::SetFontItalic(bool bb)
{
	m_fontCharacter._italic = bb;
}


void MSCharacterMarkerSymbolObj::SetFontBold(bool bb)
{
	m_fontCharacter._blod = bb;
}


void MSCharacterMarkerSymbolObj::SetFontUnderline(bool bb)
{
	m_fontCharacter._underline = bb;
}


void MSCharacterMarkerSymbolObj::SetFontStrikeout(bool bb)
{
	m_fontCharacter._strikeout = bb;
}


void MSCharacterMarkerSymbolObj::SetCharacter(wchar_t ch)
{
	m_nCharacter = ch;
}


char* MSCharacterMarkerSymbolObj::GetFontName()
{
	return m_fontCharacter._fontName;
}


bool MSCharacterMarkerSymbolObj::GetFontItalic()
{
	return m_fontCharacter._italic;
}


bool MSCharacterMarkerSymbolObj::GetFontBold()
{
	return m_fontCharacter._blod;
}


bool MSCharacterMarkerSymbolObj::GetFontUnderline()
{
	return m_fontCharacter._underline;
}


bool MSCharacterMarkerSymbolObj::GetFontStrikeout()
{
	return m_fontCharacter._strikeout;
}


wchar_t MSCharacterMarkerSymbolObj::GetCharacter()
{
	return m_nCharacter;
}

void	MSCharacterMarkerSymbolObj::SetEdgeColor(MSColor egColor)
{
	m_colorEdge = egColor;
}
MSColor	MSCharacterMarkerSymbolObj::GetEdgeColor()
{
	return m_colorEdge;
}
void	MSCharacterMarkerSymbolObj::SetUseBigEdge(bool bUseBigEdge/* = true*/)
{
	m_bUseBigEdge = bUseBigEdge;
}
bool	MSCharacterMarkerSymbolObj::GetUseBigEdge()
{
	return m_bUseBigEdge;
}



//************************************************************************
//
// class MSMultiLayerMarkSymbolObj
//
//************************************************************************
MSMultiLayerMarkSymbolObj::MSMultiLayerMarkSymbolObj( void )
{
	SetSymbolType(MS_MULTILAYER_MARKER_SYMBOL);

	m_vMarkerSymbol.clear();
}
MSMultiLayerMarkSymbolObj::MSMultiLayerMarkSymbolObj(const MSMultiLayerMarkSymbolObj& srcObj)
{
	SetSymbolType(MS_MULTILAYER_MARKER_SYMBOL);

	MSMultiLayerMarkSymbolObj *pMMS = &((MSMultiLayerMarkSymbolObj)srcObj);
	for (int i = 0; i < pMMS->GetMarkSymbolsNum(); i++)
	{
		MSMarkerSymbolBaseObj* pMarkerSymbolBaseObj = pMMS->GetMarkSymbol(i);
		MSMarkerSymbolBaseObj* pNewMarkerSymbolBaseObj = NULL;
		MSSymbolType type = pMarkerSymbolBaseObj->GetSymbolType();
		if(type == MS_SIMPLE_MARKER_SYMBOL)
		{
			MSSimpleMarkerSymbolObj* pSrcSMS = (MSSimpleMarkerSymbolObj*)pMarkerSymbolBaseObj;
			pNewMarkerSymbolBaseObj = new MSSimpleMarkerSymbolObj(*pSrcSMS);
		}
		else if (type == MS_CHARACTER_MARKER_SYMBOL)
		{
			MSCharacterMarkerSymbolObj* pSrcCMS = (MSCharacterMarkerSymbolObj*)pMarkerSymbolBaseObj;
			pNewMarkerSymbolBaseObj = new MSCharacterMarkerSymbolObj(*pSrcCMS);
		}
		else if (type == MS_PICTURE_MARKER_SYMBOL)
		{
			MSPictureMarkerSymbolObj* pSrcPMS = (MSPictureMarkerSymbolObj*)pMarkerSymbolBaseObj;
			pNewMarkerSymbolBaseObj = new MSPictureMarkerSymbolObj(*pSrcPMS);
		}
		else
			ASSERT_FALSE_AND_CONTINUE;
		AppendMarkSymbol(pNewMarkerSymbolBaseObj);
	}
}

MSMultiLayerMarkSymbolObj::~MSMultiLayerMarkSymbolObj( void )
{
	int nSize = (int)m_vMarkerSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSMarkerSymbolBaseObj* pBase = GetMarkSymbol(i);
		delete pBase;
		pBase = 0;
	}
	m_vMarkerSymbol.clear();
}


int MSMultiLayerMarkSymbolObj::AppendMarkSymbol( MSMarkerSymbolBaseObj* pSymbol )
{
	if(pSymbol==0) return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type < MS_SIMPLE_MARKER_SYMBOL || type > MS_PICTURE_MARKER_SYMBOL))
	{
		assert(0);
		return -1;
	}

	m_vMarkerSymbol.push_back(pSymbol);

	return 0;
}


int MSMultiLayerMarkSymbolObj::ReplaceMarkerSymbol( MSMarkerSymbolBaseObj* newpSymbol, int iIndex )
{
	if(!newpSymbol)
		return -1;

	MSSymbolType type = newpSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_MARKER_SYMBOL) && 
		(type!=MS_PICTURE_MARKER_SYMBOL) && 
		(type!=MS_CHARACTER_MARKER_SYMBOL))
	{
		assert(false);
		return -1;
	}

	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)
		return -1;

	//shenzf 释放原来的内存
	MSMarkerSymbolBaseObj* oldMarker = m_vMarkerSymbol.at(iIndex);
	delete oldMarker;

	m_vMarkerSymbol.at(iIndex) = newpSymbol;

	return 0;
}


int MSMultiLayerMarkSymbolObj::InsertBefore( MSMarkerSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_MARKER_SYMBOL) && 
		(type!=MS_PICTURE_MARKER_SYMBOL) && 
		(type!=MS_CHARACTER_MARKER_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = m_vMarkerSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	m_vMarkerSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerMarkSymbolObj::InsertAfter( MSMarkerSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_MARKER_SYMBOL) && 
		(type!=MS_PICTURE_MARKER_SYMBOL) && 
		(type!=MS_CHARACTER_MARKER_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || iIndex>nSize)return -1;

	ITT itt = m_vMarkerSymbol.begin();
	for(int i=0; i<=iIndex; i++)itt++;

	m_vMarkerSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerMarkSymbolObj::RemoveSymbol( MSMarkerSymbolBaseObj* pSymbol )
{
	int nSize = (int)m_vMarkerSymbol.size();
	ITT itt = m_vMarkerSymbol.begin();
	for(int i=0; i<nSize; i++, itt++)
	{
		MSMarkerSymbolBaseObj* pBase = GetMarkSymbol(i);
		if(pBase==pSymbol)
		{
			//shenzf 释放内存
			delete pBase;

			m_vMarkerSymbol.erase(itt);
			return 0;
		}
	}
	return -1;
}


int MSMultiLayerMarkSymbolObj::RemoveSymbol( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = m_vMarkerSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	//shenzf 释放内存
	delete m_vMarkerSymbol.at(iIndex);

	m_vMarkerSymbol.erase(itt);

	return 0;
}


int MSMultiLayerMarkSymbolObj::RemoveAllSymbols()
{
	int nSize = (int)m_vMarkerSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSMarkerSymbolBaseObj* pBase = GetMarkSymbol(i);
		delete pBase;
		pBase = 0;
	}
	m_vMarkerSymbol.clear();

	return 0;
}


int MSMultiLayerMarkSymbolObj::SwapMarkSymbol( int iSrcIndex, int iDstIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(nSize<2 || iSrcIndex<0 || iDstIndex<0 || iSrcIndex>nSize-1 || iDstIndex>nSize-1)return -1;

	MSMarkerSymbolBaseObj* pA = m_vMarkerSymbol.at(iSrcIndex);
	MSMarkerSymbolBaseObj* pB = m_vMarkerSymbol.at(iDstIndex);

	m_vMarkerSymbol.at(iDstIndex) = pA;
	m_vMarkerSymbol.at(iSrcIndex) = pB;

	return 0;
}


int MSMultiLayerMarkSymbolObj::MoveUp( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex>nSize-1 || iIndex<1)return -1;

	return SwapMarkSymbol(iIndex, iIndex-1);
}


int MSMultiLayerMarkSymbolObj::MoveDown( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex>nSize-2 || iIndex<0)return -1;

	return SwapMarkSymbol(iIndex, iIndex+1);
}


int MSMultiLayerMarkSymbolObj::MoveToTop( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i>0; i--)
	{
		SwapMarkSymbol(i, i-1);
	}

	return 0;
}


int MSMultiLayerMarkSymbolObj::MoveToBottom( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i<nSize-1; i++)
	{
		SwapMarkSymbol(i, i+1);
	}

	return 0;
}


int MSMultiLayerMarkSymbolObj::ReverseAllSymbols()
{
	ITT beg = m_vMarkerSymbol.begin();
	ITT eed = m_vMarkerSymbol.end();

	reverse(beg, eed);

	return 0;
}


int MSMultiLayerMarkSymbolObj::GetMarkSymbolsNum()
{
	return (int)m_vMarkerSymbol.size();
}


MSMarkerSymbolBaseObj* MSMultiLayerMarkSymbolObj::GetMarkSymbol( int iIndex )
{
	int nSize = (int)m_vMarkerSymbol.size();
	if(iIndex>nSize-1 || iIndex<0)return 0;

	return (MSMarkerSymbolBaseObj*)(m_vMarkerSymbol.at(iIndex));
}



//************************************************************************
//
// class MSSimpleLineSymbolObj
//
//************************************************************************
MSLineSymbolBaseObj::MSLineSymbolBaseObj( void )
{
	SetSymbolType(MS_NULL_SYMBOL);

	m_colorLine._colorR = 0; m_colorLine._colorG = 0;
	m_colorLine._colorB = 0; m_colorLine._colorA = 255;

	m_colorHighlight._colorR = 255; m_colorHighlight._colorG = 0;
	m_colorHighlight._colorB = 0; m_colorHighlight._colorA = 0;

	m_dWidth = 1;

	m_eStart = MS_LineCapRound;
	m_eEnd = MS_LineCapRound;
	m_eDash = MS_DashCapRound;
	m_eJoin = MS_LineJoinRound;

	m_fOffset = 0;
// 	_offset = 0;
}


MSLineSymbolBaseObj::MSLineSymbolBaseObj(const MSLineSymbolBaseObj& srcObj) : MSSymbolObj(srcObj)
{
	SetSymbolType(MS_NULL_SYMBOL);
	m_colorLine = srcObj.m_colorLine;
	m_colorHighlight = srcObj.m_colorHighlight;
	m_dWidth = srcObj.m_dWidth;
	m_eStart = srcObj.m_eStart;
	m_eEnd = srcObj.m_eEnd;
	m_eDash = srcObj.m_eDash;
	m_eJoin = srcObj.m_eJoin;
	m_fOffset = srcObj.m_fOffset;
}


MSLineSymbolBaseObj::~MSLineSymbolBaseObj( void )
{

}


void MSLineSymbolBaseObj::SetColor(MSColor dstColor)
{
	m_colorLine = dstColor;
}


void MSLineSymbolBaseObj::SetHighLightColor(MSColor dstColor)
{
	m_colorHighlight = dstColor;
}


void MSLineSymbolBaseObj::SetWidth(double wid)
{
#ifdef GDOSMAP_EXCEPTION
	if(wid<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSUnitType");
	}
#endif
	m_dWidth = wid;
}


MSColor	MSLineSymbolBaseObj::GetColor()
{
	return m_colorLine;
}


MSColor	MSLineSymbolBaseObj::GetHighLightColor()
{
	return m_colorHighlight;
}

double MSLineSymbolBaseObj::GetWidth()
{
	return m_dWidth;
}
void MSLineSymbolBaseObj::SetLineCap(MSLineCap startCap, MSLineCap endCap, MSDashCap dashCap)
{
	m_eStart = startCap;
	m_eEnd = endCap;
	m_eDash = dashCap;
}
void MSLineSymbolBaseObj::GetLineCap(MSLineCap &startCap, MSLineCap &endCap, MSDashCap &dashCap)
{
	startCap = m_eStart;
	endCap = m_eEnd;
	dashCap = m_eDash;
}
void MSLineSymbolBaseObj::SetLineJoin(MSLineJoin join_type)
{
	m_eJoin = join_type;
}
MSLineJoin	MSLineSymbolBaseObj::GetLineJoin()
{
	return m_eJoin;
}
void MSLineSymbolBaseObj::SetOffset(float offset)
{
	m_fOffset = offset;
}
float MSLineSymbolBaseObj::GetOffset()
{
	return m_fOffset;
}




//************************************************************************
//
// class MSSimpleLineSymbolObj(simple)
//
//************************************************************************
MSSimpleLineSymbolObj::MSSimpleLineSymbolObj( void )
{
	SetSymbolType(MS_SIMPLE_LINE_SYMBOL);
}


MSSimpleLineSymbolObj::MSSimpleLineSymbolObj(const MSSimpleLineSymbolObj& srcObj)
{
	SetSymbolType(MS_SIMPLE_LINE_SYMBOL);

	MSSimpleLineSymbolObj* pSourceSimpleLineSymbolObj = (MSSimpleLineSymbolObj*)&srcObj;
	SetColor(pSourceSimpleLineSymbolObj->GetColor());
	SetHighLightColor(pSourceSimpleLineSymbolObj->GetHighLightColor());
	SetWidth(pSourceSimpleLineSymbolObj->GetWidth());
	MSLineCap startCap; MSLineCap endCap; MSDashCap dashCap;
	pSourceSimpleLineSymbolObj->GetLineCap(startCap, endCap, dashCap);
	SetLineCap(startCap, endCap, dashCap);
	SetLineJoin(pSourceSimpleLineSymbolObj->GetLineJoin());
	SetOffset(pSourceSimpleLineSymbolObj->GetOffset());
}


MSSimpleLineSymbolObj::~MSSimpleLineSymbolObj( void )
{

}



//************************************************************************
//
// class MSHashLineSymbolObj
//
//************************************************************************
MSHashLineSymbolObj::MSHashLineSymbolObj( void )
{
	SetSymbolType(MS_HASH_LINE_SYMBOL);

	m_dashStyle = MS_DashStyleDash;
	m_fDashArray = NULL;
	m_fDashArrayReal = NULL;
	m_nCount = 0;
}
MSHashLineSymbolObj::MSHashLineSymbolObj(const MSHashLineSymbolObj&srcObj)
{
	SetSymbolType(MS_HASH_LINE_SYMBOL);

	m_dashStyle = srcObj.m_dashStyle;
	m_fDashArray = srcObj.m_fDashArray;
	m_fDashArrayReal = srcObj.m_fDashArrayReal;
	m_nCount = srcObj.m_nCount;
}

MSHashLineSymbolObj::~MSHashLineSymbolObj( void )
{
	if(m_fDashArray)
	{
		delete []m_fDashArray;
		m_fDashArray = NULL;
	}
	if (m_fDashArrayReal)
	{
		delete []m_fDashArrayReal;
		m_fDashArrayReal = NULL;
	}
	m_nCount = 0;
}


void MSHashLineSymbolObj::SetDashPattern(float* dashArray,int nCount)
{
	m_nCount = nCount;
	if (m_fDashArray)
	{
		delete []m_fDashArray;
		m_fDashArray = NULL;
	}
	m_fDashArray = new float[nCount];
	for (int i=0;i<nCount;i++)
	{
		m_fDashArray[i] = dashArray[i]/m_dWidth;
		if(m_fDashArray[i] == 0)
			m_fDashArray[i] = 0.000001;
	}

	if (m_fDashArrayReal)
	{
		delete []m_fDashArrayReal;
		m_fDashArrayReal = NULL;
	}
	m_fDashArrayReal = new float[nCount];
	for (int i=0;i<nCount;i++)
	{
		m_fDashArrayReal[i] = dashArray[i];
	}
	SetDashStyle(MS_DashStyleCustom);
}
void MSHashLineSymbolObj::GetDashPattern(float*& dashArray,int& nCount)
{
	dashArray = m_fDashArray;
	nCount = m_nCount;
}
void MSHashLineSymbolObj::SetDashStyle(MSDashStyle style)
{
	m_dashStyle = style;
}
MSDashStyle MSHashLineSymbolObj::GetDashStyle()
{
	return m_dashStyle;
}
void MSHashLineSymbolObj::SetWidth(double wid)
{
	m_dWidth = wid;
	if (m_fDashArrayReal)
	{
		SetDashPattern(m_fDashArrayReal,m_nCount);
	}
}



//************************************************************************
//
// class MSMarkerLineSymbolObj
//
//************************************************************************
MSMarkerLineSymbolObj::MSMarkerLineSymbolObj( void )
{
	SetSymbolType(MS_MARKER_LINE_SYMBOL);

	m_dInterval = 0;
	m_pMarkSymbol = NULL;
}
MSMarkerLineSymbolObj::MSMarkerLineSymbolObj(const MSMarkerLineSymbolObj& srcObj)
{
	SetSymbolType(MS_MARKER_LINE_SYMBOL);

	m_dInterval = srcObj.m_dInterval;
	m_pMarkSymbol = srcObj.m_pMarkSymbol;
}

MSMarkerLineSymbolObj::~MSMarkerLineSymbolObj( void )
{
	if(m_pMarkSymbol != 0)
	{
		delete (MSSymbolObj*)(m_pMarkSymbol);
		m_pMarkSymbol = 0;
	}
}


void MSMarkerLineSymbolObj::SetInterval(double len)
{
#ifdef GDOSMAP_EXCEPTION
	if(len<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "Interval");
	}
#endif
	m_dInterval = len;
}

double	MSMarkerLineSymbolObj::GetInterval()
{
	return m_dInterval;
}


void MSMarkerLineSymbolObj::SetMarkerSymbol(MSSymbolObj* pSymbol)
{
	MSSymbolType type = pSymbol->GetSymbolType();

	bool bInvalidType = (type==MS_CHARACTER_MARKER_SYMBOL) || 
		(type==MS_SIMPLE_MARKER_SYMBOL) ||
		(type==MS_PICTURE_MARKER_SYMBOL) ||
		(type==MS_MULTILAYER_MARKER_SYMBOL);

#ifdef GDOSMAP_EXCEPTION
	if(bInvalidType==false)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSSymbolType");
	}
#endif

	assert(bInvalidType==true);

	m_pMarkSymbol = pSymbol;
}


MSSymbolObj* MSMarkerLineSymbolObj::GetMarkerSymbol()
{
	return (MSSymbolObj*)(m_pMarkSymbol);
}



//************************************************************************
//
// class MSPictureLineSymbolObj
//
//************************************************************************
MSPictureLineSymbolObj::MSPictureLineSymbolObj(void)
{
	SetSymbolType(MS_PICTURE_LINE_SYMBOL);

	m_dAngle = 0;
	m_pPicture._imageBuf[0] = '\0';
	m_eWarpMode = MS_WrapModeTile;
}
MSPictureLineSymbolObj::MSPictureLineSymbolObj(const MSPictureLineSymbolObj& srcObj)
{
	SetSymbolType(MS_PICTURE_LINE_SYMBOL);

	m_dAngle = srcObj.m_dAngle;
	m_pPicture = srcObj.m_pPicture;
	m_eWarpMode = srcObj.m_eWarpMode;
}
MSPictureLineSymbolObj::~MSPictureLineSymbolObj(void)
{
}
void MSPictureLineSymbolObj::SetPictureName(const char* name)
{
	assert(name);
	strcpy(m_pPicture._imageBuf, name);
}
// void MSPictureLineSymbolObj::SetPictureNameLen(int len)
// {
// 	_picture._imageLen = len;
// }
// void MSPictureLineSymbolObj::SetPictureType(MSPictureType type)
// {
// 	_picture._pictureType = type;
// }
void MSPictureLineSymbolObj::SetScaleX(double scalex)
{
	m_pPicture._scaleX = scalex;
}
void MSPictureLineSymbolObj::SetScaleY(double scaleY)
{
	m_pPicture._scaleY = scaleY;
}
void MSPictureLineSymbolObj::SetAngle(double angle)
{
	m_dAngle = angle;
}
char* MSPictureLineSymbolObj::GetPictureName()
{
	return m_pPicture._imageBuf;
}
double MSPictureLineSymbolObj::GetScaleX()
{
	return m_pPicture._scaleX;
}
double MSPictureLineSymbolObj::GetScaleY()
{
	return m_pPicture._scaleY;
}
double MSPictureLineSymbolObj::GetAngle()
{
	return m_dAngle;
}
void		MSPictureLineSymbolObj::SetWrapMode(MSWrapMode wrapMode)
{
	m_eWarpMode = wrapMode;
}
MSWrapMode	MSPictureLineSymbolObj::GetWrapMode()
{
	return m_eWarpMode;
}

//************************************************************************
//
// class MSMultiLayerLineSymbolObj
//
//************************************************************************
MSMultiLayerLineSymbolObj::MSMultiLayerLineSymbolObj( void )
{
	SetSymbolType(MS_MULTILAYER_LINE_SYMBOL);
	
	_pVctLineSymbol.clear();
}
MSMultiLayerLineSymbolObj::MSMultiLayerLineSymbolObj(const MSMultiLayerLineSymbolObj& srcObj)
{
	SetSymbolType(MS_MULTILAYER_LINE_SYMBOL);

	MSMultiLayerLineSymbolObj* pMLLS = &((MSMultiLayerLineSymbolObj)srcObj);
	for (int i=0;i<pMLLS->GetLineSymbolsNum();i++)
	{
		MSLineSymbolBaseObj* pLineSymbolBaseObj = pMLLS->GetLineSymbol(i);
		MSSymbolType type = pLineSymbolBaseObj->GetSymbolType();
		MSLineSymbolBaseObj* pNewLineSymbolBaseObj;
		if (type == MS_SIMPLE_LINE_SYMBOL)
		{
			MSSimpleLineSymbolObj* pSimpleLineSymbolObj = (MSSimpleLineSymbolObj*)pLineSymbolBaseObj;
			pNewLineSymbolBaseObj = new MSSimpleLineSymbolObj(*pSimpleLineSymbolObj);
		}
		else if (type == MS_HASH_LINE_SYMBOL)
		{
			MSHashLineSymbolObj* pHashLineSymbolObj = (MSHashLineSymbolObj*)pLineSymbolBaseObj;
			pNewLineSymbolBaseObj = new MSHashLineSymbolObj(*pHashLineSymbolObj);
		}
		else if (type == MS_MARKER_LINE_SYMBOL)
		{
			MSMarkerLineSymbolObj* pMarkerLineSymbolObj = (MSMarkerLineSymbolObj*)pLineSymbolBaseObj;
			pNewLineSymbolBaseObj = new MSMarkerLineSymbolObj(*pMarkerLineSymbolObj);
		}
		else if (type == MS_PICTURE_LINE_SYMBOL)
		{
			MSPictureLineSymbolObj* pPictureLineSymbolObj = (MSPictureLineSymbolObj*)pLineSymbolBaseObj;
			pNewLineSymbolBaseObj = new MSPictureLineSymbolObj(*pPictureLineSymbolObj);
		}
		else
			ASSERT_FALSE_AND_RETURN;
		AppendLineSymbol(pNewLineSymbolBaseObj);
	}
}

MSMultiLayerLineSymbolObj::~MSMultiLayerLineSymbolObj( void )
{
	int nSize = (int)_pVctLineSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSLineSymbolBaseObj* pBase = GetLineSymbol(i);
		delete pBase;
		pBase = 0;
	}
	_pVctLineSymbol.clear();
}


int MSMultiLayerLineSymbolObj::AppendLineSymbol( MSLineSymbolBaseObj* pSymbol )
{
	if(pSymbol==0) return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type < MS_SIMPLE_LINE_SYMBOL || type > MS_PICTURE_LINE_SYMBOL))
	{
		assert(0);
		return -1;
	}

	_pVctLineSymbol.push_back(pSymbol);

	return 0;
}


int MSMultiLayerLineSymbolObj::ReplaceLineSymbol( MSLineSymbolBaseObj* newpSymbol, int iIndex )
{
	if(newpSymbol==0)return -1;

	MSSymbolType type = newpSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_LINE_SYMBOL) && 
		(type!=MS_HASH_LINE_SYMBOL) && 
		(type!=MS_MARKER_LINE_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	_pVctLineSymbol.at(iIndex) = (newpSymbol);

	return 0;
}


int MSMultiLayerLineSymbolObj::InsertBefore( MSLineSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_LINE_SYMBOL) && 
		(type!=MS_HASH_LINE_SYMBOL) && 
		(type!=MS_MARKER_LINE_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = _pVctLineSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	_pVctLineSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerLineSymbolObj::InsertAfter( MSLineSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_LINE_SYMBOL) && 
		(type!=MS_HASH_LINE_SYMBOL) && 
		(type!=MS_MARKER_LINE_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || iIndex>nSize)return -1;

	ITT itt = _pVctLineSymbol.begin();
	for(int i=0; i<=iIndex; i++)itt++;

	_pVctLineSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerLineSymbolObj::RemoveSymbol( MSLineSymbolBaseObj* pSymbol )
{
	int nSize = (int)_pVctLineSymbol.size();
	ITT itt = _pVctLineSymbol.begin();
	for(int i=0; i<nSize; i++, itt++)
	{
		MSLineSymbolBaseObj* pBase = GetLineSymbol(i);
		if(pBase==pSymbol)
		{
			//shenzf 释放内存
			delete pSymbol;
			_pVctLineSymbol.erase(itt);
			return 0;
		}
	}
	return -1;
}


int MSMultiLayerLineSymbolObj::RemoveSymbol( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = _pVctLineSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	//shenzf 释放内存
	delete _pVctLineSymbol.at(iIndex);

	_pVctLineSymbol.erase(itt);

	return 0;
}


int MSMultiLayerLineSymbolObj::RemoveAllSymbols()
{
	int nSize = (int)_pVctLineSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSLineSymbolBaseObj* pBase = GetLineSymbol(i);
		delete pBase;
		pBase = 0;
	}
	_pVctLineSymbol.clear();

	return 0;
}


int MSMultiLayerLineSymbolObj::SwapLineSymbol( int iSrcIndex, int iDstIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(nSize<2 || iSrcIndex<0 || iDstIndex<0 || iSrcIndex>nSize-1 || iDstIndex>nSize-1)return -1;

	MSLineSymbolBaseObj* pA = _pVctLineSymbol.at(iSrcIndex);
	MSLineSymbolBaseObj* pB = _pVctLineSymbol.at(iDstIndex);

	_pVctLineSymbol.at(iDstIndex) = pA;
	_pVctLineSymbol.at(iSrcIndex) = pB;

	return 0;
}


int MSMultiLayerLineSymbolObj::MoveUp( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex>nSize-1 || iIndex<1)return -1;

	return SwapLineSymbol(iIndex, iIndex-1);
}


int MSMultiLayerLineSymbolObj::MoveDown( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex>nSize-2 || iIndex<0)return -1;

	return SwapLineSymbol(iIndex, iIndex+1);
}


int MSMultiLayerLineSymbolObj::MoveToTop( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i>0; i--)
	{
		SwapLineSymbol(i, i-1);
	}

	return 0;
}


int MSMultiLayerLineSymbolObj::MoveToBottom( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i<nSize-1; i++)
	{
		SwapLineSymbol(i, i+1);
	}

	return 0;
}


int MSMultiLayerLineSymbolObj::ReverseAllSymbols()
{
	ITT beg = _pVctLineSymbol.begin();
	ITT eed = _pVctLineSymbol.end();

	reverse(beg, eed);

	return 0;
}

int MSMultiLayerLineSymbolObj::GetLineSymbolsNum()
{
	return (int)_pVctLineSymbol.size();
}


MSLineSymbolBaseObj* MSMultiLayerLineSymbolObj::GetLineSymbol( int iIndex )
{
	int nSize = (int)_pVctLineSymbol.size();
	if(iIndex>nSize-1 || iIndex<0)return 0;

	return (MSLineSymbolBaseObj*)(_pVctLineSymbol.at(iIndex));
}




//************************************************************************
//
// class MSSimpleFillSymbolObj
//
//************************************************************************
MSFillSymbolBaseObj::MSFillSymbolBaseObj( void )
{
	SetSymbolType(MS_NULL_SYMBOL);

	//默认背景色为透明色
	m_colorFill._colorR = 0; m_colorFill._colorG = 0;
	m_colorFill._colorB = 0; m_colorFill._colorA = 0;

// 	_backColor._colorR = 255; _backColor._colorG = 255;
// 	_backColor._colorB = 255; _backColor._colorA = 0;

	m_colorHighlight._colorR = 255; m_colorHighlight._colorG = 0;
	m_colorHighlight._colorB = 0; m_colorHighlight._colorA = 0;

	m_pOutLineSymbolObj = 0;
}
MSFillSymbolBaseObj::MSFillSymbolBaseObj(const MSFillSymbolBaseObj& srcObj)
{
	SetSymbolType(MS_NULL_SYMBOL);

	m_pOutLineSymbolObj = srcObj.m_pOutLineSymbolObj;
	m_colorFill = srcObj.m_colorFill;
// 	_backColor = srcObj._backColor;
	m_colorHighlight = srcObj.m_colorHighlight;
}

MSFillSymbolBaseObj::~MSFillSymbolBaseObj( void )
{
	if(m_pOutLineSymbolObj)
	{
		delete m_pOutLineSymbolObj;
		m_pOutLineSymbolObj = NULL;
	}
}


void MSFillSymbolBaseObj::SetFillColor( MSColor dstColor )
{
	m_colorFill = dstColor;
}


// void MSFillSymbolBaseObj::SetBackColor( MSColor dstColor )
// {
// 	_backColor = dstColor;
// }


void MSFillSymbolBaseObj::SetHighLightColor( MSColor dstColor )
{
	MSSymbolObj* pSymbolObj = GetOutlineSymbol();
	if (!pSymbolObj)
	{
		//这里需要增加没有OutLine时的情况
		MSSimpleLineSymbolObj* pLineStringObj_SelectHighLight = new MSSimpleLineSymbolObj();
		SetOutlineSymbol(pLineStringObj_SelectHighLight);
		MSColor color;
		color._colorR = 0; color._colorG = 0; color._colorB = 0; color._colorA = 0;
		pLineStringObj_SelectHighLight->SetColor(color);
		pLineStringObj_SelectHighLight->SetHighLightColor(dstColor);
		return;
	}
	if(pSymbolObj->GetSymbolType() < MS_SIMPLE_LINE_SYMBOL ||
		pSymbolObj->GetSymbolType() > MS_MULTILAYER_LINE_SYMBOL)
	{
		assert(false);
		return;
	}
	MSLineSymbolBaseObj * pLineSymbolBaseObj = (MSLineSymbolBaseObj*)pSymbolObj;
	if (!pLineSymbolBaseObj)
	{
		assert(false);
		return;
	}
	pLineSymbolBaseObj->SetHighLightColor(dstColor);
	m_colorHighlight = dstColor;
}


MSColor MSFillSymbolBaseObj::GetFillColor()
{
	return m_colorFill;
}


// MSColor MSFillSymbolBaseObj::GetBackColor()
// {
// 	return _backColor;
// }


MSColor MSFillSymbolBaseObj::GetHighLightColor()
{
	return m_colorHighlight;
}


void MSFillSymbolBaseObj::SetOutlineSymbol(MSSymbolObj* pOutlineSymbol)
{
	MSSymbolType type = pOutlineSymbol->GetSymbolType();

	bool bInvalidType = (type==MS_SIMPLE_LINE_SYMBOL) || 
		(type==MS_HASH_LINE_SYMBOL) || 
		(type==MS_MARKER_LINE_SYMBOL) || 
		(type == MS_PICTURE_LINE_SYMBOL) ||
		(type==MS_MULTILAYER_LINE_SYMBOL);

#ifdef GDOSMAP_EXCEPTION
	if(bInvalidType==false)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSSymbolType");
	}
#endif

	assert(bInvalidType==true);

	if (m_pOutLineSymbolObj)
	{
		delete m_pOutLineSymbolObj;
		m_pOutLineSymbolObj = NULL;
	}

	m_pOutLineSymbolObj = pOutlineSymbol;
}


MSSymbolObj* MSFillSymbolBaseObj::GetOutlineSymbol()
{
	return m_pOutLineSymbolObj;
}




//************************************************************************
//
// class MSSimpleFillSymbolObj
//
//************************************************************************
MSSimpleFillSymbolObj::MSSimpleFillSymbolObj( void )
{
	SetSymbolType(MS_SIMPLE_FILL_SYMBOL);
}
MSSimpleFillSymbolObj::MSSimpleFillSymbolObj(const MSSimpleFillSymbolObj& srcObj)
{
	SetSymbolType(MS_SIMPLE_FILL_SYMBOL);

	MSSimpleFillSymbolObj* pSourceSimpleFillSymbolObj = (MSSimpleFillSymbolObj*)&srcObj;
	SetFillColor(pSourceSimpleFillSymbolObj->GetFillColor());
	SetHighLightColor(pSourceSimpleFillSymbolObj->GetHighLightColor());

	MSSimpleLineSymbolObj* pSourceOutLineSymbol = (MSSimpleLineSymbolObj*)pSourceSimpleFillSymbolObj->GetOutlineSymbol();
	if (pSourceOutLineSymbol)
	{
		MSSimpleLineSymbolObj* pOutLineSymbol = new MSSimpleLineSymbolObj;
		pOutLineSymbol->SetWidth(pSourceOutLineSymbol->GetWidth());
		pOutLineSymbol->SetColor(pSourceOutLineSymbol->GetColor());

		pOutLineSymbol->SetHighLightColor(pSourceOutLineSymbol->GetHighLightColor());
		MSLineCap startCap; MSLineCap endCap; MSDashCap dashCap;
		pSourceOutLineSymbol->GetLineCap(startCap, endCap, dashCap);
		pOutLineSymbol->SetLineCap(startCap, endCap, dashCap);
		pOutLineSymbol->SetLineJoin(pSourceOutLineSymbol->GetLineJoin());
		pOutLineSymbol->SetOffset(pSourceOutLineSymbol->GetOffset());
		SetOutlineSymbol(pOutLineSymbol);

	}
}


MSSimpleFillSymbolObj::~MSSimpleFillSymbolObj( void )
{

}




//************************************************************************
//
// class MSGraduateFillSymbolObj
//
//************************************************************************
MSGraduateFillSymbolObj::MSGraduateFillSymbolObj( void )
{
	SetSymbolType(MS_GRADUATE_FILL_SYMBOL);

	m_pointCenter.x = m_pointCenter.y = -1;
}
MSGraduateFillSymbolObj::MSGraduateFillSymbolObj(const MSGraduateFillSymbolObj& srcObj)
{
	SetSymbolType(MS_GRADUATE_FILL_SYMBOL);

	for (int i=0;i<srcObj.m_vPoint.size();i++)
	{
		m_vPoint.push_back(srcObj.m_vPoint.at(i));
	}
	for (int i=0;i<srcObj.m_vColor.size();i++)
	{
		m_vColor.push_back(srcObj.m_vColor.at(i));
	}
	m_pointCenter = srcObj.m_pointCenter;
	m_colorCenter = srcObj.m_colorCenter;
}

MSGraduateFillSymbolObj::~MSGraduateFillSymbolObj( void )
{

}
void			MSGraduateFillSymbolObj::AddPOINTAndColor(MSPoint pt,MSColor color)
{
	m_vPoint.push_back(pt);
	m_vColor.push_back(color);
}
bool			MSGraduateFillSymbolObj::DeletePOINTAndColor(MSPoint pt)
{
	for (int i=0;i<m_vPoint.size();i++)
	{
		if (m_vPoint.at(i) == pt)
		{
			m_vPoint.erase(m_vPoint.begin()+i);
			m_vColor.erase(m_vColor.begin()+i);
			return true;
		}
	}
	return false;
}
void			MSGraduateFillSymbolObj::SetCenterPOINTAndColor(MSPoint pt,MSColor color)
{
	m_colorCenter = color;
	m_pointCenter = pt;
}
int				MSGraduateFillSymbolObj::GetPOINTNum()
{
	return (int)m_vPoint.size();
}
bool			MSGraduateFillSymbolObj::GetPOINTAndColor(int index,MSPoint& pt,MSColor& color)
{
	if(index < 0 || index >= m_vPoint.size())return false;
	pt = m_vPoint.at(index);
	color = m_vColor.at(index);
	return true;
}
bool			MSGraduateFillSymbolObj::GetCenterPOINTAndColor(MSPoint& pt,MSColor& color)
{
	color = m_colorCenter;
	pt = m_pointCenter;
	return true;
}
bool			MSGraduateFillSymbolObj::DeleteCenterPOINTAndColor()
{
	m_pointCenter.x = m_pointCenter.y = -1;
	return true;
}



//************************************************************************
//
// class MSPictureFillSymbolObj
//
//************************************************************************
MSPictureFillSymbolObj::MSPictureFillSymbolObj( void )
{
	SetSymbolType(MS_PICTURE_FILL_SYMBOL);

	m_dAngle = 0;
	m_dOffsetX = m_dOffsetY = 0;

	m_pPicture._imageBuf[0] = '\0';
	m_dSeparationX = m_dSeparationY = 0;

	m_eWrapMode = MS_WrapModeTile;
}
MSPictureFillSymbolObj::MSPictureFillSymbolObj(const MSPictureFillSymbolObj& srcObj)
{
	SetSymbolType(MS_PICTURE_FILL_SYMBOL);

	m_dAngle = srcObj.m_dAngle;
	m_dOffsetX = srcObj.m_dOffsetX;
	m_dOffsetY = srcObj.m_dOffsetY;
	m_dSeparationX = srcObj.m_dSeparationX;
	m_dSeparationY = srcObj.m_dSeparationY;
	m_pPicture = srcObj.m_pPicture;
	m_eWrapMode = srcObj.m_eWrapMode;
}

MSPictureFillSymbolObj::~MSPictureFillSymbolObj( void )
{

}


void MSPictureFillSymbolObj::SetAngle( double angel )
{
	m_dAngle = angel;
}


void MSPictureFillSymbolObj::SetOffsetX( double offsetX )
{
	m_dOffsetX = offsetX;
}


void MSPictureFillSymbolObj::SetOffsetY( double offsetY )
{
	m_dOffsetY = offsetY;
}


void MSPictureFillSymbolObj::SetPictureFilename( const char* buffer )
{
	if(buffer == 0)
		return;

	strcpy(m_pPicture._imageBuf, buffer);
}

void MSPictureFillSymbolObj::SetWrapMode(MSWrapMode wrapMode)
{
	m_eWrapMode = wrapMode;
}

double MSPictureFillSymbolObj::GetAngle()
{
	return m_dAngle;
}


double MSPictureFillSymbolObj::GetOffsetX()
{
	return m_dOffsetX;
}


double MSPictureFillSymbolObj::GetOffsetY()
{
	return m_dOffsetY;
}


char* MSPictureFillSymbolObj::GetPictureFilename()
{
	return m_pPicture._imageBuf;
}


MSWrapMode MSPictureFillSymbolObj::GetWrapMode()
{
	return m_eWrapMode;
}

void MSPictureFillSymbolObj::SetScaleX(double scaleX)
{
	m_pPicture._scaleX = scaleX;
}
void MSPictureFillSymbolObj::SetScaleY(double scaleY)
{
	m_pPicture._scaleY = scaleY;
}
double MSPictureFillSymbolObj::GetScaleX()
{
	return m_pPicture._scaleX;
}
double MSPictureFillSymbolObj::GetScaleY()
{
	return m_pPicture._scaleY;
}
void MSPictureFillSymbolObj::SetSeparationX(double separationX)
{
	m_dSeparationX  = separationX;
}
void MSPictureFillSymbolObj::SetSeparationY(double separationY)
{
	m_dSeparationY = separationY;
}
double MSPictureFillSymbolObj::GetSeparationX()
{
	return m_dSeparationX;
}
double MSPictureFillSymbolObj::GetSeparationY()
{
	return m_dSeparationY;
}

//************************************************************************
//
// class MSMarkerFillSymbolObj
//
//************************************************************************
MSMarkerFillSymbolObj::MSMarkerFillSymbolObj( void )
{
	SetSymbolType(MS_MARKER_FILL_SYMBOL);

	m_fFillIntervalX = 0;
	m_fFillIntervalY = 0;
	m_pMarkSymbol = NULL;
	m_bUseRandomMarker = false;

}
MSMarkerFillSymbolObj::MSMarkerFillSymbolObj( const MSMarkerFillSymbolObj& srcObj )
{
	SetSymbolType(MS_MARKER_FILL_SYMBOL);

	m_bUseRandomMarker = srcObj.m_bUseRandomMarker;
	m_fFillIntervalX = srcObj.m_fFillIntervalX;
	m_fFillIntervalY = srcObj.m_fFillIntervalY;
	m_pMarkSymbol = srcObj.m_pMarkSymbol;
}

MSMarkerFillSymbolObj::~MSMarkerFillSymbolObj( void )
{
	if(m_pMarkSymbol!=0)
	{
		delete (MSSymbolObj*)m_pMarkSymbol;
		m_pMarkSymbol = 0;
	}
}


void MSMarkerFillSymbolObj::SetFillIntVX(float intvalX)
{
#ifdef GDOSMAP_EXCEPTION
	if(intvalx<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "intvalx<0");
	}
#endif
	m_fFillIntervalX = intvalX;
}


void MSMarkerFillSymbolObj::SetFillIntVY(float intvalY)
{
#ifdef GDOSMAP_EXCEPTION
	if(intvalY<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "intvalY<0");
	}
#endif
	m_fFillIntervalY = intvalY;
}


float	MSMarkerFillSymbolObj::GetFillIntVX()
{
	return m_fFillIntervalX;
}


float	MSMarkerFillSymbolObj::GetFillIntVY()
{
	return m_fFillIntervalY;
}


MSSymbolObj* MSMarkerFillSymbolObj::GetMarkerSymbol()
{
	return (MSSymbolObj*)m_pMarkSymbol;
}


void MSMarkerFillSymbolObj::SetMarkerSymbol(MSSymbolObj* pMarker)
{
	MSSymbolType type = pMarker->GetSymbolType();

	bool bInvalidType = (type==MS_CHARACTER_MARKER_SYMBOL) || 
		(type==MS_SIMPLE_MARKER_SYMBOL) ||
		(type==MS_PICTURE_MARKER_SYMBOL) ||
		(type==MS_MULTILAYER_MARKER_SYMBOL);

#ifdef GDOSMAP_EXCEPTION
	if(bInvalidType==false)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSSymbolType");
	}
#endif

	assert(bInvalidType==true);

	m_pMarkSymbol = pMarker;
}
void			 MSMarkerFillSymbolObj::SetUseRandomMarker()
{
	m_bUseRandomMarker = true;
}
bool			 MSMarkerFillSymbolObj::GetUseRandomMarker()
{
	return m_bUseRandomMarker;
}


void MSMarkerFillSymbolObj::RemoveMarkerSymbol()
{
	if(m_pMarkSymbol!=0)
	{
		delete (MSSymbolObj*)m_pMarkSymbol;
		m_pMarkSymbol = 0;
	}
}



//************************************************************************
//
// class MSHatchFillSymbolObj
//
//************************************************************************
MSHatchFillSymbolObj::MSHatchFillSymbolObj(void)
{
	SetSymbolType(MS_HATCH_FILL_SYMBOL);

	m_eFillStyle = MS_FillStyle_HatchStyleHorizontal;
	m_colorBackHatch._colorA = 0;
}
MSHatchFillSymbolObj::MSHatchFillSymbolObj(const MSHatchFillSymbolObj& srcObj)
{
	SetSymbolType(MS_HATCH_FILL_SYMBOL);

	m_colorForeHatch = srcObj.m_colorForeHatch;
	m_colorBackHatch = srcObj.m_colorBackHatch;
	m_eFillStyle = srcObj.m_eFillStyle;
}
MSHatchFillSymbolObj::~MSHatchFillSymbolObj(void)
{

}
void			MSHatchFillSymbolObj::SetFillStyle(MSFillStyle style)
{
	m_eFillStyle = style;
}
void			MSHatchFillSymbolObj::SetForeColor(MSColor color)
{
	m_colorForeHatch = color;
}
void			MSHatchFillSymbolObj::SetBackColor(MSColor color)
{
	m_colorBackHatch = color;
}

MSFillStyle		MSHatchFillSymbolObj::GetFillStyle()
{
	return m_eFillStyle;
}
MSColor			MSHatchFillSymbolObj::GetForeColor()
{
	return m_colorForeHatch;
}
MSColor			MSHatchFillSymbolObj::GetBackColor()
{
	return m_colorBackHatch;
}



//************************************************************************
//
// class MSLineFillSymbolObj
//
//************************************************************************
MSLineFillSymbolObj::MSLineFillSymbolObj( void )
{
	SetSymbolType(MS_LINE_FILL_SYMBOL);

	m_dAngle = 0;
	m_dInterval = 10;
	m_pFillLineSymbol = NULL;
}
MSLineFillSymbolObj::MSLineFillSymbolObj( const MSLineFillSymbolObj& srcObj )
{
	SetSymbolType(MS_LINE_FILL_SYMBOL);

	m_dAngle = srcObj.m_dAngle;
	m_dInterval = srcObj.m_dInterval;
	m_pFillLineSymbol = srcObj.m_pFillLineSymbol;
}

MSLineFillSymbolObj::~MSLineFillSymbolObj( void )
{
	if(m_pFillLineSymbol!=0)
	{
		delete (MSSymbolObj*)m_pFillLineSymbol;
		m_pFillLineSymbol = 0;
	}
}


void MSLineFillSymbolObj::SetAngle( double angel )
{
	m_dAngle = angel;
}


void MSLineFillSymbolObj::SetInteval( double itvl )
{
#ifdef GDOSMAP_EXCEPTION
	if(itvl<0)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "itvl<0");
	}
#endif
	m_dInterval = itvl;
}


void MSLineFillSymbolObj::SetFilllineSymbol( MSSymbolObj* pSymbol )
{
	MSSymbolType type = pSymbol->GetSymbolType();

	bool bInvalidType = (type==MS_SIMPLE_LINE_SYMBOL) || 
		(type==MS_HASH_LINE_SYMBOL) || 
		(type==MS_MARKER_LINE_SYMBOL) || 
		(type==MS_MULTILAYER_LINE_SYMBOL);

#ifdef GDOSMAP_EXCEPTION
	if(bInvalidType==false)
	{
		MSMAP_THROW(MSMInvalidValueExcption, "MSSymbolType");
	}
#endif

	assert(bInvalidType==true);

	m_pFillLineSymbol = pSymbol;
}


double MSLineFillSymbolObj::GetAngle()
{
	return m_dAngle;
}


double MSLineFillSymbolObj::GetInterval()
{
	return m_dInterval;
}


MSSymbolObj* MSLineFillSymbolObj::GetFilllineSymbol()
{
	return (MSSymbolObj*)(m_pFillLineSymbol);
}


void MSLineFillSymbolObj::RemoveFilllineSymbol()
{
	if(m_pFillLineSymbol!=0)
	{
		delete (MSSymbolObj*)m_pFillLineSymbol;
		m_pFillLineSymbol = 0;
	}
}
// void MSLineFillSymbolObj::SetFillStyle(MSFillStyle style)
// {
// 	m_FillStyle = style;
// }
// MSFillStyle MSLineFillSymbolObj::GetFillStyle()
// {
// 	return m_FillStyle;
// }

//************************************************************************
//
// class MSMultiLayerFillSymbolObj
//
//************************************************************************
MSMultiLayerFillSymbolObj::MSMultiLayerFillSymbolObj( void )
{
	SetSymbolType(MS_MULTILAYER_FILL_SYMBOL);

	_pVctFillSymbol.clear();

}
MSMultiLayerFillSymbolObj::MSMultiLayerFillSymbolObj( const MSMultiLayerFillSymbolObj& srcObj )
{
	SetSymbolType(MS_MULTILAYER_FILL_SYMBOL);

	MSMultiLayerFillSymbolObj* pMLFS = &((MSMultiLayerFillSymbolObj)srcObj);
	for (int i=0;i<pMLFS->GetFillSymbolsNum();i++)
	{
		MSFillSymbolBaseObj* pFillSymbolBaseObj = pMLFS->GetFillSymbol(i);
		MSSymbolType type = pFillSymbolBaseObj->GetSymbolType();
		MSFillSymbolBaseObj* pNewFillSymbolBaseObj;
		if (type == MS_SIMPLE_FILL_SYMBOL)
		{
			MSSimpleFillSymbolObj* pSimpleFillSymbolObj = (MSSimpleFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSSimpleFillSymbolObj(*pSimpleFillSymbolObj);
		}
		else if (type == MS_GRADUATE_FILL_SYMBOL)
		{
			MSGraduateFillSymbolObj* pGraduateFillSymbolObj = (MSGraduateFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSGraduateFillSymbolObj(*pGraduateFillSymbolObj);
		}
		else if (type == MS_MARKER_FILL_SYMBOL)
		{
			MSMarkerFillSymbolObj* pMarkerFillSymbolObj = (MSMarkerFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSMarkerFillSymbolObj(*pMarkerFillSymbolObj);
		}
		else if (type == MS_HATCH_FILL_SYMBOL)
		{
			MSHatchFillSymbolObj* pHatchFillSymbolObj = (MSHatchFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSHatchFillSymbolObj(*pHatchFillSymbolObj);
		}
		else if (type == MS_LINE_FILL_SYMBOL)
		{
			MSLineFillSymbolObj* pLineFillSymbolObj = (MSLineFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSLineFillSymbolObj(*pLineFillSymbolObj);
		}
		else if (type == MS_PICTURE_FILL_SYMBOL)
		{
			MSPictureFillSymbolObj* pPictureFillSymbolObj = (MSPictureFillSymbolObj*)pFillSymbolBaseObj;
			pNewFillSymbolBaseObj = new MSPictureFillSymbolObj(*pPictureFillSymbolObj);
		}
		AppendFillSymbol(pNewFillSymbolBaseObj);
	}
}

MSMultiLayerFillSymbolObj::~MSMultiLayerFillSymbolObj( void )
{
	int nSize = (int)_pVctFillSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSFillSymbolBaseObj* pBase = GetFillSymbol(i);
		delete pBase;
		pBase = 0;
	}
	_pVctFillSymbol.clear();
}


int MSMultiLayerFillSymbolObj::AppendFillSymbol( MSFillSymbolBaseObj* pSymbol )
{
	if(pSymbol==0) return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type < MS_SIMPLE_FILL_SYMBOL || type > MS_PICTURE_FILL_SYMBOL))
	{
		assert(0);
		return -1;
	}

	_pVctFillSymbol.push_back(pSymbol);

	return 0;
}


int MSMultiLayerFillSymbolObj::ReplaceFillSymbol( MSFillSymbolBaseObj* newpSymbol, int iIndex )
{
	if(newpSymbol==0)return -1;

	MSSymbolType type = newpSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_FILL_SYMBOL) && 
		(type!=MS_MARKER_FILL_SYMBOL) && 
		(type!=MS_LINE_FILL_SYMBOL) &&
		(type!=MS_PICTURE_FILL_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	//shenzf 释放内存
	delete _pVctFillSymbol.at(iIndex);

	_pVctFillSymbol.at(iIndex) = (newpSymbol);

	return 0;
}


int MSMultiLayerFillSymbolObj::InsertBefore( MSFillSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_FILL_SYMBOL) && 
		(type!=MS_MARKER_FILL_SYMBOL) && 
		(type!=MS_LINE_FILL_SYMBOL) &&
		(type!=MS_PICTURE_FILL_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = _pVctFillSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	_pVctFillSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerFillSymbolObj::InsertAfter( MSFillSymbolBaseObj* pSymbol,int iIndex )
{
	if(pSymbol==0)return -1;

	MSSymbolType type = pSymbol->GetSymbolType();
	if ((type!=MS_SIMPLE_FILL_SYMBOL) && 
		(type!=MS_MARKER_FILL_SYMBOL) && 
		(type!=MS_LINE_FILL_SYMBOL) &&
		(type!=MS_PICTURE_FILL_SYMBOL))
	{
		assert(0);
		return -1;
	}

	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || iIndex>nSize)return -1;

	ITT itt = _pVctFillSymbol.begin();
	for(int i=0; i<=iIndex; i++)itt++;

	_pVctFillSymbol.insert(itt, pSymbol);

	return 0;
}


int MSMultiLayerFillSymbolObj::RemoveSymbol( MSFillSymbolBaseObj* pSymbol )
{
	int nSize = (int)_pVctFillSymbol.size();
	ITT itt = _pVctFillSymbol.begin();
	for(int i=0; i<nSize; i++, itt++)
	{
		MSFillSymbolBaseObj* pBase = GetFillSymbol(i);
		if(pBase==pSymbol)
		{
			//shenzf 释放内存
			delete pBase;

			_pVctFillSymbol.erase(itt);
			return 0;
		}
	}
	return -1;
}


int MSMultiLayerFillSymbolObj::RemoveSymbol( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || iIndex>nSize-1)return -1;

	ITT itt = _pVctFillSymbol.begin();
	for(int i=0; i<iIndex; i++)itt++;

	//shenzf 释放内存
	delete _pVctFillSymbol.at(iIndex);

	_pVctFillSymbol.erase(itt);

	return 0;
}


int MSMultiLayerFillSymbolObj::RemoveAllSymbols()
{
	int nSize = (int)_pVctFillSymbol.size();
	for(int i=0; i<nSize; i++)
	{
		MSFillSymbolBaseObj* pBase = GetFillSymbol(i);
		delete pBase;
		pBase = 0;
	}
	_pVctFillSymbol.clear();

	return 0;
}


int MSMultiLayerFillSymbolObj::SwapFillSymbol( int iSrcIndex, int iDstIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(nSize<2 || iSrcIndex<0 || iDstIndex<0 || iSrcIndex>nSize-1 || iDstIndex>nSize-1)return -1;

	MSFillSymbolBaseObj* pA = _pVctFillSymbol.at(iSrcIndex);
	MSFillSymbolBaseObj* pB = _pVctFillSymbol.at(iDstIndex);

	_pVctFillSymbol.at(iDstIndex) = pA;
	_pVctFillSymbol.at(iSrcIndex) = pB;

	return 0;
}


int MSMultiLayerFillSymbolObj::MoveUp( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex>nSize-1 || iIndex<1)return -1;

	return SwapFillSymbol(iIndex, iIndex-1);
}


int MSMultiLayerFillSymbolObj::MoveDown( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex>nSize-2 || iIndex<0)return -1;

	return SwapFillSymbol(iIndex, iIndex+1);
}


int MSMultiLayerFillSymbolObj::MoveToTop( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i>0; i--)
	{
		SwapFillSymbol(i, i-1);
	}

	return 0;
}


int MSMultiLayerFillSymbolObj::MoveToBottom( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex<0 || nSize<iIndex+1)return -1;

	for(int i=iIndex; i<nSize-1; i++)
	{
		SwapFillSymbol(i, i+1);
	}

	return 0;
}


int MSMultiLayerFillSymbolObj::ReverseAllSymbols()
{
	ITT beg = _pVctFillSymbol.begin();
	ITT eed = _pVctFillSymbol.end();

	reverse(beg, eed);

	return 0;
}

int MSMultiLayerFillSymbolObj::GetFillSymbolsNum()
{
	return (int)_pVctFillSymbol.size();
}


MSFillSymbolBaseObj* MSMultiLayerFillSymbolObj::GetFillSymbol( int iIndex )
{
	int nSize = (int)_pVctFillSymbol.size();
	if(iIndex>nSize-1 || iIndex<0)return 0;

	return (MSFillSymbolBaseObj*)(_pVctFillSymbol.at(iIndex));
}



//************************************************************************
//
// class MSRangeSymbolObj
//
//************************************************************************
MSRangeSymbolObj::MSRangeSymbolObj( void )
{
	SetSymbolType(MS_RANGE_SYMBOL);

	m_dMinValue = 0;
	m_dMaxValue = 0;
	m_cLabel[0] = '\0';
	m_pSymbol = NULL;
}
MSRangeSymbolObj::MSRangeSymbolObj(const MSRangeSymbolObj& srcObj)
{
	SetSymbolType(MS_RANGE_SYMBOL);

	m_dMinValue = srcObj.m_dMinValue;
	m_dMaxValue = srcObj.m_dMaxValue;
	strcpy(m_cLabel,srcObj.m_cLabel);

	MSSymbolType type = srcObj.m_pSymbol->GetSymbolType();
	if (type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pSimpleMarkerSymbolObj = new MSSimpleMarkerSymbolObj(*(MSSimpleMarkerSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleMarkerSymbolObj;
	}
	else if (type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pSimpleLineSymbolObj = new MSSimpleLineSymbolObj(*(MSSimpleLineSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleLineSymbolObj;
	}
	else if (type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSimpleFillSymbolObj = new MSSimpleFillSymbolObj(*(MSSimpleFillSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleFillSymbolObj;
	}
	else
		ASSERT_FALSE_AND_RETURN;
}
MSRangeSymbolObj::~MSRangeSymbolObj( void )
{
	if(m_pSymbol!=0)
	{
		delete (MSSymbolObj*)m_pSymbol;
		m_pSymbol = 0;
	}
}


void MSRangeSymbolObj::SetMinValue( double val )
{
	m_dMinValue = val;
}


void MSRangeSymbolObj::SetMaxValue( double val )
{
	m_dMaxValue = val;
}


void MSRangeSymbolObj::SetLabel( const char* strLabel )
{
	if (strLabel == 0)
		return;

	strcpy(m_cLabel, strLabel);
}


double MSRangeSymbolObj::GetMinValue()
{
	return m_dMinValue;
}


double MSRangeSymbolObj::GetMaxValue()
{
	return m_dMaxValue;
}


char* MSRangeSymbolObj::GetLabel()
{
	return m_cLabel;
}


void MSRangeSymbolObj::RemoveSymbol()
{
	if(m_pSymbol!=0)
	{
		delete (MSSymbolObj*)m_pSymbol;
		m_pSymbol = 0;
	}
}


void MSRangeSymbolObj::SetSymbol(MSSymbolObj *p)
{
	if (m_pSymbol != 0) 
	{
		delete m_pSymbol;
		m_pSymbol = 0;
	}
	m_pSymbol = p;
}


MSSymbolObj* MSRangeSymbolObj::GetSymbol()
{
	return (MSSymbolObj*)m_pSymbol;
}




//************************************************************************
//
// class MSValueSymbolObj
//
//************************************************************************
MSValueSymbolObj::MSValueSymbolObj( void )
{
	SetSymbolType(MS_VALUE_SYMBOL);

	m_cValue[0] = '\0';
	m_cLabel[0] = '\0';
	m_pSymbol = NULL;
}
MSValueSymbolObj::MSValueSymbolObj(const MSValueSymbolObj& srcObj)
{
	SetSymbolType(MS_VALUE_SYMBOL);

	strcpy(m_cValue,srcObj.m_cValue);
	strcpy(m_cLabel,srcObj.m_cLabel);

	MSSymbolType type = srcObj.m_pSymbol->GetSymbolType();
	if (type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pSimpleMarkerSymbolObj = new MSSimpleMarkerSymbolObj(*(MSSimpleMarkerSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleMarkerSymbolObj;
	}
	else if (type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pSimpleLineSymbolObj = new MSSimpleLineSymbolObj(*(MSSimpleLineSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleLineSymbolObj;
	}
	else if (type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSimpleFillSymbolObj = new MSSimpleFillSymbolObj(*(MSSimpleFillSymbolObj*)srcObj.m_pSymbol);
		m_pSymbol = pSimpleFillSymbolObj;
	}
	else
		ASSERT_FALSE_AND_RETURN;
}

MSValueSymbolObj::~MSValueSymbolObj( void )
{
	if(m_pSymbol!=0)
	{
		delete (MSSymbolObj*)m_pSymbol;
		m_pSymbol = 0;
	}
}


void MSValueSymbolObj::SetValue( const char* strVal )
{
	if (strVal == NULL)
		return;
	strcpy(m_cValue, strVal);
}


void MSValueSymbolObj::SetLabel( const char* strLabel )
{
	if (strLabel == NULL)
		return;

	strcpy(m_cLabel, strLabel);
}


char* MSValueSymbolObj::GetValue()
{
	return m_cValue;
}


char* MSValueSymbolObj::GetLabel()
{
	return m_cLabel;
}


void MSValueSymbolObj::RemoveSymbol()
{
	if(m_pSymbol!=0)
	{
		delete (MSSymbolObj*)m_pSymbol;
		m_pSymbol = 0;
	}
}


void MSValueSymbolObj::SetSymbol(MSSymbolObj *p)
{
	if (m_pSymbol != 0) 
	{
		delete m_pSymbol;
		m_pSymbol = 0;
	}
	m_pSymbol = p;
}


MSSymbolObj* MSValueSymbolObj::GetSymbol()
{
	return (MSSymbolObj*)m_pSymbol;
}
