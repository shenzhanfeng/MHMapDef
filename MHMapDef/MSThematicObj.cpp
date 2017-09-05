#include "stdafx.h"
#include "MSThematicObj.h"
#include "MSMapObj.h"

#include <map>
using namespace std;

MSAttribute::MSAttribute()
{
}
MSAttribute::~MSAttribute( void )
{
}


MSFilterAttribute::MSFilterAttribute()
{
	_strFilter[0] = '\0';
}
MSFilterAttribute::~MSFilterAttribute()
{

}
void	MSFilterAttribute::SetFilter(const char* strFilter)
{
	assert(strFilter);
	if(strFilter==0)return;

	strcpy(_strFilter, strFilter);
}
char*	MSFilterAttribute::GetFilter()
{
	return _strFilter;
}


MSAOIAttribute::MSAOIAttribute()
{

}
MSAOIAttribute::~MSAOIAttribute()
{

}
MSEnvelopObj*	MSAOIAttribute::GetEnvelop()
{
	return &m_AOIObj;
}


MSThematicObj::MSThematicObj()
{
	m_eObjType = MS_OBJ_THEMATIC;

	m_eThematicType = MS_THEMATIC_NULL;
}
MSThematicObj::MSThematicObj(const MSThematicObj& srcObj)
{
	m_eObjType = srcObj.m_eObjType;
	m_eThematicType = srcObj.m_eThematicType;
}
MSThematicObj::~MSThematicObj(void)
{
}
void			MSThematicObj::SetThematicType( MSThematicType type )
{
	m_eThematicType = type;
}
MSThematicType	MSThematicObj::GetThematicType()
{
	return m_eThematicType;
}


MSFeatureThematicObj::MSFeatureThematicObj()
{
	m_bFilter = false;
	m_bUseLabel = false;
}
MSFeatureThematicObj::MSFeatureThematicObj(MSFeatureThematicObj& srcObj)
{
	m_LabelObj = srcObj.m_LabelObj;
	m_bUseLabel = srcObj.m_bUseLabel;
	m_FilterAttribute = srcObj.m_FilterAttribute;
	m_bFilter = srcObj.m_bFilter;
}
MSFeatureThematicObj::~MSFeatureThematicObj()
{
}
MSLabelThematicObj*		MSFeatureThematicObj::GetLabelObj()
{
	return &m_LabelObj;
}
MSFilterAttribute*		MSFeatureThematicObj::GetFilterAttribute()
{
	return &m_FilterAttribute;
}
bool	MSFeatureThematicObj::GetUseLabel()
{
	return m_bUseLabel;
}
void	MSFeatureThematicObj::SetUseLabel( bool bUse )
{
	m_bUseLabel = bUse;
}
void	MSFeatureThematicObj::SetUseFilter(bool bUse)
{
	m_bFilter = bUse;
}
bool	MSFeatureThematicObj::GetUseFilter()
{
	return m_bFilter;
}


MSRasterThematicObj :: MSRasterThematicObj()
{
	m_bAOI = false;
}
MSRasterThematicObj :: MSRasterThematicObj(const MSRasterThematicObj& srcObj)
{
	m_AOIAttribute = srcObj.m_AOIAttribute;
	m_bAOI = srcObj.m_bAOI;
}
MSRasterThematicObj :: ~MSRasterThematicObj()
{

}
MSAOIAttribute*			MSRasterThematicObj::GetAOIObj()
{
	return &m_AOIAttribute;
}
void	MSRasterThematicObj::SetAOIState( bool bb )
{
	m_bAOI = bb;
}
bool	MSRasterThematicObj::GetAOIState()
{
	return m_bAOI;
}


MSAnnotationThematicObj::MSAnnotationThematicObj()
{
	m_eThematicType = MS_THEMATIC_ANNOTATION;
	m_colorAnno = _MSColor(255,0,0,0);
	m_fontAnno._blod = m_fontAnno._italic = m_fontAnno._strikeout = m_fontAnno._underline = false;
	strcpy(m_fontAnno._fontName,"宋体");
	m_dSizeAnno = 10;
	m_dIntervalAnno = 0;
	m_eHorizontalAlignment = HORIZONTAL_LEFT_ALIGMENT;
	m_eVerticalAlignment = VERTICAL_TOP_ALIGMENT;
	m_dOffsetX = 0;
	m_dOffsetY = 0;
	m_eAnnoType = MH_POINT_ANNO;
	m_bAllowOvelapped = m_bAllowRemoveDuplicated = m_bShowWithScale = false;
	strcpy(m_cField,"");
}
MSAnnotationThematicObj::MSAnnotationThematicObj(const MSAnnotationThematicObj& srcObj)
{
	m_eThematicType = srcObj.m_eThematicType;
	m_colorAnno = srcObj.m_colorAnno;
	m_fontAnno = srcObj.m_fontAnno;
	m_dSizeAnno = srcObj.m_dSizeAnno;
	m_dIntervalAnno = srcObj.m_dIntervalAnno;
	m_eHorizontalAlignment = srcObj.m_eHorizontalAlignment;
	m_eVerticalAlignment = srcObj.m_eVerticalAlignment;
	m_dOffsetX = srcObj.m_dOffsetX;
	m_dOffsetY = srcObj.m_dOffsetY;
	m_eAnnoType = srcObj.m_eAnnoType;
	m_bShowWithScale = srcObj.m_bShowWithScale;
	m_bAllowOvelapped = srcObj.m_bAllowOvelapped;
	m_bAllowRemoveDuplicated = m_bAllowRemoveDuplicated;
	strcpy(m_cField,srcObj.m_cField);
}
MSAnnotationThematicObj::~MSAnnotationThematicObj()
{
}
void	MSAnnotationThematicObj::SetColor(MSColor dstColor)
{
	m_colorAnno = dstColor;
}
void	MSAnnotationThematicObj::SetSize( double dSize )
{
	m_dSizeAnno = dSize;
}
void	MSAnnotationThematicObj::SetFontName( const char* strName )
{
	assert(strName!=0);
	if(strName==0)return;

	strcpy(m_fontAnno._fontName, strName);
}
void	MSAnnotationThematicObj::SetFontItalic( bool bb )
{
	m_fontAnno._italic = bb;
}
void	MSAnnotationThematicObj::SetFontBold( bool bb )
{
	m_fontAnno._blod = bb;
}
void	MSAnnotationThematicObj::SetFontUnderline(bool bb)
{
	m_fontAnno._underline = bb;
}
void	MSAnnotationThematicObj::SetInterval( double interval )
{
	m_dIntervalAnno = interval;
}
void	MSAnnotationThematicObj::SetHorizAllig( MSHorizontalAligmentType type )
{
	m_eHorizontalAlignment = type;
}
void	MSAnnotationThematicObj::SetVertiAllig( MSVerticalAligmentType type )
{
	m_eVerticalAlignment = type;
}
void	MSAnnotationThematicObj::SetOffsetX( double offsetX )
{
	m_dOffsetX = offsetX;
}
void	MSAnnotationThematicObj::SetOffsetY( double offsetY )
{
	m_dOffsetY = offsetY;
}
void	MSAnnotationThematicObj::SetAnnoType( MSANNOTYPE type )
{
	m_eAnnoType = type;
}
void	MSAnnotationThematicObj::SetShowWithScale( bool bb )
{
	m_bShowWithScale = bb;
}
void	MSAnnotationThematicObj::SetAllowOverlapped( bool bb )
{
	m_bAllowOvelapped = bb;
}
void	MSAnnotationThematicObj::SetAllowRemoveDuplicated( bool bb )
{
	m_bAllowRemoveDuplicated = bb;
}
void	MSAnnotationThematicObj::SetField(const char* pField)
{
	assert(pField);
	if(pField==0)return;

	strcpy(m_cField, pField);
}
MSColor MSAnnotationThematicObj::GetColor()
{
	return m_colorAnno;
}
double	MSAnnotationThematicObj::GetSize()
{
	return m_dSizeAnno;
}
char*	MSAnnotationThematicObj::GetFontName()
{
	return m_fontAnno._fontName;
}
bool	MSAnnotationThematicObj::GetFontItalic()
{
	return m_fontAnno._italic;
}
bool	MSAnnotationThematicObj::GetFontBold()
{
	return m_fontAnno._blod;
}
bool	MSAnnotationThematicObj::GetFontUnderline()
{
	return m_fontAnno._underline;
}
double	MSAnnotationThematicObj::GetInterval()
{
	return m_dIntervalAnno;
}
MSHorizontalAligmentType MSAnnotationThematicObj::GetHorizAllig()
{
	return m_eHorizontalAlignment;
}
MSVerticalAligmentType MSAnnotationThematicObj::GetVertiAllig()
{
	return m_eVerticalAlignment;
}
double	MSAnnotationThematicObj::GetOffsetX()
{
	return m_dOffsetX;
}
double	MSAnnotationThematicObj::GetOffsetY()
{
	return m_dOffsetY;
}
MSANNOTYPE MSAnnotationThematicObj::GetAnnoType()
{
	return m_eAnnoType;
}
bool	MSAnnotationThematicObj::GetShowWithScale()
{
	return m_bShowWithScale;
}
bool	MSAnnotationThematicObj::GetAllowOvelapped()
{
	return m_bAllowOvelapped;
}
bool	MSAnnotationThematicObj::GetAllowRemoveDuplicated()
{
	return m_bAllowRemoveDuplicated;
}
char*	MSAnnotationThematicObj::GetField()
{
	return m_cField;
}
MSFont* MSAnnotationThematicObj::GetFontPtr()
{
	return &m_fontAnno;
}


MSLabelThematicObj::MSLabelThematicObj()
{
	m_eThematicType = MS_THEMATIC_LABEL;

	m_posLabel.push_back(Right);
	m_posLabel.push_back(RightTop);
	m_posLabel.push_back(RightBottom);
	m_posLabel.push_back(Top);
	m_posLabel.push_back(Bottom);
	m_posLabel.push_back(LeftTop);
	m_posLabel.push_back(Left);
	m_posLabel.push_back(LeftBottom);
	m_nPrecision = -1;

	m_colorLabel = _MSColor(255,0,0,0);
	m_fontLabel._blod = m_fontLabel._italic = m_fontLabel._strikeout = m_fontLabel._underline = false;
	strcpy(m_fontLabel._fontName,"宋体");
	m_dSizeLabel = 10;
	m_dIntervalLabel = 0;
	m_eHorizontalAlignment = HORIZONTAL_LEFT_ALIGMENT;
	m_eVerticalAlignment = VERTICAL_TOP_ALIGMENT;
	m_dOffsetX = 0;
	m_dOffsetY = 0;
	m_eLabelType = MH_POINT_ANNO;
	m_bAllowOvelapped = m_bAllowRemoveDuplicated = m_bShowWithScale = false;
	strcpy(m_cField,"");
	m_pFillPolygonSymbol = NULL;
	m_bUseBigEdge = false;
	//透明
	m_colorEdge = _MSColor(0,0,0,0);
}
MSLabelThematicObj::MSLabelThematicObj(const MSLabelThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_LABEL;

	for (int i=0;i<srcObj.m_posLabel.size();i++)
	{
		m_posLabel.push_back(srcObj.m_posLabel.at(i));
	}
	m_nPrecision = srcObj.m_nPrecision;
	m_colorLabel = srcObj.m_colorLabel;
	m_fontLabel = srcObj.m_fontLabel;
	m_dSizeLabel = srcObj.m_dSizeLabel;
	m_dIntervalLabel = srcObj.m_dIntervalLabel;
	m_eHorizontalAlignment = srcObj.m_eHorizontalAlignment;
	m_eVerticalAlignment = srcObj.m_eVerticalAlignment;
	m_dOffsetX = srcObj.m_dOffsetX;
	m_dOffsetY = srcObj.m_dOffsetY;
	m_eLabelType = srcObj.m_eLabelType;
	m_bShowWithScale = srcObj.m_bShowWithScale;
	m_bAllowOvelapped = srcObj.m_bAllowOvelapped;
	m_bAllowRemoveDuplicated = m_bAllowRemoveDuplicated;
	strcpy(m_cField,srcObj.m_cField);
	m_colorEdge = srcObj.m_colorEdge;
	m_bUseBigEdge = srcObj.m_bUseBigEdge;
	m_pFillPolygonSymbol = srcObj.m_pFillPolygonSymbol;
}
MSLabelThematicObj::~MSLabelThematicObj()
{
	if (m_pFillPolygonSymbol)
	{
		delete m_pFillPolygonSymbol;
		m_pFillPolygonSymbol = NULL;
	}
}
void MSLabelThematicObj::operator = (MSLabelThematicObj& right)
{
	SetColor(right.GetColor());
	SetSize(right.GetSize());
	SetFontName(right.GetFontName());
	SetFontItalic(right.GetFontItalic());
	SetFontBold(right.GetFontBold());
	SetFontUnderline(right.GetFontUnderline());
	SetFontStrikeout(right.GetFontStrikeout());
	SetInterval(right.GetInterval());
	SetHorizAllig(right.GetHorizAllig());
	SetVertiAllig(right.GetVertiAllig());
	SetOffsetX(right.GetOffsetX());
	SetOffsetY(right.GetOffsetY());
	SetAnnoType(right.GetAnnoType());
	SetShowWithScale(right.GetShowWithScale());
	SetAllowOverlapped(right.GetAllowOvelapped());
	SetAllowRemoveDuplicated(right.GetAllowRemoveDuplicated());
	SetField(right.GetField());
	SetLabelPosition(right.GetLablePosition());
	SetPrecision(right.GetPrecision());
	SetEdgeColor(right.GetEdgeColor());
	SetUseBigEdge(right.GetUseBigEdge());
	SetFillPolygonSymbol(right.GetFillPolygonSymbol());

}
void	MSLabelThematicObj::SetColor(MSColor dstColor)
{
	m_colorLabel = dstColor;
}
void	MSLabelThematicObj::SetSize( double dSize )
{
	m_dSizeLabel = dSize;
}
void	MSLabelThematicObj::SetFontName( const char* strName )
{
	assert(strName!=0);
	if(strName==0)return;

	strcpy(m_fontLabel._fontName, strName);
}
void	MSLabelThematicObj::SetFontItalic( bool bb )
{
	m_fontLabel._italic = bb;
}
void	MSLabelThematicObj::SetFontBold( bool bb )
{
	m_fontLabel._blod = bb;
}
void	MSLabelThematicObj::SetFontUnderline(bool bb)
{
	m_fontLabel._underline = bb;
}
void	MSLabelThematicObj::SetFontStrikeout(bool bb)
{
	m_fontLabel._strikeout = bb;
}
void	MSLabelThematicObj::SetInterval( double interval )
{
	m_dIntervalLabel = interval;
}
void	MSLabelThematicObj::SetHorizAllig( MSHorizontalAligmentType type )
{
	m_eHorizontalAlignment = type;
}
void	MSLabelThematicObj::SetVertiAllig( MSVerticalAligmentType type )
{
	m_eVerticalAlignment = type;
}
void	MSLabelThematicObj::SetOffsetX( double offsetX )
{
	m_dOffsetX = offsetX;
}
void	MSLabelThematicObj::SetOffsetY( double offsetY )
{
	m_dOffsetY = offsetY;
}
void	MSLabelThematicObj::SetAnnoType( MSANNOTYPE type )
{
	m_eLabelType = type;
}
void	MSLabelThematicObj::SetShowWithScale( bool bb )
{
	m_bShowWithScale = bb;
}
void	MSLabelThematicObj::SetAllowOverlapped( bool bb )
{
	m_bAllowOvelapped = bb;
}
void	MSLabelThematicObj::SetAllowRemoveDuplicated( bool bb )
{
	m_bAllowRemoveDuplicated = bb;
}
void	MSLabelThematicObj::SetField(const char* pField)
{
	assert(pField);
	if(pField==0)return;

	strcpy(m_cField, pField);
}
MSColor MSLabelThematicObj::GetColor()
{
	return m_colorLabel;
}
double	MSLabelThematicObj::GetSize()
{
	return m_dSizeLabel;
}
char*	MSLabelThematicObj::GetFontName()
{
	return m_fontLabel._fontName;
}
bool	MSLabelThematicObj::GetFontStrikeout()
{
	return m_fontLabel._strikeout;
}
bool	MSLabelThematicObj::GetFontItalic()
{
	return m_fontLabel._italic;
}
bool	MSLabelThematicObj::GetFontBold()
{
	return m_fontLabel._blod;
}
bool	MSLabelThematicObj::GetFontUnderline()
{
	return m_fontLabel._underline;
}
double	MSLabelThematicObj::GetInterval()
{
	return m_dIntervalLabel;
}
vector<MSPointLabelPosition> MSLabelThematicObj::GetLablePosition()
{
	return m_posLabel;
}
void	MSLabelThematicObj::SetLabelPosition(vector<MSPointLabelPosition> pos)
{
	m_posLabel.clear();
	for (int i=0;i<pos.size();i++)
	{
		m_posLabel.push_back(pos.at(i));
	}
}
MSHorizontalAligmentType MSLabelThematicObj::GetHorizAllig()
{
	return m_eHorizontalAlignment;
}
MSVerticalAligmentType MSLabelThematicObj::GetVertiAllig()
{
	return m_eVerticalAlignment;
}
double	MSLabelThematicObj::GetOffsetX()
{
	return m_dOffsetX;
}
double	MSLabelThematicObj::GetOffsetY()
{
	return m_dOffsetY;
}
MSANNOTYPE MSLabelThematicObj::GetAnnoType()
{
	return m_eLabelType;
}
bool	MSLabelThematicObj::GetShowWithScale()
{
	return m_bShowWithScale;
}
bool	MSLabelThematicObj::GetAllowOvelapped()
{
	return m_bAllowOvelapped;
}
bool	MSLabelThematicObj::GetAllowRemoveDuplicated()
{
	return m_bAllowRemoveDuplicated;
}
char*	MSLabelThematicObj::GetField()
{
	return m_cField;
}
void	MSLabelThematicObj::CloneTo( MSLabelThematicObj& other )
{
	other.SetColor(GetColor());
	other.SetSize(GetSize());
	other.SetFontName(GetFontName());
	other.SetFontItalic(GetFontItalic());
	other.SetFontBold(GetFontBold());
	other.SetFontUnderline(GetFontUnderline());
	other.SetFontStrikeout(GetFontStrikeout());
	other.SetInterval(GetInterval());
	other.SetHorizAllig(GetHorizAllig());
	other.SetVertiAllig(GetVertiAllig());
	other.SetOffsetX(GetOffsetX());
	other.SetOffsetY(GetOffsetY());
	other.SetAnnoType(GetAnnoType());
	other.SetShowWithScale(GetShowWithScale());
	other.SetAllowOverlapped(GetAllowOvelapped());
	other.SetAllowRemoveDuplicated(GetAllowRemoveDuplicated());
	other.SetField(GetField());
	other.SetLabelPosition(GetLablePosition());
	other.SetPrecision(GetPrecision());
	other.SetEdgeColor(GetEdgeColor());
	other.SetUseBigEdge(GetUseBigEdge());
	other.SetFillPolygonSymbol(GetFillPolygonSymbol());
	
}
void	MSLabelThematicObj::SetEdgeColor(MSColor egColor)
{
	m_colorEdge = egColor;
}
MSColor	MSLabelThematicObj::GetEdgeColor()
{
	return m_colorEdge;
}
void	MSLabelThematicObj::SetUseBigEdge(bool bUseBigEdge/* = true*/)
{
	m_bUseBigEdge = bUseBigEdge;
}
bool	MSLabelThematicObj::GetUseBigEdge()
{
	return m_bUseBigEdge;
}
MSSymbolObj* MSLabelThematicObj::GetFillPolygonSymbol() 
{ 
	return m_pFillPolygonSymbol; 
}
bool	MSLabelThematicObj::SetFillPolygonSymbol(MSSymbolObj* pSymbolObj) 
{ 
	if(!pSymbolObj)
		return false;
	MSSymbolType type = pSymbolObj->GetSymbolType();
	if(type < MS_SIMPLE_FILL_SYMBOL || type > MS_MULTILAYER_FILL_SYMBOL)
		return false;
	m_pFillPolygonSymbol = pSymbolObj; 
	return true;
}



MSSimpleThematicObj::MSSimpleThematicObj()
{
	m_eThematicType = MS_THEMATIC_SIMPLE;

	m_pSymbolObj = 0;
}
MSSimpleThematicObj::MSSimpleThematicObj(const MSSimpleThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_SIMPLE;

	m_pSymbolObj = srcObj.m_pSymbolObj;
}
MSSimpleThematicObj::~MSSimpleThematicObj()
{
	if(m_pSymbolObj!=0)
	{
		delete m_pSymbolObj;
		m_pSymbolObj = 0;
	}

	for (int i = _toDelete.size() - 1; i >= 0; i--)
	{
		MSSymbolObj* pSymbolObj = _toDelete.at(i);
		if (pSymbolObj)
		{
			delete pSymbolObj;
			pSymbolObj = NULL;
		}
	}
	_toDelete.clear();
}
void	MSSimpleThematicObj::SetSymbolObj( MSSymbolObj* symbolObj/*,bool bDelOldSymbol = true*/)
{
//  防止内存泄漏，记住，最后析构时一起删除

	if (symbolObj == m_pSymbolObj)
		return;

	if (m_pSymbolObj && find(_toDelete.begin(), _toDelete.end(), m_pSymbolObj) == _toDelete.end())
	{
		_toDelete.push_back(m_pSymbolObj);
	}
	m_pSymbolObj = symbolObj;
}
MSSymbolObj* MSSimpleThematicObj::GetSymbolObj()
{
	return m_pSymbolObj;
}


MSCategoryThematicObj::MSCategoryThematicObj()
{
	m_eThematicType = MS_THEMATIC_CATEGORY;

	m_VectValueSymbolObj.clear();

	m_cField1[0] = '\0';
	m_cField2[0] = '\0';
	m_colorRampMin = MSColor::_MSColor(0,0,255);
	m_colorRampMax = MSColor::_MSColor(255,0,0);
	m_colorRampNum = 10;
	m_bUseColorRampNum = FALSE;
	m_bUseRandomColor = FALSE;
}
MSCategoryThematicObj::MSCategoryThematicObj(const MSCategoryThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_CATEGORY;

	for (int i = 0; i < srcObj.m_VectValueSymbolObj.size(); i++)
	{
		MSValueSymbolObj* pValueSymbolObj = new MSValueSymbolObj(*srcObj.m_VectValueSymbolObj.at(i));
		m_VectValueSymbolObj.push_back(pValueSymbolObj);
	}
	strcpy(m_cField1,srcObj.m_cField1);
	strcpy(m_cField2,srcObj.m_cField2);
	m_colorRampMin = srcObj.m_colorRampMin;
	m_colorRampMax = srcObj.m_colorRampMax;
	m_colorRampNum = srcObj.m_colorRampNum;
	m_bUseColorRampNum = srcObj.m_bUseColorRampNum;
}
MSCategoryThematicObj::~MSCategoryThematicObj()
{
	ClearAllSymbolObj();
}
void	MSCategoryThematicObj::SetField1( const char* strField )
{
	if(strField==0)
	{
		m_cField1[0] = '\0';
		return;
	}

	strcpy(m_cField1, strField);
}
void	MSCategoryThematicObj::SetField2( const char* strField )
{
	if(strField==0)
	{
		m_cField2[0] = '\0';
		return;
	}

	strcpy(m_cField2, strField);
}
char*	MSCategoryThematicObj::GetField1()
{
	return m_cField1;
}
char*	MSCategoryThematicObj::GetField2()
{
	return m_cField2;
}
int		MSCategoryThematicObj::InsertSymbolObj( MSValueSymbolObj* symbolObj )
{
	m_VectValueSymbolObj.push_back(symbolObj);

	return 0;
}
void	MSCategoryThematicObj::ClearAllSymbolObj()
{
	int nSize = (int)m_VectValueSymbolObj.size();
	for(int i=0; i<nSize; i++)
	{
		MSValueSymbolObj* p = m_VectValueSymbolObj.at(i);

		delete p;
		p = 0;
	}
	m_VectValueSymbolObj.clear();
}
int		MSCategoryThematicObj::RemoveSymbolObj( int iIdex )
{
	int nSize = (int)m_VectValueSymbolObj.size();
	if(iIdex<0 || iIdex>nSize-1)return -1;

	ITT itt = m_VectValueSymbolObj.begin();
	for(int i=0; i<iIdex; i++)itt++;

	MSValueSymbolObj* p = m_VectValueSymbolObj.at(iIdex);
	delete p; p = 0;

	m_VectValueSymbolObj.erase(itt);

	return 0;
}
int		MSCategoryThematicObj::GetSymbolCount()
{
	return (int)m_VectValueSymbolObj.size();
}
MSValueSymbolObj* MSCategoryThematicObj::GetValueSymbol(int iIdex)
{
	if(iIdex<0 || iIdex>GetSymbolCount()-1)return 0;

	return m_VectValueSymbolObj.at(iIdex);
}
MSColor		MSCategoryThematicObj::GetColorRampMin()
{
	return m_colorRampMin;
}
void		MSCategoryThematicObj::SetColorRampMin(MSColor clr)
{
	m_colorRampMin = clr;
}
MSColor		MSCategoryThematicObj::GetColorRampMax()
{
	return m_colorRampMax;
}
void		MSCategoryThematicObj::SetColorRampMax(MSColor clr)
{
	m_colorRampMax = clr;
}
int			MSCategoryThematicObj::GetColorRampNum()
{
	return m_colorRampNum;
}
void		MSCategoryThematicObj::SetColorRampNum(int num)
{
	m_colorRampNum = num;
}
BOOL		MSCategoryThematicObj::GetUseColorRampNum()
{
	return m_bUseColorRampNum;
}
void		MSCategoryThematicObj::SetUseColorRampNum(BOOL b)
{
	m_bUseColorRampNum = b;
}
BOOL		MSCategoryThematicObj::GetUseRandomColor()
{
	return m_bUseRandomColor;
}
void		MSCategoryThematicObj::SetUseRandomColor(BOOL b)
{
	m_bUseRandomColor = b;
}


MSGraduateThematicObj::MSGraduateThematicObj()
{
	m_eThematicType = MS_THEMATIC_GRADUATE;

	m_VectRangeSymbolObj.clear();

	m_cField[0] = '\0';
	m_colorRampMin = MSColor::_MSColor(0,0,255);
	m_colorRampMax = MSColor::_MSColor(255,0,0);
	m_colorRampNum = 10;
	m_bUseColorRampNum = FALSE;
	m_bUseRandomColor = FALSE;
}
MSGraduateThematicObj::MSGraduateThematicObj(const MSGraduateThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_GRADUATE;

	for (int i=0;i<srcObj.m_VectRangeSymbolObj.size();i++)
	{
		MSRangeSymbolObj* pRangeSymbolObj = new MSRangeSymbolObj(*srcObj.m_VectRangeSymbolObj.at(i));
		m_VectRangeSymbolObj.push_back(pRangeSymbolObj);
	}
	strcpy(m_cField,srcObj.m_cField);
	m_colorRampMin = srcObj.m_colorRampMin;
	m_colorRampMax = srcObj.m_colorRampMax;
	m_colorRampNum = srcObj.m_colorRampNum;
	m_bUseColorRampNum = srcObj.m_bUseColorRampNum;
	m_bUseRandomColor = srcObj.m_bUseColorRampNum;
	m_eClsType = srcObj.m_eClsType;
	m_dInterval = srcObj.m_dInterval;
}
MSGraduateThematicObj::~MSGraduateThematicObj()
{
	ClearAllSymbolObj();
}
void MSGraduateThematicObj::SetField( const char* strField )
{
	if(strField==0)return;

	strcpy(m_cField, strField);
}
char*	MSGraduateThematicObj::GetField()
{
	return m_cField;
}
int		MSGraduateThematicObj::InsertSymbolObj( MSRangeSymbolObj* symbolObj )
{
	m_VectRangeSymbolObj.push_back(symbolObj);

	return 0;
}
int		MSGraduateThematicObj::RemoveSymbolObj( int iIdex )
{
	int nSize = (int)m_VectRangeSymbolObj.size();
	if(iIdex<0 || iIdex>nSize-1)return -1;

	ITT itt = m_VectRangeSymbolObj.begin();
	for(int i=0; i<iIdex; i++)itt++;

	MSRangeSymbolObj* p = m_VectRangeSymbolObj.at(iIdex);
	delete p; p = 0;

	m_VectRangeSymbolObj.erase(itt);

	return 0;
}
void	MSGraduateThematicObj::ClearAllSymbolObj()
{
	int nSize = (int)m_VectRangeSymbolObj.size();
	for(int i=0; i<nSize; i++)
	{
		MSRangeSymbolObj* p = m_VectRangeSymbolObj.at(i);

		delete p;
		p = 0;
	}
	m_VectRangeSymbolObj.clear();
}
int		MSGraduateThematicObj::GetSymbolCnt()
{
	return (int)m_VectRangeSymbolObj.size();
}
MSRangeSymbolObj* MSGraduateThematicObj::GetRangeSymbol(int iIdex)
{
	if(iIdex<0 || iIdex>GetSymbolCnt()-1)return 0;

	return m_VectRangeSymbolObj.at(iIdex);
}
MSColor		MSGraduateThematicObj::GetColorRampMin()
{
	return m_colorRampMin;
}
void		MSGraduateThematicObj::SetColorRampMin(MSColor clr)
{
	m_colorRampMin = clr;
}
MSColor		MSGraduateThematicObj::GetColorRampMax()
{
	return m_colorRampMax;
}
void		MSGraduateThematicObj::SetColorRampMax(MSColor clr)
{
	m_colorRampMax = clr;
}
int			MSGraduateThematicObj::GetColorRampNum()
{
	return m_colorRampNum;
}
void		MSGraduateThematicObj::SetColorRampNum(int num)
{
	m_colorRampNum = num;
}
BOOL		MSGraduateThematicObj::GetUseColorRampNum()
{
	return m_bUseColorRampNum;
}
void		MSGraduateThematicObj::SetUseColorRampNum(BOOL b)
{
	m_bUseColorRampNum = b;
}
BOOL		MSGraduateThematicObj::GetUseRandomColor()
{
	return m_bUseRandomColor;
}
void		MSGraduateThematicObj::SetUseRandomColor(BOOL b)
{
	m_bUseRandomColor = b;
}
MSClsType	MSGraduateThematicObj::GetClsType()
{
	return m_eClsType;
}
void		MSGraduateThematicObj::SetClsType(MSClsType c)
{
	m_eClsType = c;
}
double		MSGraduateThematicObj::GetInterval()
{
	return m_dInterval;
}
void		MSGraduateThematicObj::SetInterval(double v)
{
	m_dInterval = v;
}


MSChartThematicObj::MSChartThematicObj()
{
	m_eThematicType = MS_THEMATIC_CHART;
}
MSChartThematicObj::MSChartThematicObj(const MSChartThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_CHART;
}
MSChartThematicObj::~MSChartThematicObj()
{

}


MSRGBRasterThematicObj::MSRGBRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_COLOR;

	m_colorMode = COLOR_TYPE_RGB;
	m_dMinValueR = m_dMinValueG = m_dMinValueB = -MAXNUM_MSTHEMATIC;
	m_dMaxValueR = m_dMaxValueG = m_dMaxValueB = -MAXNUM_MSTHEMATIC;
	m_nChannelR = 0;
	m_nChannelG = 0;
	m_nChannelB = 0;
	m_nChannelA = 0;
	m_eScalingMethod = XML_SCALINGMETHORD_NULL;
	m_bChannelRVisible = m_bChannelGVisible = m_bChannelBVisible = m_bCheckBackGround = true;
	m_bChannelAVisible = false;
	m_nBackGroundValueR = m_nBackGroundValueG = m_nBackGroundValueB = -9999;
	m_bInvert = false;
}
MSRGBRasterThematicObj::MSRGBRasterThematicObj(const MSRGBRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_COLOR;

	m_dMinValueR = srcObj.m_dMinValueR;
	m_dMinValueG = srcObj.m_dMinValueG;
	m_dMinValueB = srcObj.m_dMinValueB;
	m_dMaxValueR = srcObj.m_dMaxValueR;
	m_dMaxValueG = srcObj.m_dMaxValueG;
	m_dMaxValueB = srcObj.m_dMaxValueB;
	m_colorMode = srcObj.m_colorMode;
	m_nChannelR = srcObj.m_nChannelR;
	m_nChannelG = srcObj.m_nChannelG;
	m_nChannelB = srcObj.m_nChannelB;
	m_nChannelA = srcObj.m_nChannelA;
	m_eScalingMethod = srcObj.m_eScalingMethod;
	m_nBackGroundValueR = srcObj.m_nBackGroundValueR;
	m_nBackGroundValueG = srcObj.m_nBackGroundValueG;
	m_nBackGroundValueB = srcObj.m_nBackGroundValueB;
}
MSRGBRasterThematicObj::~MSRGBRasterThematicObj()
{
}
void	MSRGBRasterThematicObj::SetColorMode( MSColorType type )
{
	m_colorMode = type;
}
void	MSRGBRasterThematicObj::SetChannelR( int Channel )
{
	m_nChannelR = Channel;
}
void	MSRGBRasterThematicObj::SetChannelG( int Channel )
{
	m_nChannelG = Channel;
}
void	MSRGBRasterThematicObj::SetChannelB( int Channel )
{
	m_nChannelB = Channel;
}
void	MSRGBRasterThematicObj::SetChannelA( int Channel )
{
	m_nChannelA = Channel;
}
void	MSRGBRasterThematicObj::SetChannelRVisible(bool bV)
{
	m_bChannelRVisible = bV;
}
void	MSRGBRasterThematicObj::SetChannelGVisible(bool bV)
{
	m_bChannelGVisible = bV;
}
void	MSRGBRasterThematicObj::SetChannelBVisible(bool bV)
{
	m_bChannelBVisible = bV;
}
void	MSRGBRasterThematicObj::SetChannelAVisible(bool bV)
{
	m_bChannelAVisible = bV;
}
void    MSRGBRasterThematicObj::SetCheckBackGround(bool bV)
{
	m_bCheckBackGround = bV;
}
void	MSRGBRasterThematicObj::SetInvert(bool bV)
{
	m_bInvert = bV;
}
void	MSRGBRasterThematicObj::SetNoDataColor(MSColor rgb)
{
	m_colorNoData = rgb;
}
void	MSRGBRasterThematicObj::SetBackGroundColor(MSColor rgb)
{
	m_colorBackGround = rgb;
}
void	MSRGBRasterThematicObj::SetBackGroundValue(int nBackGroundR, int nBackGroundG, int nBackGroundB)
{
	m_nBackGroundValueR = nBackGroundR;
	m_nBackGroundValueG = nBackGroundG;
	m_nBackGroundValueB = nBackGroundB;
}
void	MSRGBRasterThematicObj::SetStretchMethod(MSXMLScalingMethord method)
{
	m_eScalingMethod = method;
}
MSColorType MSRGBRasterThematicObj::GetColorMode()
{
	return m_colorMode;
}
int		MSRGBRasterThematicObj::GetChannelR()
{
	return m_nChannelR;
}
int		MSRGBRasterThematicObj::GetChannelG()
{
	return m_nChannelG;
}
int		MSRGBRasterThematicObj::GetChannelB()
{
	return m_nChannelB;
}
int		MSRGBRasterThematicObj::GetChannelA()
{
	return m_nChannelA;
}
bool    MSRGBRasterThematicObj::GetChannelRVisible()
{
	return m_bChannelRVisible;
}
bool    MSRGBRasterThematicObj::GetChannelGVisible()
{
	return m_bChannelGVisible;
}
bool    MSRGBRasterThematicObj::GetChannelBVisible()
{
	return m_bChannelBVisible;
}
bool    MSRGBRasterThematicObj::GetChannelAVisible()
{
	return m_bChannelAVisible;
}
bool	MSRGBRasterThematicObj::GetCheckBackGround()
{
	return m_bCheckBackGround;
}
MSColor MSRGBRasterThematicObj::GetNoDataColor()
{
	return m_colorNoData;
}
MSColor MSRGBRasterThematicObj::GetBackGroundColor()
{
	return m_colorBackGround;
}
void	MSRGBRasterThematicObj::GetBackGroundValue(int& nBackGroundR, int& nBackGroundG, int& nBackGroundB)
{
	nBackGroundR = m_nBackGroundValueR;
	nBackGroundG = m_nBackGroundValueG;
	nBackGroundB = m_nBackGroundValueB;
}
bool	MSRGBRasterThematicObj::GetInvert()
{
	return m_bInvert;
}
MSXMLScalingMethord	MSRGBRasterThematicObj::GetStretchMethod()
{
	return m_eScalingMethod;
}
void	MSRGBRasterThematicObj::SetMinMaxValueR(double dMin,double dMax)
{
	m_dMinValueR = dMin;
	m_dMaxValueR = dMax;
}
bool	MSRGBRasterThematicObj::GetMinMaxValueR(double& dMin,double& dMax)
{
	dMin = m_dMinValueR;
	dMax = m_dMaxValueR;
	if(m_dMinValueR == -MAXNUM_MSTHEMATIC || m_dMaxValueR == -MAXNUM_MSTHEMATIC /*|| m_dMinValueR == m_dMaxValueR*/)
		return false;
	return true;
}
void	MSRGBRasterThematicObj::SetMinMaxValueG(double dMin,double dMax)
{
	m_dMinValueG = dMin;
	m_dMaxValueG = dMax;
}
bool	MSRGBRasterThematicObj::GetMinMaxValueG(double& dMin,double& dMax)
{
	dMin = m_dMinValueG;
	dMax = m_dMaxValueG;
	if(m_dMinValueG == -MAXNUM_MSTHEMATIC || m_dMaxValueG == -MAXNUM_MSTHEMATIC /*|| _minValueG == m_dMaxValueG*/)
		return false;
	return true;
}
void	MSRGBRasterThematicObj::SetMinMaxValueB(double dMin,double dMax)
{
	m_dMinValueB = dMin;
	m_dMaxValueB = dMax;
}
bool	MSRGBRasterThematicObj::GetMinMaxValueB(double& dMin,double& dMax)
{
	dMin = m_dMinValueB;
	dMax = m_dMaxValueB;
	if(m_dMinValueB == -MAXNUM_MSTHEMATIC || m_dMaxValueB == -MAXNUM_MSTHEMATIC /*|| m_dMinValueB == _maxValueB*/)
		return false;
	return true;
}
void	MSRGBRasterThematicObj::ResetMinMaxValueR()
{
	m_dMinValueR = -MAXNUM_MSTHEMATIC;
	m_dMaxValueR = -MAXNUM_MSTHEMATIC;
}
void	MSRGBRasterThematicObj::ResetMinMaxValueG()
{
	m_dMinValueG = -MAXNUM_MSTHEMATIC;
	m_dMaxValueG = -MAXNUM_MSTHEMATIC;
}
void	MSRGBRasterThematicObj::ResetMinMaxValueB()
{
	m_dMinValueB = -MAXNUM_MSTHEMATIC;
	m_dMaxValueB = -MAXNUM_MSTHEMATIC;
}


MSClassificationRasterThematicObj::MSClassificationRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_CLASSIFICATION;

	m_VectRangeSymbolObj.clear();

	m_nBand = 1;
	m_dMinValue = m_dMaxValue = -MAXNUM_MSTHEMATIC;
	m_bHasPrint = false;
	m_nHillZValue = 0;
	m_bUseScheme = false;
}
MSClassificationRasterThematicObj::MSClassificationRasterThematicObj(const MSClassificationRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_CLASSIFICATION;

	m_dMinValue = srcObj.m_dMinValue;
	m_dMaxValue = srcObj.m_dMaxValue;
	m_bHasPrint = srcObj.m_bHasPrint;
	for (int i=0;i<srcObj.m_VectRangeSymbolObj.size();i++)
	{
		MSRangeSymbolObj* pRangeSymbolObj = srcObj.m_VectRangeSymbolObj.at(i);
		MSRangeSymbolObj* pNewRangeSymbolObj = new MSRangeSymbolObj(*pRangeSymbolObj);
		m_VectRangeSymbolObj.push_back(pNewRangeSymbolObj);
	}
// 	_colorScheme = srcObj._colorScheme;
	m_nBand = srcObj.m_nBand;
	m_colorNoData = srcObj.m_colorNoData;
	m_nHillZValue = srcObj.m_nHillZValue;
	m_bUseScheme = srcObj.m_bUseScheme;
}
MSClassificationRasterThematicObj::~MSClassificationRasterThematicObj()
{
	ClearAllSymbolObj();
}
void	MSClassificationRasterThematicObj::SetBand( int nBand )
{
	m_nBand = nBand;
}
int		MSClassificationRasterThematicObj::GetBand()
{
	return m_nBand;
}
void	MSClassificationRasterThematicObj::SetHillShade(int hillz)
{
	m_nHillZValue = hillz;
}
int		MSClassificationRasterThematicObj::GetHillShade()
{
	return m_nHillZValue;
}
void	MSClassificationRasterThematicObj::SetbUseScheme(bool bUse)
{
	m_bUseScheme = bUse;
}
bool	MSClassificationRasterThematicObj::GetbUseScheme()
{
	return m_bUseScheme;
}
void MSClassificationRasterThematicObj::SetColor( int iIndex, MSColor color )
{
	int nSize = (int)m_VectRangeSymbolObj.size();
	if(iIndex >= nSize || iIndex < 0)
		ASSERT_FALSE_AND_RETURN;

	MSRangeSymbolObj* pRangeSymbolObj = m_VectRangeSymbolObj.at(iIndex);
	MSSymbolObj* pSymbolObj = pRangeSymbolObj->GetSymbol();
	if (!pSymbolObj)
		ASSERT_FALSE_AND_RETURN;
	MSSymbolType type = pSymbolObj->GetSymbolType();
	if (type != MS_SIMPLE_FILL_SYMBOL)
		ASSERT_FALSE_AND_RETURN;
	MSSimpleFillSymbolObj* pSimpleFillSymbolObj = (MSSimpleFillSymbolObj*)pSymbolObj;
	pSimpleFillSymbolObj->SetFillColor(color);
}

int		MSClassificationRasterThematicObj::AddSymbolObj( MSRangeSymbolObj* symbolObj)
{
	m_VectRangeSymbolObj.push_back(symbolObj);
	return 0;
}
int		MSClassificationRasterThematicObj::RemoveSymbolObj( int iIdex )
{
	int nSize = (int)m_VectRangeSymbolObj.size();
	if(iIdex<0 || iIdex>nSize-1)return -1;

	ITT itt = m_VectRangeSymbolObj.begin();
	for(int i=0; i<iIdex; i++)itt++;

	MSRangeSymbolObj* p = m_VectRangeSymbolObj.at(iIdex);
	delete p; p = 0;

	m_VectRangeSymbolObj.erase(itt);

	return 0;
}
void	MSClassificationRasterThematicObj::ClearAllSymbolObj()
{
	int nSize = (int)m_VectRangeSymbolObj.size();
	for(int i=nSize-1; i>=0; i--)
	{
		MSRangeSymbolObj* p = m_VectRangeSymbolObj.at(i);

		delete p;
		p = 0;
	}
	m_VectRangeSymbolObj.clear();
}
int		MSClassificationRasterThematicObj::GetSymbolCount()
{
	return (int)m_VectRangeSymbolObj.size();
}
MSRangeSymbolObj* MSClassificationRasterThematicObj::GetRangeSymbol(int iIdex)
{
	if(iIdex<0 || iIdex>GetSymbolCount()-1)return 0;

	return m_VectRangeSymbolObj.at(iIdex);
}
void	MSClassificationRasterThematicObj::SetNoDataColor(MSColor rgb)
{
	m_colorNoData = rgb;
}
MSColor	MSClassificationRasterThematicObj::GetNoDataColor()
{
	return m_colorNoData;
}
MSColor	MSClassificationRasterThematicObj::GetColor(double dValue)
{
	for (int i=0;i<m_VectRangeSymbolObj.size();i++)
	{
		//这2个是一一对应的
		MSRangeSymbolObj* pRangeSymbolObj = m_VectRangeSymbolObj.at(i);
		double dMin = pRangeSymbolObj->GetMinValue();
		double dMax = pRangeSymbolObj->GetMaxValue();
		if(dMax == dMin)continue;
		if (dMin <= dValue && dValue < dMax || dValue == dMax && i == m_VectRangeSymbolObj.size() - 1)
		{
			MSSymbolObj* pSymbolObj = pRangeSymbolObj->GetSymbol();
			if (!pSymbolObj)
			{
				ASSERT(FALSE);;
				return _MSColor(0, 0, 0, 0);
			}
			MSSymbolType type = pSymbolObj->GetSymbolType();
			if (type != MS_SIMPLE_FILL_SYMBOL)
			{
				ASSERT(FALSE);;
				return _MSColor(0, 0, 0, 0);
			}
			MSSimpleFillSymbolObj* pSimpleFillSymbolObj = (MSSimpleFillSymbolObj*)pSymbolObj;
			return pSimpleFillSymbolObj->GetFillColor();
		}
	}
	return _MSColor(0,0,0,0);
}
void	MSClassificationRasterThematicObj::SetMinMaxValue(double dMin,double dMax)
{
	assert(dMax >= dMin);
	m_dMinValue = dMin;
	m_dMaxValue = dMax;
}
bool	MSClassificationRasterThematicObj::GetMinMaxValue(double& dMin,double& dMax)
{
	dMin = m_dMinValue;
	dMax = m_dMaxValue;
	if(m_dMinValue == -MAXNUM_MSTHEMATIC || m_dMaxValue == -MAXNUM_MSTHEMATIC || m_dMinValue == m_dMaxValue)
		return false;
	return true;
}
// MSColorScheme* MSClassificationRasterThematicObj::GetColorSchemeObj()
// {
// 	return &_colorScheme;
// }


MSStretchRasterThematicObj::MSStretchRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_STRETCH;

	m_nHillZValue = 0;

	m_dGrammStretch = 0;
// 	_minValue = _maxValue = -MAXNUM_MSTHEMATIC;

	m_nBand = 1;
	m_nBackgroundValue = -1;
	m_nHillZValue = 0;
	m_eStretchType = XML_SCALINGMETHORD_NULL;
	m_bInvert = false;
	m_dStdev = 0;
	m_dGrammStretch = 0;
	m_eStatisticType = MS_STATISTIC_EACH_RASTER;
}
MSStretchRasterThematicObj::MSStretchRasterThematicObj(const MSStretchRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_STRETCH;

	m_nBand = srcObj.m_nBand;
	m_colorRamps = srcObj.m_colorRamps;
	for (int i=0;i<3;i++)
	{
		m_sLabels[i] = srcObj.m_sLabels[i];
	}
	m_nBackgroundValue = srcObj.m_nBackgroundValue;
	m_colorBackground = srcObj.m_colorBackground;
	m_colorNoData = srcObj.m_colorNoData;
	m_nHillZValue = srcObj.m_nHillZValue;
	m_eStretchType = srcObj.m_eStretchType;
	m_bInvert = srcObj.m_nBand;
	m_dStdev = srcObj.m_dStdev;
	m_dGrammStretch = srcObj.m_dGrammStretch;
	m_eStatisticType = srcObj.m_eStatisticType;
	m_sStatisticValue = srcObj.m_sStatisticValue;
}
MSStretchRasterThematicObj::~MSStretchRasterThematicObj()
{

}
void	MSStretchRasterThematicObj::SetUpLabel( const char* strLabel )
{
	m_sLabels[0] = strLabel;
}
void	MSStretchRasterThematicObj::SetMidLabel( const char* strLabel )
{
	m_sLabels[1] = strLabel;
}
void	MSStretchRasterThematicObj::SetLowLabel( const char* strLabel )
{
	m_sLabels[2] = strLabel;
}
void	MSStretchRasterThematicObj::SetBackGroundValue( int bgValue )
{
	m_nBackgroundValue = bgValue;
}
void	MSStretchRasterThematicObj::SetBackGroundColor( MSColor bgColor )
{
	m_colorBackground = bgColor;
}
void	MSStretchRasterThematicObj::SetHillShade( int hillz )
{
	m_nHillZValue = hillz;
}
void	MSStretchRasterThematicObj::SetbInvert( bool bInvert )
{
	m_bInvert = bInvert;
}
void	MSStretchRasterThematicObj::SetSTDValue( float stdValue )
{
	m_dStdev = stdValue;
}
void	MSStretchRasterThematicObj::SetGrammStretch( float gramValue )
{
	m_dGrammStretch = gramValue;
}
void	MSStretchRasterThematicObj::SetStatisticType( MSStatisticType type )
{
	m_eStatisticType = type;
}
void	MSStretchRasterThematicObj::SetStatisticMin( float sMin )
{
	m_sStatisticValue._fMin = sMin;
}
void	MSStretchRasterThematicObj::SetStatisticMax( float sMax )
{
	m_sStatisticValue._fMax = sMax;
}
void	MSStretchRasterThematicObj::SetStatisticMean( float sMean )
{
	m_sStatisticValue._fMean = sMean;
}
void	MSStretchRasterThematicObj::SetStatisticSTD( float stdVale )
{
	m_sStatisticValue._fStd = stdVale;
}
void	MSStretchRasterThematicObj::SetColorInternalType( MSColorInternalType type )
{
	m_colorRamps._internalType = type;
}
MSColorInternalType MSStretchRasterThematicObj::GetColorInternalType()
{
	return m_colorRamps._internalType;
}
void	MSStretchRasterThematicObj::ClearAllColorRampPoints()
{
	m_colorRamps._pair.clear();
}
int		MSStretchRasterThematicObj::GetColorRampPointsCount()
{
	return (int)m_colorRamps._pair.size();
}
void	MSStretchRasterThematicObj::GetColorRampPoint( int iIndex, double& pos, MSColor& colr )
{
	if(iIndex<0 || iIndex>=m_colorRamps._pair.size())
	{
		pos = -1;
		return;
	}

	pos = m_colorRamps._pair.at(iIndex)._value;
	colr = m_colorRamps._pair.at(iIndex)._color;
}
void	MSStretchRasterThematicObj::SetColorRampPoint( int iIndex, double pos, MSColor colr )
{
	if(iIndex<0 || iIndex>=m_colorRamps._pair.size())
	{
		return;
	}

	m_colorRamps._pair.at(iIndex)._value = pos;
	m_colorRamps._pair.at(iIndex)._color = colr;
}
void	MSStretchRasterThematicObj::AddColorRampPoint( double pos, MSColor colr )
{
	Double_MSColor tmp;
	tmp._color = colr;
	tmp._value = pos;
	m_colorRamps._pair.push_back(tmp);
}
void	MSStretchRasterThematicObj::RemoveColorRampPointAt( int iIndex )
{
	if(iIndex<0 || iIndex>=m_colorRamps._pair.size())
		return;
	m_colorRamps._pair.erase(m_colorRamps._pair.begin()+iIndex);
}
const char* MSStretchRasterThematicObj::GetUpLabel()
{
	return m_sLabels[0].c_str();
}
const char*	MSStretchRasterThematicObj::GetMidLabel()
{
	return m_sLabels[1].c_str();
}
const char*	MSStretchRasterThematicObj::GetLowLabel()
{
	return m_sLabels[2].c_str();
}
int		MSStretchRasterThematicObj::GetBackGroundValue()
{
	return m_nBackgroundValue;
}
MSColor MSStretchRasterThematicObj::GetBackGroundColor()
{
	return m_colorBackground;
}
int		MSStretchRasterThematicObj::GetHillShade()
{
	return m_nHillZValue;
}
bool	MSStretchRasterThematicObj::GetbInvert()
{
	return m_bInvert;
}
double	MSStretchRasterThematicObj::GetSTDValue()
{
	return m_dStdev;
}
double	MSStretchRasterThematicObj::GetGrammStretch()
{
	return m_dGrammStretch;
}
MSStatisticType MSStretchRasterThematicObj::GetStatisticType()
{
	return m_eStatisticType;
}
double	MSStretchRasterThematicObj::GetStatisticMin()
{
	return m_sStatisticValue._fMin;
}
double	MSStretchRasterThematicObj::GetStatisticMax()
{
	return m_sStatisticValue._fMax;
}
double	MSStretchRasterThematicObj::GetStatisticMean()
{
	return m_sStatisticValue._fMean;
}
double	MSStretchRasterThematicObj::GetStatisticSTD()
{
	return m_sStatisticValue._fStd;
}
void	MSStretchRasterThematicObj::SetBand( int band )
{
	m_nBand = band;
}
int		MSStretchRasterThematicObj::GetBand()
{
	return m_nBand;
}
void	MSStretchRasterThematicObj::SetNoDataColor(MSColor colr)
{
	m_colorNoData = colr;
}
MSColor	MSStretchRasterThematicObj::GetNoDataColor()
{
	return m_colorNoData;
}
MSColor	MSStretchRasterThematicObj::GetColor(double dValue)
{
	return m_colorRamps.GetColor(dValue);
}
void	MSStretchRasterThematicObj::SetMinMaxValue(double dMin,double dMax)
{
	assert(dMax >= dMin);
	m_sStatisticValue._fMin = dMin;
	m_sStatisticValue._fMax = dMax;
}
bool	MSStretchRasterThematicObj::GetMinMaxValue(double& dMin,double& dMax)
{
	dMin = m_sStatisticValue._fMin;
	dMax = m_sStatisticValue._fMax;
	if(m_sStatisticValue._fMin == -MAXNUM_MSTHEMATIC || m_sStatisticValue._fMax == -MAXNUM_MSTHEMATIC)
		return false;
	return true;
}
MSColorRamps* MSStretchRasterThematicObj::GetColorRampsObj()
{
	return &m_colorRamps;
}
void	MSStretchRasterThematicObj::SetStretchType( MSXMLScalingMethord type )
{
	m_eStretchType = type;
}
MSXMLScalingMethord MSStretchRasterThematicObj::GetStretchType()
{
	return m_eStretchType;
}
MSStatisticValue* MSStretchRasterThematicObj::GetStasticValueObj()
{
	return &m_sStatisticValue;
}


MSDiscreteRasterThematicObj::MSDiscreteRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_DISCRETE;
	m_dMinValue = m_dMaxValue = -MAXNUM_MSTHEMATIC;
	m_nBand = 1;
	m_bUseRandColor = false;
}
MSDiscreteRasterThematicObj::MSDiscreteRasterThematicObj(const MSDiscreteRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_DISCRETE;

	m_colorRamps = srcObj.m_colorRamps;
	m_colorNoData = srcObj.m_colorNoData;
	m_nBand = srcObj.m_nBand;
	m_dMinValue = srcObj.m_dMinValue;
	m_dMaxValue = srcObj.m_dMaxValue;
	m_bUseRandColor = srcObj.m_bUseRandColor;

	m_colorRamps = srcObj.m_colorRamps;
}
MSDiscreteRasterThematicObj::~MSDiscreteRasterThematicObj()
{

}
void	MSDiscreteRasterThematicObj::SetNoDataColor( MSColor colr )
{
	m_colorNoData = colr;
}
MSColor MSDiscreteRasterThematicObj::GetNoDataColor()
{
	return m_colorNoData;
}
void	MSDiscreteRasterThematicObj::ClearColorScheme()
{
	m_colorRamps._pair.clear();
}
void	MSDiscreteRasterThematicObj::AddColor( double dValue, MSColor colr )
{
	Double_MSColor tmp;
	tmp._color = colr;
	tmp._value = dValue;
	m_colorRamps._pair.push_back(tmp);
}
void	MSDiscreteRasterThematicObj::SetColor( int pos, MSColor colr )
{
	int nSize = (int)m_colorRamps._pair.size();
	if(pos>=nSize || pos<0)return;

	m_colorRamps._pair.at(pos)._color = colr;
}
void	MSDiscreteRasterThematicObj::RemoveColor( int pos )
{
	int nSize = (int)m_colorRamps._pair.size();
	if(pos>=nSize || pos<0)return;

	m_colorRamps._pair.erase(m_colorRamps._pair.begin()+pos);
}
void	MSDiscreteRasterThematicObj::SetBand(int band)
{
	m_nBand = band;
}
int		MSDiscreteRasterThematicObj::GetBand()
{
	return m_nBand;
}
MSColor MSDiscreteRasterThematicObj::GetColor(double dValue)
{
	if(m_colorRamps._pair.size() < 2)
	{
		assert(false);
		return _MSColor(0,0,0,0);
	}
	//首先将 _colorRamps._vctPos 进行降序排列
	sort(m_colorRamps._pair.begin(),m_colorRamps._pair.end());
// 	//同时需要将对应的 进行排序
// 	for (int i=(int)_colorRamps._pair.size()-1;i>=0;i--)
// 	{
// 		double pos = _colorRamps._pair.at(i)._value;
// 		MSColor color = _colorRamps._pair.at(i)._color;
// 		//这里要注意，对于Discrete，是将0～255平均分为若干份（如25），只需要给的dValue >= pos即可
// 		if (dValue >= pos)
// 		{
// 			return color;
// 		}
// 	}
	for (int i = 0; i < (int)m_colorRamps._pair.size() - 1; i++)
	{
		double pos1 = m_colorRamps._pair.at(i)._value;
		MSColor color1 = m_colorRamps._pair.at(i)._color;
		double pos2 = m_colorRamps._pair.at(i + 1)._value;
		MSColor color2 = m_colorRamps._pair.at(i + 1)._color;
		//这2个是一一对应的
		if (pos1 <= dValue && dValue < pos2 || dValue == pos2 && i == (int)m_colorRamps._pair.size() - 2)
		{
			int nA = (dValue - pos1)*(color2._colorA - color1._colorA) / (pos2 - pos1) + color1._colorA;
			int nR = (dValue - pos1)*(color2._colorR - color1._colorR) / (pos2 - pos1) + color1._colorR;
			int nG = (dValue - pos1)*(color2._colorG - color1._colorG) / (pos2 - pos1) + color1._colorG;
			int nB = (dValue - pos1)*(color2._colorB - color1._colorB) / (pos2 - pos1) + color1._colorB;
			return MSColor(nA, nR, nG, nB);
		}
	}
	return _MSColor(0, 0, 0, 0);
}
void	MSDiscreteRasterThematicObj::SetMinMaxValue(double dMin,double dMax)
{
	assert(dMax >= dMin);
	m_dMinValue = dMin;
	m_dMaxValue = dMax;
}
bool	MSDiscreteRasterThematicObj::GetMinMaxValue(double& dMin,double& dMax)
{
	dMin = m_dMinValue;
	dMax = m_dMaxValue;
	if(m_dMinValue == -MAXNUM_MSTHEMATIC || m_dMaxValue == -MAXNUM_MSTHEMATIC || m_dMinValue == m_dMaxValue)
		return false;
	return true;
}
MSColorRamps* MSDiscreteRasterThematicObj::GetColorRampsObj()
{
	return &m_colorRamps;
}


MSColorMapRasterThematicObj::MSColorMapRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_COLORMAP;
	m_dMinValue = m_dMaxValue = -MAXNUM_MSTHEMATIC;
	m_nBand = 1;
}
MSColorMapRasterThematicObj::MSColorMapRasterThematicObj(const MSColorMapRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_COLORMAP;

	for (int i = 0; i < srcObj.m_VectValue.size(); i++)
	{
		m_VectValue.push_back(srcObj.m_VectValue[i]);
	}
	for (int i = 0; i < srcObj.m_VectLabel.size(); i++)
	{
		m_VectLabel.push_back(srcObj.m_VectLabel[i]);
	}
	for (int i = 0; i < srcObj.m_VectColor.size(); i++)
	{
		m_VectColor.push_back(srcObj.m_VectColor[i]);
	}
	m_colorNoData = srcObj.m_colorNoData;
	m_dMinValue = srcObj.m_dMinValue;
	m_dMaxValue = srcObj.m_dMaxValue;
	m_nBand = srcObj.m_nBand;
}
MSColorMapRasterThematicObj::~MSColorMapRasterThematicObj()
{
	//沈占锋增加 2012.12.20
	ClearAllSymbolObj();
}
void	MSColorMapRasterThematicObj::SetNoDataColor( MSColor colr )
{
	m_colorNoData = colr;
}
MSColor MSColorMapRasterThematicObj::GetNoDataColor()
{
	return m_colorNoData;
}
int		MSColorMapRasterThematicObj::AddSymbolObj(double dValue,MSColor cColor,string sLabel)
{
	if (find(m_VectValue.begin(),m_VectValue.end(),dValue) != m_VectValue.end())
	{
		return -1;
	}
	m_VectValue.push_back(dValue);
	m_VectColor.push_back(cColor);
	if(sLabel == "")
	{
		m_VectLabel.push_back(_szf_format_double(dValue));
	}
	else
		m_VectLabel.push_back(sLabel);
	return 0;
}
void	MSColorMapRasterThematicObj::ClearAllSymbolObj()
{
	m_VectColor.clear();
	m_VectLabel.clear();
	m_VectValue.clear();
}
int		MSColorMapRasterThematicObj::RemoveSymbolObj( int iIndex )
{
	int nSize = (int)m_VectValue.size();
	if (iIndex < 0 || iIndex > nSize)
		return -1;

	m_VectColor.erase(m_VectColor.begin() + iIndex);
	m_VectLabel.erase(m_VectLabel.begin() + iIndex);
	m_VectValue.erase(m_VectValue.begin() + iIndex);

	return 0;
}
int		MSColorMapRasterThematicObj::GetSymbolCount()
{
	return (int)m_VectValue.size();
}
MSColor		MSColorMapRasterThematicObj::GetColor(double dValue)
{
	int nIndex = GetIndexByValue(dValue);
	if (nIndex < 0 || nIndex >= m_VectColor.size())
	{
		ASSERT(false);
		return _MSColor();
	}
	return m_VectColor.at(nIndex);
}
int	MSColorMapRasterThematicObj::GetIndexByValue(double dValue)
{
	int nIndex = -1;
	vector<double>::iterator it;
	it = find(m_VectValue.begin(), m_VectValue.end(), dValue);
	if (it != m_VectValue.end())
	{
		return it - m_VectValue.begin();
	}
	return -1;
// 	for (int i = 0; i < m_VectValue.size(); i++)
// 	{
// 		double dV = m_VectValue.at(i);
// 		if (fabs(dV - dValue) < 1e-10)
// 		{
// 			nIndex = i;
// 			break;
// 		}
// 	}
// 	return nIndex;
}
MSColor	MSColorMapRasterThematicObj::GetColor(int iIndex)
{
	int nSize = (int)m_VectValue.size();
	if (iIndex < 0 || iIndex > nSize)
		return _MSColor();

	return m_VectColor.at(iIndex);
}
string	MSColorMapRasterThematicObj::GetLabel(int iIndex)
{
	int nSize = (int)m_VectValue.size();
	if (iIndex < 0 || iIndex > nSize)
		return "";

	return m_VectLabel.at(iIndex);
}
double		MSColorMapRasterThematicObj::GetValue(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_VectValue.size())
		ASSERT_FALSE_AND_RETURN_ERROR;
	return m_VectValue.at(iIndex);
}
void	MSColorMapRasterThematicObj::SetColor(int iIndex, MSColor color)
{
	if (iIndex < 0 || iIndex >= m_VectValue.size())
		return;
	m_VectColor[iIndex] = color;
}
void	MSColorMapRasterThematicObj::SetMinMaxValue(double dMin,double dMax)
{
	assert(dMax >= dMin);
	m_dMinValue = dMin;
	m_dMaxValue = dMax;
}
bool	MSColorMapRasterThematicObj::GetMinMaxValue(double& dMin,double& dMax)
{
	dMin = m_dMinValue;
	dMax = m_dMaxValue;
	if(m_dMinValue == -MAXNUM_MSTHEMATIC || m_dMaxValue == -MAXNUM_MSTHEMATIC || m_dMinValue == m_dMaxValue)
		return false;
	return true;
}
void	MSColorMapRasterThematicObj::SetBand(int band)
{
	m_nBand = band;
}
int		MSColorMapRasterThematicObj::GetBand()
{
	return m_nBand;
}


MSUniqueValueRasterThematicObj::MSUniqueValueRasterThematicObj()
{
	m_eThematicType = MS_THEMATIC_UNIQUEVALUE;
	m_dMinValue = m_dMaxValue = -MAXNUM_MSTHEMATIC;
	m_nBand = 1;
}
MSUniqueValueRasterThematicObj::MSUniqueValueRasterThematicObj(const MSUniqueValueRasterThematicObj& srcObj)
{
	m_eThematicType = MS_THEMATIC_UNIQUEVALUE;

	for (int i = 0; i < srcObj.m_VectValue.size(); i++)
	{
		m_VectValue.push_back(srcObj.m_VectValue[i]);
	}
	for (int i = 0; i < srcObj.m_VectLabel.size(); i++)
	{
		m_VectLabel.push_back(srcObj.m_VectLabel[i]);
	}
	for (int i = 0; i < srcObj.m_VectColor.size(); i++)
	{
		m_VectColor.push_back(srcObj.m_VectColor[i]);
	}
	m_colorNoData = srcObj.m_colorNoData;
	m_dMinValue = srcObj.m_dMinValue;
	m_dMaxValue = srcObj.m_dMaxValue;
	m_nBand = srcObj.m_nBand;
}
MSUniqueValueRasterThematicObj::~MSUniqueValueRasterThematicObj()
{
	ClearAllSymbolObj();
}


