#include "stdafx.h"
#include "MSThematicStruct.h"

MSColorRamps::_MSColorRamps()
{
	_internalType = MS_COLOR_INTERNAL_DEFAULT;
}
MSColor MSColorRamps::GetColor(double dValue)
{
	int nMinPos = -MAXNUM_MSTHEMATIC,nMaxPos = MAXNUM_MSTHEMATIC;
	double dMinValue = -MAXNUM_MSTHEMATIC,dMaxValue = MAXNUM_MSTHEMATIC;
	for (int i=0;i<_pair.size();i++)
	{
		double dnValue = _pair.at(i)._value;
		int nPos = i;
		if(fabs(dnValue - dValue) < 1e-6)
			return _pair.at(nPos)._color;
		if(dnValue > dValue && dnValue < dMaxValue)
		{
			nMaxPos = i;
			dMaxValue = dnValue;
		}
		if(dnValue < dValue && dnValue > dMinValue)
		{
			nMinPos = i;
			dMinValue = dnValue;
		}
	}
	if(nMinPos == -MAXNUM_MSTHEMATIC || nMaxPos == MAXNUM_MSTHEMATIC)
		return _MSColor(0,0,0,0);
	MSColor colorMin = _pair.at(nMinPos)._color;
	MSColor colorMax = _pair.at(nMaxPos)._color;
	double valueMin = _pair.at(nMinPos)._value;
	double valueMax = _pair.at(nMaxPos)._value;
	double dIndex = (dValue-valueMin)*1.0/(valueMax-valueMin);
	unsigned char a = (unsigned char)(dIndex*(colorMax._colorA-colorMin._colorA)+colorMin._colorA);
	unsigned char r = (unsigned char)(dIndex*(colorMax._colorR-colorMin._colorR)+colorMin._colorR);
	unsigned char g = (unsigned char)(dIndex*(colorMax._colorG-colorMin._colorG)+colorMin._colorG);
	unsigned char b = (unsigned char)(dIndex*(colorMax._colorB-colorMin._colorB)+colorMin._colorB);

	return _MSColor(a,r,g,b);
};


