//////////////////////////////////////////////////////////////////////////
//
// Brief	:	
// Author	:	zhounanq
// Email	:	zhounanq@126.com
// Date		:	
// Note		:	
//
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "MSProject.h"

#include <sstream>

#include "pugixml.hpp"
#include "MSMapObj.h"

using namespace pugi;
//////////////////////////////////////////////////////////////////////////
//
// class 
//
//////////////////////////////////////////////////////////////////////////
MSProject::MSProject()
{
}


MSProject::~MSProject()
{
}


int MSProject::load_projectfile(const std::string& project_file, MSMapObj* pMapObj)
{
	if (project_file.length() < 8 || pMapObj == nullptr)return -1;

	xml_document xml_doc;
	if (false == xml_doc.load_file(project_file.c_str()))
	{
		return -1;
	}
	xml_node root_node = xml_doc.child("MHGIS");
	if (root_node.empty() == true)return -1;
	std::string version = root_node.attribute("version").as_string();
	std::string project_name = root_node.attribute("projectname").as_string();

	/// MSMap
	xml_node map_node = root_node.child("MSMap");
	if (map_node.empty() == true)return -1;
	if (0 != ReadXml(&map_node, pMapObj))
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::save_projectfile(const std::string& project_file, MSMapObj* pMapObj)
{
	if (project_file.length() < 8 || pMapObj == nullptr)return -1;

	xml_document doc;

	xml_node root_node = doc.append_child("MHGIS");
	root_node.append_attribute("version").set_value("1.0.0");
	root_node.append_attribute("projectname").set_value("mhgis-prj");

	/// MSMap
	xml_node map_node = root_node.append_child("MSMap");
	if (map_node.empty() == true)return -1;
	if (0 != WriteXml(&map_node, pMapObj))
	{

	}

	/// save and cleanup and return
	doc.save_file(project_file.c_str());

	return 0;
}


int MSProject::load_projectxml(const std::string& project_xml, MSMapObj* pMapObj)
{
	if (project_xml.length() < 8 || pMapObj == nullptr)return -1;


	/// cleanup and return
	return 0;
}


int MSProject::save_projectxml(std::string& project_xml, MSMapObj* pMapObj)
{
	if (pMapObj == nullptr)return -1;



	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMapObj* msobjPtr)
{
	if (nodePtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSMap")return -1;

	xml_attribute attr_name = nodePtr->attribute("mapName");
	if (true != attr_name.empty())
	{
		msobjPtr->SetName(attr_name.as_string());
	}

	xml_attribute attr_labelcompact = nodePtr->attribute("mapLabelCompactness");
	if (true != attr_labelcompact.empty())
	{
		msobjPtr->SetLabelCompactness(attr_labelcompact.as_int());
	}

	/// MSEnvelop_Whole & MSEnvelop_CurrentView
	xml_node envelop_node_whole = nodePtr->child("MSEnvelop");
	if (envelop_node_whole.empty() == true)return -1;
	MSEnvelopObj* pEnvelop_Whole = msobjPtr->GetEnvelopObj_Whole();
	if (pEnvelop_Whole == nullptr) return -1;
	xml_node envelop_node_currentview = nodePtr->child("MSEnvelop_CurrentView");

	MSEnvelopObj* pEnvelop_CurrentView = msobjPtr->GetEnvelopObj_CurrentView();
	if (pEnvelop_CurrentView == nullptr) return -1;
	if (0 != ReadXml(&envelop_node_whole, pEnvelop_Whole,&envelop_node_currentview, pEnvelop_CurrentView))
	{
		return -1;
	}

	/// MSSpatialRef
	xml_node spatialref_node = nodePtr->child("MSSpatialRef");
	if (spatialref_node.empty() == true)return -1;
	MSSpatialRefObj* pSpatialRef = msobjPtr->GetSpatialRefObj();
	if (pSpatialRef == nullptr) return -1;
	if (0 != ReadXml(&spatialref_node, pSpatialRef))
	{
		return -1;
	}

	/// MSLayers
	xml_node layers_node = nodePtr->child("MSLayers");
	if (layers_node.empty() == true)return -1;
	for (pugi::xml_node_iterator it = layers_node.begin(); it != layers_node.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "MSLayerGroup")
		{
			MSGroupObj* pGroup = new MSGroupObj();
			if (0 != ReadXml(&(*it), pGroup))
			{
				return -1;
			}

			if (0 != msobjPtr->AppendRootLayer(pGroup))
			{
				return -1;
			}
		}
		else if (node_name == "MSLayer")
		{
			MSLayerType layer_tpye = MSLayerType(it->attribute("layerType").as_uint());
			MSLayerObj* pLayer = new MSLayerObj();
			if (0 != ReadXml(&(*it), pLayer))
			{
				MessageBox(NULL, "某些图层读取失败！", "错误", MB_OK | MB_ICONEXCLAMATION);
				delete pLayer;
				continue;
			}

			if (0 != msobjPtr->AppendRootLayer(pLayer))
			{
				MessageBox(NULL, "某些图层添加失败！", "错误", MB_OK | MB_ICONEXCLAMATION);
				delete pLayer;
				continue;
			}
		}
		else
		{
			ASSERT_FALSE_AND_CONTINUE;
		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMapObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSMap")return -1;

	xml_attribute attr_name = nodePtr->append_attribute("mapName");
	attr_name.set_value(msobjPtr->GetName().c_str());

	xml_attribute attr_labelcompact = nodePtr->append_attribute("mapLabelCompactness");
	attr_labelcompact.set_value(msobjPtr->GetLabelCompactness());

	/// MSEnvelop_Whole
	xml_node envelop_node_whole = nodePtr->append_child("MSEnvelop");
	if (envelop_node_whole.empty() == true)return -1;
	MSEnvelopObj* pEnvelop_Whole = msobjPtr->GetEnvelopObj_Whole();
	if (pEnvelop_Whole == nullptr) return -1;
	xml_node envelop_node_currentview = nodePtr->append_child("MSEnvelop_CurrentView");
	if (envelop_node_currentview.empty() == true)return -1;
	MSEnvelopObj* pEnvelop_CurrentView = msobjPtr->GetEnvelopObj_CurrentView();
	if (pEnvelop_CurrentView == nullptr) return -1;
	if (0 != WriteXml(&envelop_node_whole, pEnvelop_Whole, &envelop_node_currentview, pEnvelop_CurrentView))
	{

	}

	/// MSSpatialRef
	xml_node spatialref_node = nodePtr->append_child("MSSpatialRef");
	if (spatialref_node.empty() == true)return -1;
	MSSpatialRefObj* pSpatialRef = msobjPtr->GetSpatialRefObj();
	if (pSpatialRef == nullptr) return -1;
	if (0 != WriteXml(&spatialref_node, pSpatialRef))
	{

	}

	/// MSLayers
	xml_node layers_node = nodePtr->append_child("MSLayers");
	if (layers_node.empty() == true)return -1;
	int rootlayer_cnt = msobjPtr->GetRootLayerCount();
	for (int i = 0; i < rootlayer_cnt; ++i)
	{
		MSLayerObj* pLayer = msobjPtr->GetRootLayer(i);
		if (pLayer == nullptr)continue;

		MSDataSourceObj* pDataSourceObj = pLayer->GetDataSrcObj();
		MSDataSourceType dsType = pDataSourceObj->GetDataSrcType();
		if (dsType == MH_SOURCE_MEMORYFILE)
			continue;

		MSLayerType layer_tpye = pLayer->GetLayerType();
		// LayerGroup
		if (layer_tpye == MS_LAYER_GROUP)
		{
			xml_node layer_node = layers_node.append_child("MSLayerGroup");
			if (layer_node.empty() == true)continue;
			if (0 != WriteXml(&layer_node, (MSGroupObj*)pLayer))
			{

			}
		}
		// valid layer type
		else if (layer_tpye == MS_LAYER_RASTER || layer_tpye == MS_LAYER_LINE ||
			layer_tpye == MS_LAYER_POINT || layer_tpye == MS_LAYER_POLYGON)
		{
			xml_node layer_node = layers_node.append_child("MSLayer");
			if (layer_node.empty() == true)continue;
			if (0 != WriteXml(&layer_node, pLayer))
			{

			}
		}
		// invalid layer type
		else
		{
			continue;
		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSEnvelopObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSEnvelop")return -1;

	xml_attribute attr_minx = nodePtr->attribute("minX");
	msobjPtr->SetMinx(attr_minx.as_double());

	xml_attribute attr_miny = nodePtr->attribute("minY");
	msobjPtr->SetMiny(attr_miny.as_double());

	xml_attribute attr_maxx = nodePtr->attribute("maxX");
	msobjPtr->SetMaxx(attr_maxx.as_double());

	xml_attribute attr_maxy = nodePtr->attribute("maxY");
	msobjPtr->SetMaxy(attr_maxy.as_double());

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSEnvelopObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSEnvelop")return -1;

	xml_attribute attr_minx = nodePtr->append_attribute("minX");
	attr_minx.set_value(msobjPtr->GetMinx());

	xml_attribute attr_miny = nodePtr->append_attribute("minY");
	attr_miny.set_value(msobjPtr->GetMiny());

	xml_attribute attr_maxx = nodePtr->append_attribute("maxX");
	attr_maxx.set_value(msobjPtr->GetMaxx());

	xml_attribute attr_maxy = nodePtr->append_attribute("maxY");
	attr_maxy.set_value(msobjPtr->GetMaxy());

	return 0;
}

int MSProject::ReadXml(pugi::xml_node* nodePtr_Whole, MSEnvelopObj* msobjPtr_Whole, pugi::xml_node* nodePtr_CurrentView, MSEnvelopObj* msobjPtr_CurrentView)
{
	if (nodePtr_Whole == 0 || msobjPtr_Whole == 0 || nodePtr_CurrentView == 0 || msobjPtr_CurrentView == 0)return -1;

	string node_name = nodePtr_Whole->name();
	if (node_name != "MSEnvelop")return -1;

	xml_attribute attr_minx = nodePtr_Whole->attribute("minX");
	msobjPtr_Whole->SetMinx(attr_minx.as_double());

	xml_attribute attr_miny = nodePtr_Whole->attribute("minY");
	msobjPtr_Whole->SetMiny(attr_miny.as_double());

	xml_attribute attr_maxx = nodePtr_Whole->attribute("maxX");
	msobjPtr_Whole->SetMaxx(attr_maxx.as_double());

	xml_attribute attr_maxy = nodePtr_Whole->attribute("maxY");
	msobjPtr_Whole->SetMaxy(attr_maxy.as_double());

	node_name = nodePtr_CurrentView->name();
	if (node_name == "MSEnvelop_CurrentView")
	{
		attr_minx = nodePtr_CurrentView->attribute("minX");
		msobjPtr_CurrentView->SetMinx(attr_minx.as_double());

		attr_miny = nodePtr_CurrentView->attribute("minY");
		msobjPtr_CurrentView->SetMiny(attr_miny.as_double());

		attr_maxx = nodePtr_CurrentView->attribute("maxX");
		msobjPtr_CurrentView->SetMaxx(attr_maxx.as_double());

		attr_maxy = nodePtr_CurrentView->attribute("maxY");
		msobjPtr_CurrentView->SetMaxy(attr_maxy.as_double());
	}


	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr_Whole, MSEnvelopObj* msobjPtr_Whole, pugi::xml_node* nodePtr_CurrentView, MSEnvelopObj* msobjPtr_CurrentView)
{
	if (nodePtr_Whole == 0 || msobjPtr_Whole == 0 || nodePtr_CurrentView == 0 || msobjPtr_CurrentView == 0)return -1;

	string node_name = nodePtr_Whole->name();
	if (node_name != "MSEnvelop")return -1;

	xml_attribute attr_minx = nodePtr_Whole->append_attribute("minX");
	attr_minx.set_value(msobjPtr_Whole->GetMinx());

	xml_attribute attr_miny = nodePtr_Whole->append_attribute("minY");
	attr_miny.set_value(msobjPtr_Whole->GetMiny());

	xml_attribute attr_maxx = nodePtr_Whole->append_attribute("maxX");
	attr_maxx.set_value(msobjPtr_Whole->GetMaxx());

	xml_attribute attr_maxy = nodePtr_Whole->append_attribute("maxY");
	attr_maxy.set_value(msobjPtr_Whole->GetMaxy());


	node_name = nodePtr_CurrentView->name();
	if (node_name != "MSEnvelop_CurrentView")return -1;

	attr_minx = nodePtr_CurrentView->append_attribute("minX");
	attr_minx.set_value(msobjPtr_CurrentView->GetMinx());

	attr_miny = nodePtr_CurrentView->append_attribute("minY");
	attr_miny.set_value(msobjPtr_CurrentView->GetMiny());

	attr_maxx = nodePtr_CurrentView->append_attribute("maxX");
	attr_maxx.set_value(msobjPtr_CurrentView->GetMaxx());

	attr_maxy = nodePtr_CurrentView->append_attribute("maxY");
	attr_maxy.set_value(msobjPtr_CurrentView->GetMaxy());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSSpatialRefObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSSpatialRef")return -1;

	xml_attribute attr_minx = nodePtr->attribute("epsgCode");
	msobjPtr->SetEpsg(attr_minx.as_int());

	xml_attribute attr_miny = nodePtr->attribute("wktStr");
	msobjPtr->SetWkt(attr_miny.as_string());

	xml_attribute attr_maxx = nodePtr->attribute("unit");
	msobjPtr->SetUnit(MSUnitType(attr_maxx.as_uint()));

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSSpatialRefObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSSpatialRef")return -1;

	xml_attribute attr_minx = nodePtr->append_attribute("epsgCode");
	attr_minx.set_value(msobjPtr->GetEspg());

	xml_attribute attr_miny = nodePtr->append_attribute("wktStr");
	attr_miny.set_value(msobjPtr->GetWkt().c_str());

	xml_attribute attr_maxx = nodePtr->append_attribute("unit");
	attr_maxx.set_value(msobjPtr->GetUnit());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSGroupObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSLayerGroup")return -1;

	/// from MSLayer
	xml_attribute attr_name = nodePtr->attribute("layerName");
	if (true == attr_name.empty()) return -1;
	msobjPtr->SetName(attr_name.as_string());

	xml_attribute attr_desc = nodePtr->attribute("layerDesc");
	if (true == attr_desc.empty()) return -1;
	msobjPtr->SetLayerDescription(attr_desc.as_string());

	xml_attribute attr_isvis = nodePtr->attribute("isVisible");
	if (true == attr_isvis.empty()) return -1;
	msobjPtr->SetVisible(attr_isvis.as_bool());

	/// Group or Layer
	for (pugi::xml_node_iterator it = nodePtr->begin(); it != nodePtr->end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "MSLayerGroup")
		{
			MSGroupObj* pGroup = new MSGroupObj();
			if (0 != msobjPtr->AddLayer(pGroup))
			{

			}

			if (0 != ReadXml(&(*it), pGroup))
			{

			}
		}
		else if (node_name == "MSLayer")
		{
			MSLayerType layer_tpye = MSLayerType(it->attribute("layerType").as_uint());
			MSLayerObj* pLayer = new MSLayerObj();
			if (0 != msobjPtr->AddLayer(pLayer))
			{

			}

			if (0 != ReadXml(&(*it), pLayer))
			{

			}
		}
		else
		{
			continue;
		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSGroupObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSLayerGroup")return -1;

	/// from MSLayer
	xml_attribute attr_name = nodePtr->append_attribute("layerName");
	attr_name.set_value(msobjPtr->GetName().c_str());

	xml_attribute attr_desc = nodePtr->append_attribute("layerDesc");
	attr_desc.set_value(msobjPtr->GetLayerDescription().c_str());

	xml_attribute attr_isvis = nodePtr->append_attribute("isVisible");
	attr_isvis.set_value(msobjPtr->GetVisible());


	int layer_cnt = msobjPtr->GetLayerCount();
	for (int lit = 0; lit < layer_cnt; ++lit)
	{
		MSLayerObj* pSubLayer = msobjPtr->GetLayer(lit);
		if (pSubLayer == 0)continue;
		MSLayerType layer_tpye = pSubLayer->GetLayerType();

		// LayerGroup
		if (layer_tpye == MS_LAYER_GROUP)
		{
			xml_node sublayer_node = nodePtr->append_child("MSLayerGroup");
			if (sublayer_node.empty() == true)continue;
			if (0 != WriteXml(&sublayer_node, (MSGroupObj*)pSubLayer))
			{

			}
		}
		// valid layer type
		else if (layer_tpye == MS_LAYER_RASTER || layer_tpye == MS_LAYER_LINE ||
			layer_tpye == MS_LAYER_POINT || layer_tpye == MS_LAYER_POLYGON)
		{
			xml_node sublayer_node = nodePtr->append_child("MSLayer");
			if (sublayer_node.empty() == true)continue;
			if (0 != WriteXml(&sublayer_node, pSubLayer))
			{

			}
		}
		// invalid layer type
		else
		{

		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSLayerObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSLayer")return -1;

	/// layer attributations
	xml_attribute attr_type = nodePtr->attribute("layerType");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetLayerType((MSLayerType)(attr_type.as_uint()));

	xml_attribute attr_name = nodePtr->attribute("layerName");
	if (true == attr_name.empty()) return -1;
	msobjPtr->SetName(attr_name.as_string());

	xml_attribute attr_desc = nodePtr->attribute("layerDesc");
	if (true == attr_desc.empty()) return -1;
	msobjPtr->SetLayerDescription(attr_desc.as_string());

	xml_attribute attr_isvis = nodePtr->attribute("isVisible");
	if (true == attr_isvis.empty()) return -1;
	msobjPtr->SetVisible(attr_isvis.as_bool());

	xml_attribute attr_uuid = nodePtr->attribute("layerUuid");
	if (true == attr_uuid.empty()) return -1;
	msobjPtr->SetUuid(attr_uuid.as_string());

	xml_attribute attr_transper = nodePtr->attribute("layerTransper");
	if (true == attr_transper.empty()) return -1;
	msobjPtr->SetTransper(attr_transper.as_double());

	xml_attribute attr_scalelow = nodePtr->attribute("scaleLower");
	if (true == attr_scalelow.empty()) return -1;
	msobjPtr->SetScaleLower(attr_scalelow.as_double());

	xml_attribute attr_scaleupp = nodePtr->attribute("scaleUpper");
	if (true == attr_scaleupp.empty()) return -1;
	msobjPtr->SetScaleUpper(attr_scaleupp.as_double());


	/// MSEnvelop_Whole
	xml_node envelop_node = nodePtr->child("MSEnvelop");
	if (envelop_node.empty() == true)return -1;
	MSEnvelopObj* pEnvelop = msobjPtr->GetEnvelopObj();
	if (pEnvelop == nullptr) return -1;
	if (0 != ReadXml(&envelop_node, pEnvelop))
	{
		return -1;
	}

	/// MSDataSource
	xml_node datasource_node = nodePtr->child("MSDataSource");
	if (datasource_node.empty() == true)return -1;
	MSDataSourceObj* pDS = msobjPtr->GetDataSrcObj();
	if (pDS == nullptr) return -1;
	if (0 != ReadXml(&datasource_node, pDS))
	{
		return -1;
	}

	/// MSThematic
	xml_node thematic_node = nodePtr->child("MSThematic");
	if (thematic_node.empty() == true)return -1;

	MSThematicType thematic_type = MSThematicType(thematic_node.attribute("type").as_uint());
	if (thematic_type == MS_THEMATIC_ANNOTATION)
	{
		MSAnnotationThematicObj* pThematic = new MSAnnotationThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_SIMPLE)
	{
		MSSimpleThematicObj* pThematic = new MSSimpleThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_CATEGORY)
	{
		MSCategoryThematicObj* pThematic = new MSCategoryThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_GRADUATE)
	{
		MSGraduateThematicObj* pThematic = new MSGraduateThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_CHART)
	{
		MSChartThematicObj* pThematic = new MSChartThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_COLOR)
	{
		MSRGBRasterThematicObj* pThematic = new MSRGBRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_STRETCH)
	{
		MSStretchRasterThematicObj* pThematic = new MSStretchRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_DISCRETE)
	{
		MSDiscreteRasterThematicObj* pThematic = new MSDiscreteRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_COLORMAP)
	{
		MSColorMapRasterThematicObj* pThematic = new MSColorMapRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_UNIQUEVALUE)
	{
		MSUniqueValueRasterThematicObj* pThematic = new MSUniqueValueRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else if (thematic_type == MS_THEMATIC_CLASSIFICATION)
	{
		MSClassificationRasterThematicObj* pThematic = new MSClassificationRasterThematicObj();
		if (pThematic == nullptr) return -1;
		msobjPtr->SetThematicObj(pThematic);
		if (0 != ReadXml(&thematic_node, pThematic))
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSLayerObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSLayer")return -1;

	/// layer attributations
	xml_attribute attr_type = nodePtr->append_attribute("layerType");
	attr_type.set_value(msobjPtr->GetLayerType());

	xml_attribute attr_name = nodePtr->append_attribute("layerName");
	attr_name.set_value(msobjPtr->GetName().c_str());

	xml_attribute attr_desc = nodePtr->append_attribute("layerDesc");
	attr_desc.set_value(msobjPtr->GetLayerDescription().c_str());

	xml_attribute attr_isvis = nodePtr->append_attribute("isVisible");
	attr_isvis.set_value(msobjPtr->GetVisible());

	xml_attribute attr_uuid = nodePtr->append_attribute("layerUuid");
	attr_uuid.set_value(msobjPtr->GetUuid().c_str());

	xml_attribute attr_transper = nodePtr->append_attribute("layerTransper");
	attr_transper.set_value(msobjPtr->GetTransper());

	xml_attribute attr_scalelow = nodePtr->append_attribute("scaleLower");
	attr_scalelow.set_value(msobjPtr->GetScaleLower());

	xml_attribute attr_scaleupp = nodePtr->append_attribute("scaleUpper");
	attr_scaleupp.set_value(msobjPtr->GetScaleUpper());


	/// MSEnvelop_Whole
	xml_node envelop_node = nodePtr->append_child("MSEnvelop");
	if (envelop_node.empty() == true)return -1;
	MSEnvelopObj* pEnvelop = msobjPtr->GetEnvelopObj();
	if (pEnvelop == nullptr) return -1;
	if (0 != WriteXml(&envelop_node, pEnvelop))
	{

	}

	/// MSDataSource
	xml_node datasource_node = nodePtr->append_child("MSDataSource");
	if (datasource_node.empty() == true)return -1;
	MSDataSourceObj* pDS = msobjPtr->GetDataSrcObj();
	if (pDS == nullptr) return -1;
	if (0 != WriteXml(&datasource_node, pDS))
	{

	}

	/// MSThematic
	xml_node thematic_node = nodePtr->append_child("MSThematic");
	if (thematic_node.empty() == true)return -1;
	MSThematicObj* pThematic = msobjPtr->GetThematicObj();
	if (pThematic == nullptr) return -1;
	MSThematicType thematic_type = pThematic->GetThematicType();
	if (thematic_type == MS_THEMATIC_ANNOTATION)
	{
		if (0 != WriteXml(&thematic_node, (MSAnnotationThematicObj*)pThematic))
		{

		}
	}
	if (thematic_type == MS_THEMATIC_LABEL)
	{
		if (0 != WriteXml(&thematic_node, (MSLabelThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_SIMPLE)
	{
		if (0 != WriteXml(&thematic_node, (MSSimpleThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_CATEGORY)
	{
		if (0 != WriteXml(&thematic_node, (MSCategoryThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_GRADUATE)
	{
		if (0 != WriteXml(&thematic_node, (MSGraduateThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_CHART)
	{
		if (0 != WriteXml(&thematic_node, (MSChartThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_COLOR)
	{
		if (0 != WriteXml(&thematic_node, (MSRGBRasterThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_STRETCH)
	{
		if (0 != WriteXml(&thematic_node, (MSStretchRasterThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_DISCRETE)
	{
		if (0 != WriteXml(&thematic_node, (MSDiscreteRasterThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_COLORMAP)
	{
		if (0 != WriteXml(&thematic_node, (MSColorMapRasterThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_UNIQUEVALUE)
	{
		if (0 != WriteXml(&thematic_node, (MSUniqueValueRasterThematicObj*)pThematic))
		{

		}
	}
	else if (thematic_type == MS_THEMATIC_CLASSIFICATION)
	{
		if (0 != WriteXml(&thematic_node, (MSClassificationRasterThematicObj*)pThematic))
		{

		}
	}
	else
	{

	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSDataSourceObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSDataSource")return -1;

	xml_attribute attr_minx = nodePtr->attribute("dataPath");
	if (true == attr_minx.empty()) return -1;
	msobjPtr->SetDataSrc(attr_minx.as_string());

	xml_attribute attr_miny = nodePtr->attribute("dataType");
	if (true == attr_miny.empty()) return -1;
	msobjPtr->SetDataSrcType(MSDataSourceType(attr_miny.as_uint()));

	xml_attribute attr_maxx = nodePtr->attribute("datasetID");
	if (true == attr_maxx.empty()) return -1;
	msobjPtr->SetDataSetID(attr_maxx.as_string());

	xml_attribute attr_maxy = nodePtr->attribute("dataDescription");
	if (true == attr_maxy.empty()) return -1;
	msobjPtr->SetDataDescription(attr_maxy.as_string());

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSDataSourceObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSDataSource")return -1;

	xml_attribute attr_minx = nodePtr->append_attribute("dataPath");
	attr_minx.set_value(msobjPtr->GetDataSrc().c_str());

	xml_attribute attr_miny = nodePtr->append_attribute("dataType");
	attr_miny.set_value(msobjPtr->GetDataSrcType());

	xml_attribute attr_maxx = nodePtr->append_attribute("datasetID");
	attr_maxx.set_value(msobjPtr->GetDataSetID().c_str());

	xml_attribute attr_maxy = nodePtr->append_attribute("dataDescription");
	attr_maxy.set_value(msobjPtr->GetDataDescription().c_str());

	return 0;
}


//////////////////////////////////////////////////////////////////////////


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSAnnotationThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_size = nodePtr->attribute("size");
	if (true == attr_size.empty()) return -1;
	msobjPtr->SetSize(attr_size.as_double());

	xml_attribute attr_interval = nodePtr->attribute("interval");
	if (true == attr_interval.empty()) return -1;
	msobjPtr->SetInterval(attr_interval.as_double());

	xml_attribute attr_halign = nodePtr->attribute("halign");
	if (true == attr_halign.empty()) return -1;
	msobjPtr->SetHorizAllig(MSHorizontalAligmentType(attr_halign.as_uint()));

	xml_attribute attr_valign = nodePtr->attribute("valign");
	if (true == attr_valign.empty()) return -1;
	msobjPtr->SetVertiAllig(MSVerticalAligmentType(attr_valign.as_uint()));

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	xml_attribute attr_annotype = nodePtr->attribute("annotype");
	if (true == attr_annotype.empty()) return -1;
	msobjPtr->SetAnnoType(MSANNOTYPE(attr_annotype.as_uint()));

	xml_attribute attr_bshowscale = nodePtr->attribute("bshowscale");
	if (true == attr_bshowscale.empty()) return -1;
	msobjPtr->SetShowWithScale(attr_bshowscale.as_bool());

	xml_attribute attr_ballowovelap = nodePtr->attribute("ballowovelap");
	if (true == attr_ballowovelap.empty()) return -1;
	msobjPtr->SetAllowOverlapped(attr_ballowovelap.as_bool());

	xml_attribute attr_ballowremove = nodePtr->attribute("ballowremove");
	if (true == attr_ballowremove.empty()) return -1;
	msobjPtr->SetAllowRemoveDuplicated(attr_ballowremove.as_bool());

	xml_attribute attr_field = nodePtr->attribute("field");
	if (true == attr_field.empty()) return -1;
	msobjPtr->SetField(attr_field.as_string());

	xml_node node_font = nodePtr->child("font");
	msobjPtr->SetFontName(node_font.attribute("name").as_string());
	msobjPtr->SetFontItalic(node_font.attribute("italic").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("blod").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("underline").as_bool());

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSAnnotationThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_size = nodePtr->append_attribute("size");
	attr_size.set_value(msobjPtr->GetSize());

	xml_attribute attr_interval = nodePtr->append_attribute("interval");
	attr_interval.set_value(msobjPtr->GetInterval());

	xml_attribute attr_halign = nodePtr->append_attribute("halign");
	attr_halign.set_value(msobjPtr->GetHorizAllig());

	xml_attribute attr_valign = nodePtr->append_attribute("valign");
	attr_valign.set_value(msobjPtr->GetVertiAllig());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	xml_attribute attr_annotype = nodePtr->append_attribute("annotype");
	attr_annotype.set_value(msobjPtr->GetAnnoType());

	xml_attribute attr_bshowscale = nodePtr->append_attribute("bshowscale");
	attr_bshowscale.set_value(msobjPtr->GetShowWithScale());

	xml_attribute attr_ballowovelap = nodePtr->append_attribute("ballowovelap");
	attr_ballowovelap.set_value(msobjPtr->GetAllowOvelapped());

	xml_attribute attr_ballowremove = nodePtr->append_attribute("ballowremove");
	attr_ballowremove.set_value(msobjPtr->GetAllowRemoveDuplicated());

	xml_attribute attr_field = nodePtr->append_attribute("field");
	attr_field.set_value(msobjPtr->GetField());

	xml_node node_font = nodePtr->append_child("font");
	node_font.append_attribute("name").set_value(msobjPtr->GetFontName());
	node_font.append_attribute("italic").set_value(msobjPtr->GetFontItalic());
	node_font.append_attribute("blod").set_value(msobjPtr->GetFontBold());
	node_font.append_attribute("underline").set_value(msobjPtr->GetFontUnderline());
	//node_font.append_attribute("strikeout").set_value(msobjPtr->GetFont????());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSLabelThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "label")return -1;

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_size = nodePtr->attribute("size");
	if (true == attr_size.empty()) return -1;
	msobjPtr->SetSize(attr_size.as_double());

	xml_attribute attr_interval = nodePtr->attribute("interval");
	if (true == attr_interval.empty()) return -1;
	msobjPtr->SetInterval(attr_interval.as_double());

	xml_attribute attr_halign = nodePtr->attribute("halign");
	if (true == attr_halign.empty()) return -1;
	msobjPtr->SetHorizAllig(MSHorizontalAligmentType(attr_halign.as_uint()));

	xml_attribute attr_valign = nodePtr->attribute("valign");
	if (true == attr_valign.empty()) return -1;
	msobjPtr->SetVertiAllig(MSVerticalAligmentType(attr_valign.as_uint()));

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	xml_attribute attr_annotype = nodePtr->attribute("annotype");
	if (true == attr_annotype.empty()) return -1;
	msobjPtr->SetAnnoType(MSANNOTYPE(attr_annotype.as_uint()));

	xml_attribute attr_bshowscale = nodePtr->attribute("bshowscale");
	if (true == attr_bshowscale.empty()) return -1;
	msobjPtr->SetShowWithScale(attr_bshowscale.as_bool());

	xml_attribute attr_ballowovelap = nodePtr->attribute("ballowovelap");
	if (true == attr_ballowovelap.empty()) return -1;
	msobjPtr->SetAllowOverlapped(attr_ballowovelap.as_bool());

	xml_attribute attr_ballowremove = nodePtr->attribute("ballowremove");
	if (true == attr_ballowremove.empty()) return -1;
	msobjPtr->SetAllowRemoveDuplicated(attr_ballowremove.as_bool());

	xml_attribute attr_field = nodePtr->attribute("field");
	if (true == attr_field.empty()) return -1;
	msobjPtr->SetField(attr_field.as_string());

	xml_attribute attr_edgecolor = nodePtr->attribute("edgecolor");
	if (true == attr_edgecolor.empty()) return -1;
	MSColor ms_edgecolor; ms_edgecolor.rgba = attr_edgecolor.as_uint();
	msobjPtr->SetEdgeColor(ms_edgecolor);

	xml_attribute attr_busebigedge = nodePtr->attribute("busebigedge");
	if (true == attr_busebigedge.empty()) return -1;
	msobjPtr->SetUseBigEdge(attr_busebigedge.as_bool());

	xml_attribute attr_precision = nodePtr->attribute("precision");
	if (true == attr_precision.empty()) return -1;
	msobjPtr->SetPrecision(attr_precision.as_int());

	/// vector<MSPointLabelPosition>
	xml_attribute attr_positions = nodePtr->attribute("positions");
	if (true == attr_positions.empty()) return -1;
	std::string str_positions = attr_positions.as_string();

	std::vector<MSPointLabelPosition> vct_poss;
	std::stringstream sss_positions;
	sss_positions << str_positions;
	int valu = 0; char split = 0;
	while (sss_positions >> valu)
	{
		vct_poss.push_back(MSPointLabelPosition(valu)); sss_positions >> split;
	}
	msobjPtr->SetLabelPosition(vct_poss);

	/// font
	xml_node node_font = nodePtr->child("font");
	msobjPtr->SetFontName(node_font.attribute("name").as_string());
	msobjPtr->SetFontItalic(node_font.attribute("italic").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("blod").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("underline").as_bool());

	/// symbol
	xml_node node_symbol = nodePtr->child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(node_symbol.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSymbol = new MSSimpleFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		MSGraduateFillSymbolObj* pSymbol = new MSGraduateFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		MSMarkerFillSymbolObj* pSymbol = new MSMarkerFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		MSPictureFillSymbolObj* pSymbol = new MSPictureFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		MSHatchFillSymbolObj* pSymbol = new MSHatchFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		MSLineFillSymbolObj* pSymbol = new MSLineFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{
		MSMultiLayerFillSymbolObj* pSymbol = new MSMultiLayerFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetFillPolygonSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSLabelThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "label")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_size = nodePtr->append_attribute("size");
	attr_size.set_value(msobjPtr->GetSize());

	xml_attribute attr_interval = nodePtr->append_attribute("interval");
	attr_interval.set_value(msobjPtr->GetInterval());

	xml_attribute attr_halign = nodePtr->append_attribute("halign");
	attr_halign.set_value(msobjPtr->GetHorizAllig());

	xml_attribute attr_valign = nodePtr->append_attribute("valign");
	attr_valign.set_value(msobjPtr->GetVertiAllig());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	xml_attribute attr_annotype = nodePtr->append_attribute("annotype");
	attr_annotype.set_value(msobjPtr->GetAnnoType());

	xml_attribute attr_bshowscale = nodePtr->append_attribute("bshowscale");
	attr_bshowscale.set_value(msobjPtr->GetShowWithScale());

	xml_attribute attr_ballowovelap = nodePtr->append_attribute("ballowovelap");
	attr_ballowovelap.set_value(msobjPtr->GetAllowOvelapped());

	xml_attribute attr_ballowremove = nodePtr->append_attribute("ballowremove");
	attr_ballowremove.set_value(msobjPtr->GetAllowRemoveDuplicated());

	xml_attribute attr_field = nodePtr->append_attribute("field");
	attr_field.set_value(msobjPtr->GetField());

	xml_attribute attr_edgecolor = nodePtr->append_attribute("edgecolor");
	attr_edgecolor.set_value(msobjPtr->GetEdgeColor().rgba);

	xml_attribute attr_busebigedge = nodePtr->append_attribute("busebigedge");
	attr_busebigedge.set_value(msobjPtr->GetUseBigEdge());

	xml_attribute attr_precision = nodePtr->append_attribute("precision");
	attr_precision.set_value(msobjPtr->GetPrecision());

	/// vector<MSPointLabelPosition>
	std::stringstream sss_pos;
	vector<MSPointLabelPosition> vct_pos = msobjPtr->GetLablePosition();
	for (int i = 0; i < vct_pos.size(); ++i)
	{
		sss_pos << (vct_pos[i]) << '|';
	}
	std::string str_pos = sss_pos.str();
	xml_attribute attr_positions = nodePtr->append_attribute("positions");
	attr_positions.set_value(str_pos.c_str());

	/// font
	xml_node node_font = nodePtr->append_child("font");
	node_font.append_attribute("name").set_value(msobjPtr->GetFontName());
	node_font.append_attribute("italic").set_value(msobjPtr->GetFontItalic());
	node_font.append_attribute("blod").set_value(msobjPtr->GetFontBold());
	node_font.append_attribute("underline").set_value(msobjPtr->GetFontUnderline());

	/// symbol
	xml_node node_symbol = nodePtr->append_child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolObj* pSymbol = msobjPtr->GetFillPolygonSymbol();
	if (pSymbol == nullptr) return -1;
	MSSymbolType symbol_type = pSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSGraduateFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSHatchFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSLineFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMultiLayerFillSymbolObj*)pSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSSimpleThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	/// label
	xml_attribute attr_buselabel = nodePtr->attribute("buselabel");
	if (true == attr_buselabel.empty()) return -1;
	msobjPtr->SetUseLabel(attr_buselabel.as_bool());
	if (true == attr_buselabel.as_bool())
	{
		xml_node label_node = nodePtr->child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		MSLabelThematicObj* pLabelThematicObj = msobjPtr->GetLabelObj();
		if (0 != ReadXml(&label_node, (MSLabelThematicObj*)pLabelThematicObj))
		{

		}
	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->attribute("busefilter");
	if (true == attr_busefilter.empty()) return -1;
	msobjPtr->SetUseFilter(attr_busefilter.as_bool());
	if (true == attr_busefilter.as_bool())
	{
		xml_node filter_node = nodePtr->child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol
	xml_node node_symbol = nodePtr->child("symbol");
	if (node_symbol.empty() == true)return -1;

	MSSymbolType symbol_type = MSSymbolType(node_symbol.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pSymbol = new MSSimpleMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbolObj(pSymbol);
		if (0 != ReadXml(&node_symbol, (MSSimpleMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pSymbol = new MSSimpleLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbolObj(pSymbol);
		if (0 != ReadXml(&node_symbol, (MSSimpleLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSymbol = new MSSimpleFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbolObj(pSymbol);
		if (0 != ReadXml(&node_symbol, (MSSimpleFillSymbolObj*)pSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSSimpleThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	/// label
	xml_attribute attr_buselabel = nodePtr->append_attribute("buselabel");
	attr_buselabel.set_value(msobjPtr->GetUseLabel());
	if (true == msobjPtr->GetUseLabel())
	{
		xml_node label_node = nodePtr->append_child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		if (0 != WriteXml(&label_node, (MSLabelThematicObj*)msobjPtr->GetLabelObj()))
		{

		}

	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->append_attribute("busefilter");
	attr_busefilter.set_value(msobjPtr->GetUseFilter());
	if (true == msobjPtr->GetUseFilter())
	{
		xml_node filter_node = nodePtr->append_child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol
	xml_node node_symbol = nodePtr->append_child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolObj* pSymbol = msobjPtr->GetSymbolObj();
	if (pSymbol == nullptr) return -1;
	MSSymbolType symbol_type = pSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleFillSymbolObj*)pSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSCategoryThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	/// label
	xml_attribute attr_buselabel = nodePtr->attribute("buselabel");
	if (true == attr_buselabel.empty()) return -1;
	msobjPtr->SetUseLabel(attr_buselabel.as_bool());
	if (true == attr_buselabel.as_bool())
	{
		xml_node label_node = nodePtr->child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		MSLabelThematicObj* pLabelThematicObj = msobjPtr->GetLabelObj();
		if (0 != ReadXml(&label_node, (MSLabelThematicObj*)pLabelThematicObj))
		{

		}
	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->attribute("busefilter");
	if (true == attr_busefilter.empty()) return -1;
	msobjPtr->SetUseFilter(attr_busefilter.as_bool());
	if (true == attr_busefilter.as_bool())
	{
		xml_node filter_node = nodePtr->child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol
	xml_attribute attr_field1 = nodePtr->attribute("field1");
	if (true == attr_field1.empty()) return -1;
	msobjPtr->SetField1(attr_field1.as_string());

	xml_attribute attr_field2 = nodePtr->attribute("field2");
	if (true == attr_field2.empty()) return -1;
	msobjPtr->SetField2(attr_field2.as_string());

	xml_attribute attr_mincolor = nodePtr->attribute("mincolor");
	if (true == attr_mincolor.empty()) return -1;
	MSColor ms_mincolor; ms_mincolor.rgba = attr_mincolor.as_uint();
	msobjPtr->SetColorRampMin(ms_mincolor);

	xml_attribute attr_maxcolor = nodePtr->attribute("maxcolor");
	if (true == attr_maxcolor.empty()) return -1;
	MSColor ms_maxcolor; ms_maxcolor.rgba = attr_maxcolor.as_uint();
	msobjPtr->SetColorRampMax(ms_maxcolor);

	xml_attribute attr_colornumber = nodePtr->attribute("colornumber");
	if (true == attr_colornumber.empty()) return -1;
	msobjPtr->SetColorRampNum(attr_colornumber.as_int());

	xml_attribute attr_usecolornumber = nodePtr->attribute("usecolornumber");
	if (true == attr_usecolornumber.empty()) return -1;
	msobjPtr->SetUseColorRampNum(attr_usecolornumber.as_bool());

	xml_attribute attr_randomcolor = nodePtr->attribute("randomcolor");
	if (true == attr_randomcolor.empty()) return -1;
	msobjPtr->SetUseRandomColor(attr_randomcolor.as_bool());

	/// symbol
	xml_node node_valuesymbols = nodePtr->child("valuesymbols");
	if (node_valuesymbols.empty() == true)return -1;

	for (pugi::xml_node_iterator it = node_valuesymbols.begin(); it != node_valuesymbols.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "valuesymbol")
		{
			MSValueSymbolObj* pSymbol = new MSValueSymbolObj();
			if (pSymbol == nullptr) continue;
			if (0 != msobjPtr->InsertSymbolObj(pSymbol))
			{

			}
			if (0 != ReadXml(&(*it), pSymbol))
			{

			}
		}
		else
		{
			continue;
		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSCategoryThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	/// label
	xml_attribute attr_buselabel = nodePtr->append_attribute("buselabel");
	attr_buselabel.set_value(msobjPtr->GetUseLabel());
	if (true == msobjPtr->GetUseLabel())
	{
		xml_node label_node = nodePtr->append_child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		if (0 != WriteXml(&label_node, (MSLabelThematicObj*)msobjPtr->GetLabelObj()))
		{

		}

	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->append_attribute("busefilter");
	attr_busefilter.set_value(msobjPtr->GetUseFilter());
	if (true == msobjPtr->GetUseFilter())
	{
		xml_node filter_node = nodePtr->append_child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol

	xml_attribute attr_field1 = nodePtr->append_attribute("field1");
	attr_field1.set_value(msobjPtr->GetField1());

	xml_attribute attr_field2 = nodePtr->append_attribute("field2");
	attr_field2.set_value(msobjPtr->GetField2());

	xml_attribute attr_colormin = nodePtr->append_attribute("mincolor");
	attr_colormin.set_value(msobjPtr->GetColorRampMin().rgba);

	xml_attribute attr_colormax = nodePtr->append_attribute("maxcolor");
	attr_colormax.set_value(msobjPtr->GetColorRampMax().rgba);

	xml_attribute attr_colornumber = nodePtr->append_attribute("colornumber");
	attr_colornumber.set_value(msobjPtr->GetColorRampNum());

	xml_attribute attr_usecolornumber = nodePtr->append_attribute("usecolornumber");
	attr_usecolornumber.set_value(msobjPtr->GetUseColorRampNum());

	xml_attribute attr_randomcolor = nodePtr->append_attribute("randomcolor");
	attr_randomcolor.set_value(msobjPtr->GetUseRandomColor());

	xml_node node_valuesymbols = nodePtr->append_child("valuesymbols");
	int valsym_cnt = msobjPtr->GetSymbolCount();
	for (int i = 0; i < valsym_cnt; ++i)
	{
		xml_node node_symbol = node_valuesymbols.append_child("valuesymbol");
		if (node_symbol.empty() == true)return -1;
		MSValueSymbolObj* pSymbol = msobjPtr->GetValueSymbol(i);
		if (pSymbol == nullptr)continue;
		if (0 != WriteXml(&node_symbol, pSymbol))
		{

		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSGraduateThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	/// label
	xml_attribute attr_buselabel = nodePtr->attribute("buselabel");
	if (true == attr_buselabel.empty()) return -1;
	msobjPtr->SetUseLabel(attr_buselabel.as_bool());
	if (true == attr_buselabel.as_bool())
	{
		xml_node label_node = nodePtr->child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		MSLabelThematicObj* pLabelThematicObj = msobjPtr->GetLabelObj();
		if (0 != ReadXml(&label_node, (MSLabelThematicObj*)pLabelThematicObj))
		{

		}
	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->attribute("busefilter");
	if (true == attr_busefilter.empty()) return -1;
	msobjPtr->SetUseFilter(attr_busefilter.as_bool());
	if (true == attr_busefilter.as_bool())
	{
		xml_node filter_node = nodePtr->child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol
	xml_attribute attr_field = nodePtr->attribute("field");
	if (true == attr_field.empty()) return -1;
	msobjPtr->SetField(attr_field.as_string());

	xml_attribute attr_mincolor = nodePtr->attribute("mincolor");
	if (true == attr_mincolor.empty()) return -1;
	MSColor ms_mincolor; ms_mincolor.rgba = attr_mincolor.as_uint();
	msobjPtr->SetColorRampMin(ms_mincolor);

	xml_attribute attr_maxcolor = nodePtr->attribute("maxcolor");
	if (true == attr_maxcolor.empty()) return -1;
	MSColor ms_maxcolor; ms_maxcolor.rgba = attr_maxcolor.as_uint();
	msobjPtr->SetColorRampMax(ms_maxcolor);

	xml_attribute attr_colornumber = nodePtr->attribute("colornumber");
	if (true == attr_colornumber.empty()) return -1;
	msobjPtr->SetColorRampNum(attr_colornumber.as_int());

	xml_attribute attr_usecolornumber = nodePtr->attribute("usecolornumber");
	if (true == attr_usecolornumber.empty()) return -1;
	msobjPtr->SetUseColorRampNum(attr_usecolornumber.as_bool());

	xml_attribute attr_randomcolor = nodePtr->attribute("randomcolor");
	if (true == attr_randomcolor.empty()) return -1;
	msobjPtr->SetUseRandomColor(attr_randomcolor.as_bool());

	xml_attribute attr_clstype = nodePtr->attribute("type");
	if (true == attr_clstype.empty()) return -1;
	msobjPtr->SetClsType(MSClsType(attr_clstype.as_uint()));

	xml_attribute attr_interval = nodePtr->attribute("interval");
	if (true == attr_interval.empty()) return -1;
	msobjPtr->SetInterval(attr_interval.as_double());

	/// MSRangeSymbolObjs
	xml_node rangesymbols_node = nodePtr->child("rangesymbols");
	if (rangesymbols_node.empty() == true)return -1;
	for (pugi::xml_node_iterator it = rangesymbols_node.begin(); it != rangesymbols_node.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "rangesymbol")
		{
			MSRangeSymbolObj* pSymbol = new MSRangeSymbolObj();
			if (pSymbol == nullptr) continue;

			if (0 != msobjPtr->InsertSymbolObj(pSymbol))
			{

			}

			if (0 != ReadXml(&(*it), pSymbol))
			{

			}
		}
		else
		{
			continue;
		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSGraduateThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	/// label
	xml_attribute attr_buselabel = nodePtr->append_attribute("buselabel");
	attr_buselabel.set_value(msobjPtr->GetUseLabel());
	if (true == msobjPtr->GetUseLabel())
	{
		xml_node label_node = nodePtr->append_child("label");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数，沈占锋增加20170119
		if (0 != WriteXml(&label_node, (MSLabelThematicObj*)msobjPtr->GetLabelObj()))
		{

		}

	}

	/// filter
	xml_attribute attr_busefilter = nodePtr->append_attribute("busefilter");
	attr_busefilter.set_value(msobjPtr->GetUseFilter());
	if (true == msobjPtr->GetUseFilter())
	{
		xml_node filter_node = nodePtr->append_child("filter");
		/// MSMultiLayerFillSymbolObj缺少相应的Set函数
	}

	/// symbol

	xml_attribute attr_field = nodePtr->append_attribute("field");
	attr_field.set_value(msobjPtr->GetField());

	xml_attribute attr_colormin = nodePtr->append_attribute("mincolor");
	attr_colormin.set_value(msobjPtr->GetColorRampMin().rgba);

	xml_attribute attr_colormax = nodePtr->append_attribute("maxcolor");
	attr_colormax.set_value(msobjPtr->GetColorRampMax().rgba);

	xml_attribute attr_colornumber = nodePtr->append_attribute("colornumber");
	attr_colornumber.set_value(msobjPtr->GetColorRampNum());

	xml_attribute attr_usecolornumber = nodePtr->append_attribute("usecolornumber");
	attr_usecolornumber.set_value(msobjPtr->GetUseColorRampNum());

	xml_attribute attr_randomcolor = nodePtr->append_attribute("randomcolor");
	attr_randomcolor.set_value(msobjPtr->GetUseRandomColor());

	xml_attribute attr_clstype = nodePtr->append_attribute("clstype");
	attr_clstype.set_value(msobjPtr->GetClsType());

	xml_attribute attr_interval = nodePtr->append_attribute("interval");
	attr_interval.set_value(msobjPtr->GetInterval());

	/// MSRangeSymbolObjs
	xml_node rangesymbols_node = nodePtr->append_child("rangesymbols");
	if (rangesymbols_node.empty() == true)return -1;
	int symbol_num = msobjPtr->GetSymbolCnt();
	for (int i = 0; i < symbol_num; ++i)
	{
		MSRangeSymbolObj* pSymbol = msobjPtr->GetRangeSymbol(i);
		if (pSymbol == nullptr) continue;

		xml_node symbol_node = rangesymbols_node.append_child("rangesymbol");
		if (0 != WriteXml(&symbol_node, pSymbol))
		{

		}
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSChartThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));


	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSChartThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());


	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSRGBRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_colorMode = nodePtr->attribute("colorMode");
	if (true == attr_colorMode.empty()) return -1;
	msobjPtr->SetColorMode(MSColorType(attr_colorMode.as_uint()));

	xml_attribute attr_channelR = nodePtr->attribute("channelR");
	if (true == attr_channelR.empty()) return -1;
	msobjPtr->SetChannelR(attr_channelR.as_uint());

	xml_attribute attr_channelG = nodePtr->attribute("channelG");
	if (true == attr_channelG.empty()) return -1;
	msobjPtr->SetChannelG(attr_channelG.as_uint());

	xml_attribute attr_channelB = nodePtr->attribute("channelB");
	if (true == attr_channelB.empty()) return -1;
	msobjPtr->SetChannelB(attr_channelB.as_uint());

	xml_attribute attr_channelA = nodePtr->attribute("channelA");
	if (true == attr_channelA.empty()) return -1;
	msobjPtr->SetChannelA(attr_channelA.as_uint());

	xml_attribute attr_visibleR = nodePtr->attribute("visibleR");
	if (true == attr_visibleR.empty()) return -1;
	msobjPtr->SetChannelRVisible(attr_visibleR.as_bool());

	xml_attribute attr_visibleG = nodePtr->attribute("visibleG");
	if (true == attr_visibleG.empty()) return -1;
	msobjPtr->SetChannelGVisible(attr_visibleG.as_bool());

	xml_attribute attr_visibleB = nodePtr->attribute("visibleB");
	if (true == attr_visibleB.empty()) return -1;
	msobjPtr->SetChannelBVisible(attr_visibleB.as_bool());

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodataColor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	xml_attribute attr_bgColor = nodePtr->attribute("bgColor");
	if (true == attr_bgColor.empty()) return -1;
	MSColor bg_color; bg_color.rgba = attr_bgColor.as_uint();
	msobjPtr->SetBackGroundColor(bg_color);

	int nBackGroundR, nBackGroundG, nBackGroundB;
	xml_attribute attr_bgR = nodePtr->attribute("bgR");
	if (true == attr_bgR.empty()) return -1;
	nBackGroundR = attr_bgR.as_uint();
	xml_attribute attr_bgG = nodePtr->attribute("bgG");
	if (true == attr_bgG.empty()) return -1;
	nBackGroundG = attr_bgG.as_uint();
	xml_attribute attr_bgB = nodePtr->attribute("bgB");
	if (true == attr_bgB.empty()) return -1;
	nBackGroundB = attr_bgB.as_uint();
	msobjPtr->SetBackGroundValue(nBackGroundR, nBackGroundG, nBackGroundB);

	xml_attribute attr_scalemethod = nodePtr->attribute("scaleMethod");
	if (true == attr_scalemethod.empty()) return -1;
	msobjPtr->SetStretchMethod(MSXMLScalingMethord(attr_scalemethod.as_uint()));

	double min_value, max_value;
	xml_attribute attr_minR = nodePtr->attribute("minR");
	min_value = attr_minR.as_double();
	xml_attribute attr_maxR = nodePtr->attribute("maxR");
	max_value = attr_maxR.as_double();
	msobjPtr->SetMinMaxValueR(min_value, max_value);

	xml_attribute attr_minG = nodePtr->attribute("minG");
	min_value = attr_minG.as_double();
	xml_attribute attr_maxG = nodePtr->attribute("maxG");
	max_value = attr_maxG.as_double();
	msobjPtr->SetMinMaxValueG(min_value, max_value);

	xml_attribute attr_minB = nodePtr->attribute("minB");
	min_value = attr_minB.as_double();
	xml_attribute attr_maxB = nodePtr->attribute("maxB");
	max_value = attr_maxB.as_double();
	msobjPtr->SetMinMaxValueB(min_value, max_value);

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSRGBRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_colorMode = nodePtr->append_attribute("colorMode");
	attr_colorMode.set_value(msobjPtr->GetColorMode());

	xml_attribute attr_channelR = nodePtr->append_attribute("channelR");
	attr_channelR.set_value(msobjPtr->GetChannelR());

	xml_attribute attr_channelG = nodePtr->append_attribute("channelG");
	attr_channelG.set_value(msobjPtr->GetChannelG());

	xml_attribute attr_channelB = nodePtr->append_attribute("channelB");
	attr_channelB.set_value(msobjPtr->GetChannelB());

	xml_attribute attr_channelA = nodePtr->append_attribute("channelA");
	attr_channelA.set_value(msobjPtr->GetChannelA());

	xml_attribute attr_visibleR = nodePtr->append_attribute("visibleR");
	attr_visibleR.set_value(msobjPtr->GetChannelRVisible());

	xml_attribute attr_visibleG = nodePtr->append_attribute("visibleG");
	attr_visibleG.set_value(msobjPtr->GetChannelGVisible());

	xml_attribute attr_visibleB = nodePtr->append_attribute("visibleB");
	attr_visibleB.set_value(msobjPtr->GetChannelBVisible());

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodataColor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	xml_attribute attr_bgColor = nodePtr->append_attribute("bgColor");
	attr_bgColor.set_value(msobjPtr->GetBackGroundColor().rgba);

	int nBackGroundR, nBackGroundG, nBackGroundB;
	msobjPtr->GetBackGroundValue(nBackGroundR, nBackGroundG, nBackGroundB);
	xml_attribute attr_bgR = nodePtr->append_attribute("bgR");
	attr_bgR.set_value(nBackGroundR);
	xml_attribute attr_bgG = nodePtr->append_attribute("bgG");
	attr_bgG.set_value(nBackGroundG);
	xml_attribute attr_bgB = nodePtr->append_attribute("bgB");
	attr_bgB.set_value(nBackGroundB);

	xml_attribute attr_scalemethod = nodePtr->append_attribute("scaleMethod");
	attr_scalemethod.set_value(msobjPtr->GetStretchMethod());

	double min_value, max_value;
	msobjPtr->GetMinMaxValueR(min_value, max_value);
	xml_attribute attr_minR = nodePtr->append_attribute("minR");
	attr_minR.set_value(min_value);
	xml_attribute attr_maxR = nodePtr->append_attribute("maxR");
	attr_maxR.set_value(max_value);

	msobjPtr->GetMinMaxValueG(min_value, max_value);
	xml_attribute attr_minG = nodePtr->append_attribute("minG");
	attr_minG.set_value(min_value);
	xml_attribute attr_maxG = nodePtr->append_attribute("maxG");
	attr_maxG.set_value(max_value);

	msobjPtr->GetMinMaxValueB(min_value, max_value);
	xml_attribute attr_minB = nodePtr->append_attribute("minB");
	attr_minB.set_value(min_value);
	xml_attribute attr_maxB = nodePtr->append_attribute("maxB");
	attr_maxB.set_value(max_value);


	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSClassificationRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_band = nodePtr->attribute("band");
	if (true == attr_band.empty()) return -1;
	msobjPtr->SetBand(attr_band.as_uint());

	xml_attribute attr_hillshade = nodePtr->attribute("hillshade");
	if (true == attr_hillshade.empty()) return -1;
	msobjPtr->SetHillShade(attr_hillshade.as_int());

	xml_attribute attr_usescheme = nodePtr->attribute("usescheme");
	if (true == attr_usescheme.empty()) return -1;
	msobjPtr->SetbUseScheme(attr_usescheme.as_bool());

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodatacolor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	double min_value, max_value;
	xml_attribute attr_minvalue = nodePtr->attribute("minvalue");
	min_value = attr_minvalue.as_double();
	xml_attribute attr_maxvalue = nodePtr->attribute("maxvalue");
	max_value = attr_maxvalue.as_double();
	msobjPtr->SetMinMaxValue(min_value, max_value);

	/// MSRangeSymbolObjs
	xml_node rangesymbols_node = nodePtr->child("rangesymbols");
	if (rangesymbols_node.empty() == true)return -1;
	for (pugi::xml_node_iterator it = rangesymbols_node.begin(); it != rangesymbols_node.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "rangesymbol")
		{
			MSRangeSymbolObj* pSymbol = new MSRangeSymbolObj();
			if (pSymbol == nullptr) continue;

			if (0 != msobjPtr->AddSymbolObj(pSymbol))
			{
				return -1;
			}

			if (0 != ReadXml(&(*it), pSymbol))
			{
				return -1;
			}
		}
		else
		{
			continue;
		}
	}

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSClassificationRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_band = nodePtr->append_attribute("band");
	attr_band.set_value(msobjPtr->GetBand());

	xml_attribute attr_hillshade = nodePtr->append_attribute("hillshade");
	attr_hillshade.set_value(msobjPtr->GetHillShade());

	xml_attribute attr_usescheme = nodePtr->append_attribute("usescheme");
	attr_usescheme.set_value(msobjPtr->GetbUseScheme());

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodatacolor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	double min_value, max_value;
	msobjPtr->GetMinMaxValue(min_value, max_value);
	xml_attribute attr_minvalue = nodePtr->append_attribute("minvalue");
	attr_minvalue.set_value(min_value);
	xml_attribute attr_maxvalue = nodePtr->append_attribute("maxvalue");
	attr_maxvalue.set_value(max_value);

	/// MSRangeSymbolObjs
	xml_node rangesymbols_node = nodePtr->append_child("rangesymbols");
	if (rangesymbols_node.empty() == true)return -1;
	int symbol_num = msobjPtr->GetSymbolCount();
	for (int i = 0; i < symbol_num; ++i)
	{
		MSRangeSymbolObj* pSymbol = msobjPtr->GetRangeSymbol(i);
		if (pSymbol == nullptr) continue;

		xml_node symbol_node = rangesymbols_node.append_child("rangesymbol");
		if (0 != WriteXml(&symbol_node, pSymbol))
		{

		}
	}

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSStretchRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_band = nodePtr->attribute("band");
	if (true == attr_band.empty()) return -1;
	msobjPtr->SetBand(attr_band.as_uint());

	xml_attribute attr_uplabel = nodePtr->attribute("uplabel");
	if (true == attr_uplabel.empty()) return -1;
	msobjPtr->SetUpLabel(attr_uplabel.as_string());

	xml_attribute attr_midlabel = nodePtr->attribute("midlabel");
	if (true == attr_midlabel.empty()) return -1;
	msobjPtr->SetMidLabel(attr_midlabel.as_string());

	xml_attribute attr_lowlabel = nodePtr->attribute("lowlabel");
	if (true == attr_lowlabel.empty()) return -1;
	msobjPtr->SetLowLabel(attr_lowlabel.as_string());

	xml_attribute attr_bgColor = nodePtr->attribute("bgcolor");
	if (true == attr_bgColor.empty()) return -1;
	MSColor bg_color; bg_color.rgba = attr_bgColor.as_uint();
	msobjPtr->SetBackGroundColor(bg_color);

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodatacolor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	xml_attribute attr_hillshade = nodePtr->attribute("hillshade");
	if (true == attr_hillshade.empty()) return -1;
	msobjPtr->SetHillShade(attr_hillshade.as_uint());

	xml_attribute attr_stretchtype = nodePtr->attribute("stretchtype");
	if (true == attr_stretchtype.empty()) return -1;
	msobjPtr->SetStretchType(MSXMLScalingMethord(attr_stretchtype.as_uint()));

	xml_attribute attr_binvert = nodePtr->attribute("binvert");
	if (true == attr_binvert.empty()) return -1;
	msobjPtr->SetbInvert(attr_binvert.as_bool());

	xml_attribute attr_std = nodePtr->attribute("std");
	if (true == attr_std.empty()) return -1;
	msobjPtr->SetSTDValue(attr_std.as_float());

	xml_attribute attr_gramm = nodePtr->attribute("gramm");
	if (true == attr_gramm.empty()) return -1;
	msobjPtr->SetGrammStretch(attr_gramm.as_float());

	xml_attribute attr_statistictype = nodePtr->attribute("statistictype");
	if (true == attr_statistictype.empty()) return -1;
	msobjPtr->SetStatisticType(MSStatisticType(attr_statistictype.as_uint()));

	xml_attribute attr_statisticmin = nodePtr->attribute("statisticmin");
	if (true == attr_statisticmin.empty()) return -1;
	msobjPtr->SetStatisticMin(attr_statisticmin.as_float());

	xml_attribute attr_statisticmax = nodePtr->attribute("statisticmax");
	if (true == attr_statisticmax.empty()) return -1;
	msobjPtr->SetStatisticMax(attr_statisticmax.as_float());

	xml_attribute attr_statisticstd = nodePtr->attribute("statisticstd");
	if (true == attr_statisticstd.empty()) return -1;
	msobjPtr->SetStatisticSTD(attr_statisticstd.as_float());

	xml_attribute attr_statisticmean = nodePtr->attribute("statisticmean");
	if (true == attr_statisticmean.empty()) return -1;
	msobjPtr->SetStatisticMean(attr_statisticmean.as_float());


	/// colorramps
	xml_node attr_colorramps = nodePtr->child("colorramps");
	if (true == attr_colorramps.empty()) return -1;

	xml_attribute attr_internaltype = attr_colorramps.attribute("internaltype");
	if (true == attr_internaltype.empty()) return -1;
	msobjPtr->SetColorInternalType(MSColorInternalType(attr_internaltype.as_uint()));

	for (pugi::xml_node_iterator it = attr_colorramps.begin(); it != attr_colorramps.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "colorramp")
		{
			xml_attribute attr_value = it->attribute("value");
			if (true == attr_value.empty()) continue;
			double valu = attr_value.as_double();

			xml_attribute attr_color = it->attribute("color");
			if (true == attr_color.empty()) continue;
			MSColor color; color.rgba = attr_color.as_uint();

			msobjPtr->AddColorRampPoint(valu, color);
		}
		else
		{
			continue;
		}
	}

	/// cleanup and returns
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSStretchRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_band = nodePtr->append_attribute("band");
	attr_band.set_value(msobjPtr->GetBand());

	xml_attribute attr_uplabel = nodePtr->append_attribute("uplabel");
	attr_uplabel.set_value(msobjPtr->GetUpLabel());

	xml_attribute attr_midlabel = nodePtr->append_attribute("midlabel");
	attr_midlabel.set_value(msobjPtr->GetMidLabel());

	xml_attribute attr_lowlabel = nodePtr->append_attribute("lowlabel");
	attr_lowlabel.set_value(msobjPtr->GetLowLabel());

	xml_attribute attr_bgvalue = nodePtr->append_attribute("bgvalue");
	attr_bgvalue.set_value(msobjPtr->GetBackGroundValue());

	xml_attribute attr_bgcolor = nodePtr->append_attribute("bgcolor");
	attr_bgcolor.set_value(msobjPtr->GetBackGroundColor().rgba);

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodatacolor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	xml_attribute attr_hillshade = nodePtr->append_attribute("hillshade");
	attr_hillshade.set_value(msobjPtr->GetHillShade());

	xml_attribute attr_stretchtype = nodePtr->append_attribute("stretchtype");
	attr_stretchtype.set_value(msobjPtr->GetStretchType());

	xml_attribute attr_binvert = nodePtr->append_attribute("binvert");
	attr_binvert.set_value(msobjPtr->GetbInvert());

	xml_attribute attr_std = nodePtr->append_attribute("std");
	attr_std.set_value(msobjPtr->GetSTDValue());

	xml_attribute attr_gramm = nodePtr->append_attribute("gramm");
	attr_gramm.set_value(msobjPtr->GetGrammStretch());

	xml_attribute attr_statistictype = nodePtr->append_attribute("statistictype");
	attr_statistictype.set_value(msobjPtr->GetStatisticType());

	xml_attribute attr_statisticmin = nodePtr->append_attribute("statisticmin");
	attr_statisticmin.set_value(msobjPtr->GetStatisticMin());

	xml_attribute attr_statisticmax = nodePtr->append_attribute("statisticmax");
	attr_statisticmax.set_value(msobjPtr->GetStatisticMin());

	xml_attribute attr_statisticstd = nodePtr->append_attribute("statisticstd");
	attr_statisticstd.set_value(msobjPtr->GetStatisticMin());

	xml_attribute attr_statisticmean = nodePtr->append_attribute("statisticmean");
	attr_statisticmean.set_value(msobjPtr->GetStatisticMin());

	/// colorramps
	xml_node attr_colorramps = nodePtr->append_child("colorramps");
	MSColorRamps* pColorRamps = msobjPtr->GetColorRampsObj();
	if (pColorRamps == nullptr)return -1;

	xml_attribute attr_internaltype = attr_colorramps.append_attribute("internaltype");
	attr_internaltype.set_value(pColorRamps->_internalType);
	int pair_cnt = pColorRamps->_pair.size();
	for (int i = 0; i < pair_cnt; ++i)
	{
		xml_node attr_colorramp = attr_colorramps.append_child("colorramp");

		xml_attribute attr_value = attr_colorramp.append_attribute("value");
		attr_value.set_value(pColorRamps->_pair[i]._value);

		xml_attribute attr_color = attr_colorramp.append_attribute("color");
		attr_color.set_value(pColorRamps->_pair[i]._color.rgba);
	}

	/// cleanup and returns
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSDiscreteRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_band = nodePtr->attribute("band");
	if (true == attr_band.empty()) return -1;
	msobjPtr->SetBand(attr_band.as_uint());

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodatacolor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	double dMin, dMax;
	xml_attribute attr_statisticmin = nodePtr->attribute("minvalue");
	dMin = attr_statisticmin.as_double();
	xml_attribute attr_statisticmax = nodePtr->attribute("maxvalue");
	dMax = attr_statisticmax.as_double();
	msobjPtr->SetMinMaxValue(dMin, dMax);


	/// colorramps
	xml_node attr_colorramps = nodePtr->child("colorramps");
	if (true == attr_colorramps.empty()) return -1;

	for (pugi::xml_node_iterator it = attr_colorramps.begin(); it != attr_colorramps.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "colorramp")
		{
			xml_attribute attr_value = it->attribute("value");
			if (true == attr_value.empty()) continue;
			double valu = attr_value.as_double();

			xml_attribute attr_color = it->attribute("color");
			if (true == attr_color.empty()) continue;
			MSColor color; color.rgba = attr_color.as_uint();

			msobjPtr->AddColor(valu, color);
		}
		else
		{
			continue;
		}
	}

	/// cleanup and returns
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSDiscreteRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_band = nodePtr->append_attribute("band");
	attr_band.set_value(msobjPtr->GetBand());

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodatacolor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	double dMin, dMax; msobjPtr->GetMinMaxValue(dMin, dMax);
	xml_attribute attr_statisticmin = nodePtr->append_attribute("minvalue");
	attr_statisticmin.set_value(dMin);
	xml_attribute attr_statisticmax = nodePtr->append_attribute("maxvalue");
	attr_statisticmax.set_value(dMax);

	/// colorramps
	xml_node attr_colorramps = nodePtr->append_child("colorramps");
	MSColorRamps* pColorRamps = msobjPtr->GetColorRampsObj();
	if (pColorRamps == nullptr)return -1;

	int pair_cnt = pColorRamps->_pair.size();
	for (int i = 0; i < pair_cnt; ++i)
	{
		xml_node attr_colorramp = attr_colorramps.append_child("colorramp");

		xml_attribute attr_value = attr_colorramp.append_attribute("value");
		attr_value.set_value(pColorRamps->_pair[i]._value);

		xml_attribute attr_color = attr_colorramp.append_attribute("color");
		attr_color.set_value(pColorRamps->_pair[i]._color.rgba);
	}

	/// cleanup and returns
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSColorMapRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_band = nodePtr->attribute("band");
	if (true == attr_band.empty()) return -1;
	msobjPtr->SetBand(attr_band.as_uint());

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodatacolor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	double dMin, dMax;
	xml_attribute attr_statisticmin = nodePtr->attribute("minvalue");
	dMin = attr_statisticmin.as_double();
	xml_attribute attr_statisticmax = nodePtr->attribute("maxvalue");
	dMax = attr_statisticmax.as_double();
	msobjPtr->SetMinMaxValue(dMin, dMax);

	xml_node node_colormap = nodePtr->child("colormap");
	if (node_colormap.empty() == true)return -1;
	for (pugi::xml_node_iterator it = node_colormap.begin(); it != node_colormap.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "color")
		{
			int value; MSColor colorr; std::string label = "";
			value = it->attribute("value").as_uint();
			label = it->attribute("label").as_string();
			colorr.rgba = it->attribute("color").as_uint();
			msobjPtr->AddSymbolObj(value, colorr, label);
		}
	}

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSColorMapRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_band = nodePtr->append_attribute("band");
	attr_band.set_value(msobjPtr->GetBand());

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodatacolor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	double dMin, dMax; msobjPtr->GetMinMaxValue(dMin, dMax);
	xml_attribute attr_statisticmin = nodePtr->append_attribute("minvalue");
	attr_statisticmin.set_value(dMin);
	xml_attribute attr_statisticmax = nodePtr->append_attribute("maxvalue");
	attr_statisticmax.set_value(dMax);

	xml_node node_colormap = nodePtr->append_child("colormap");
	int symbol_cnt = msobjPtr->GetSymbolCount();
	for (int i = 0; i < symbol_cnt; i++)
	{
		MSColor	color = msobjPtr->GetColor(i);
		std::string	s = msobjPtr->GetLabel(i);
		int	value = msobjPtr->GetValue(i);

		xml_node node_color = node_colormap.append_child("color");
		xml_attribute attr_value = node_color.append_attribute("value");
		attr_value.set_value(value);
		xml_attribute attr_label = node_color.append_attribute("label");
		attr_label.set_value(s.c_str());
		xml_attribute attr_color = node_color.append_attribute("color");
		attr_color.set_value(color.rgba);
	}

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSUniqueValueRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetThematicType(MSThematicType(attr_type.as_uint()));

	xml_attribute attr_band = nodePtr->attribute("band");
	if (true == attr_band.empty()) return -1;
	msobjPtr->SetBand(attr_band.as_uint());

	xml_attribute attr_nodatacolor = nodePtr->attribute("nodatacolor");
	if (true == attr_nodatacolor.empty()) return -1;
	MSColor nd_color; nd_color.rgba = attr_nodatacolor.as_uint();
	msobjPtr->SetNoDataColor(nd_color);

	double dMin, dMax;
	xml_attribute attr_statisticmin = nodePtr->attribute("minvalue");
	dMin = attr_statisticmin.as_double();
	xml_attribute attr_statisticmax = nodePtr->attribute("maxvalue");
	dMax = attr_statisticmax.as_double();
	msobjPtr->SetMinMaxValue(dMin, dMax);

	xml_node node_colormap = nodePtr->child("colormap");
	if (node_colormap.empty() == true)return -1;
	for (pugi::xml_node_iterator it = node_colormap.begin(); it != node_colormap.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "color")
		{
			int value; MSColor colorr; std::string label = "";
			value = it->attribute("value").as_uint();
			label = it->attribute("label").as_string();
			colorr.rgba = it->attribute("color").as_uint();
			msobjPtr->AddSymbolObj(value, colorr, label);
		}
	}

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSUniqueValueRasterThematicObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	string node_name = nodePtr->name();
	if (node_name != "MSThematic")return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetThematicType());

	xml_attribute attr_band = nodePtr->append_attribute("band");
	attr_band.set_value(msobjPtr->GetBand());

	xml_attribute attr_nodatacolor = nodePtr->append_attribute("nodatacolor");
	attr_nodatacolor.set_value(msobjPtr->GetNoDataColor().rgba);

	double dMin, dMax; msobjPtr->GetMinMaxValue(dMin, dMax);
	xml_attribute attr_statisticmin = nodePtr->append_attribute("minvalue");
	attr_statisticmin.set_value(dMin);
	xml_attribute attr_statisticmax = nodePtr->append_attribute("maxvalue");
	attr_statisticmax.set_value(dMax);

	xml_node node_colormap = nodePtr->append_child("colormap");
	int symbol_cnt = msobjPtr->GetSymbolCount();
	for (int i = 0; i < symbol_cnt; i++)
	{
		MSColor	color = msobjPtr->GetColor(i);
		std::string	s = msobjPtr->GetLabel(i);
		int	value = msobjPtr->GetValue(i);

		xml_node node_color = node_colormap.append_child("color");
		xml_attribute attr_value = node_color.append_attribute("value");
		attr_value.set_value(value);
		xml_attribute attr_label = node_color.append_attribute("label");
		attr_label.set_value(s.c_str());
		xml_attribute attr_color = node_color.append_attribute("color");
		attr_color.set_value(color.rgba);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSSimpleMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->attribute("edgecolor");
	if (true == attr_edgecolor.empty()) return -1;
	MSColor ms_edgecolor; ms_edgecolor.rgba = attr_edgecolor.as_uint();
	msobjPtr->SetEdgeColor(ms_edgecolor);

	xml_attribute attr_fillcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fillcolor.empty()) return -1;
	MSColor ms_fillcolor; ms_fillcolor.rgba = attr_fillcolor.as_uint();
	msobjPtr->SetFillColor(ms_fillcolor);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_highcolor; ms_highcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHightLightColor(ms_highcolor);

	xml_attribute attr_size = nodePtr->attribute("size");
	if (true == attr_size.empty()) return -1;
	msobjPtr->SetSize(attr_size.as_double());

	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	/// itself
	xml_attribute attr_makertype = nodePtr->attribute("makertype");
	if (true == attr_makertype.empty()) return -1;
	msobjPtr->SetMarkerType(MSMarkerType(attr_makertype.as_uint()));

	xml_attribute attr_multimakertype = nodePtr->attribute("multimakertype");
	if (true == attr_multimakertype.empty()) return -1;
	msobjPtr->SetMultiMarkerType(attr_multimakertype.as_uint());

	xml_attribute attr_edgewidth = nodePtr->attribute("edgewidth");
	if (true == attr_edgewidth.empty()) return -1;
	msobjPtr->SetEdgeLineWidth(attr_edgewidth.as_double());

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSSimpleMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->append_attribute("edgecolor");
	attr_edgecolor.set_value(msobjPtr->GetEdgeColor().rgba);

	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHightLightColor().rgba);

	xml_attribute attr_size = nodePtr->append_attribute("size");
	attr_size.set_value(msobjPtr->GetSize());

	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	/// itself
	xml_attribute attr_makertype = nodePtr->append_attribute("makertype");
	attr_makertype.set_value(msobjPtr->GetMarkerType());

	xml_attribute attr_multimakertype = nodePtr->append_attribute("multimakertype");
	attr_multimakertype.set_value(msobjPtr->GetMultiMarkerType());

	xml_attribute attr_edgewidth = nodePtr->append_attribute("edgewidth");
	attr_edgewidth.set_value(msobjPtr->GetEdgeLineWidth());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSPictureMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->attribute("edgecolor");
	if (true == attr_edgecolor.empty()) return -1;
	MSColor ms_edgecolor; ms_edgecolor.rgba = attr_edgecolor.as_uint();
	msobjPtr->SetEdgeColor(ms_edgecolor);

	xml_attribute attr_fillcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fillcolor.empty()) return -1;
	MSColor ms_fillcolor; ms_fillcolor.rgba = attr_fillcolor.as_uint();
	msobjPtr->SetEdgeColor(ms_fillcolor);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_highcolor; ms_highcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetEdgeColor(ms_highcolor);

	xml_attribute attr_size = nodePtr->attribute("size");
	if (true == attr_size.empty()) return -1;
	msobjPtr->SetSize(attr_size.as_double());

	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	/// itself
	xml_attribute attr_scalex = nodePtr->attribute("scalex");
	if (true == attr_scalex.empty()) return -1;
	msobjPtr->SetScaleX(attr_scalex.as_double());

	xml_attribute attr_scaley = nodePtr->attribute("scaley");
	if (true == attr_scaley.empty()) return -1;
	msobjPtr->SetScaleY(attr_scaley.as_double());

	xml_attribute attr_picfile = nodePtr->attribute("picfile");
	if (true == attr_picfile.empty()) return -1;
	msobjPtr->SetPictureFilename(attr_picfile.as_string());

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSPictureMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->append_attribute("edgecolor");
	attr_edgecolor.set_value(msobjPtr->GetEdgeColor().rgba);

	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHightLightColor().rgba);

	xml_attribute attr_size = nodePtr->append_attribute("size");
	attr_size.set_value(msobjPtr->GetSize());

	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	/// itself
	xml_attribute attr_scalex = nodePtr->append_attribute("scalex");
	attr_scalex.set_value(msobjPtr->GetScaleX());

	xml_attribute attr_scaley = nodePtr->append_attribute("scaley");
	attr_scaley.set_value(msobjPtr->GetScaleY());

	xml_attribute attr_picfile = nodePtr->append_attribute("picfile");
	attr_picfile.set_value(msobjPtr->GetPictureFilename());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSCharacterMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->attribute("edgecolor");
	if (true == attr_edgecolor.empty()) return -1;
	MSColor ms_edgecolor; ms_edgecolor.rgba = attr_edgecolor.as_uint();
	msobjPtr->SetEdgeColor(ms_edgecolor);

	xml_attribute attr_fillcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fillcolor.empty()) return -1;
	MSColor ms_fillcolor; ms_fillcolor.rgba = attr_fillcolor.as_uint();
	msobjPtr->SetEdgeColor(ms_fillcolor);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_highcolor; ms_highcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetEdgeColor(ms_highcolor);

	xml_attribute attr_size = nodePtr->attribute("size");
	if (true == attr_size.empty()) return -1;
	msobjPtr->SetSize(attr_size.as_double());

	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	/// itself
	//xml_attribute attr_edgecolor = nodePtr->attribute("edgecolor");
	//if (true == attr_edgecolor.empty()) return -1;
	//MSColor ms_edgecolor; ms_edgecolor.rgba = attr_edgecolor.as_uint();
	//msobjPtr->SetEdgeColor(ms_edgecolor);

	xml_attribute attr_busebigedge = nodePtr->attribute("busebigedge");
	if (true == attr_busebigedge.empty()) return -1;
	msobjPtr->SetUseBigEdge(attr_busebigedge.as_bool());

	xml_node node_font = nodePtr->child("font");
	msobjPtr->SetFontName(node_font.attribute("name").as_string());
	msobjPtr->SetFontItalic(node_font.attribute("italic").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("blod").as_bool());
	msobjPtr->SetFontItalic(node_font.attribute("underline").as_bool());
	msobjPtr->SetFontStrikeout(node_font.attribute("strikeout").as_bool());

	xml_attribute attr_character = nodePtr->attribute("character");
	if (true == attr_character.empty()) return -1;
	msobjPtr->SetCharacter(*(attr_character.as_string()));

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSCharacterMarkerSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSMarkerSymbolBaseObj
	xml_attribute attr_edgecolor = nodePtr->append_attribute("edgecolor");
	attr_edgecolor.set_value(msobjPtr->GetEdgeColor().rgba);

	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHightLightColor().rgba);

	xml_attribute attr_size = nodePtr->append_attribute("size");
	attr_size.set_value(msobjPtr->GetSize());

	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	/// itself
	//xml_attribute attr_edgecolor = nodePtr->append_attribute("edgecolor");
	//attr_edgecolor.set_value(msobjPtr->GetEdgeColor().rgba);

	xml_attribute attr_busebigedge = nodePtr->append_attribute("busebigedge");
	attr_busebigedge.set_value(msobjPtr->GetUseBigEdge());

	xml_node node_font = nodePtr->append_child("font");
	node_font.append_attribute("name").set_value(msobjPtr->GetFontName());
	node_font.append_attribute("italic").set_value(msobjPtr->GetFontItalic());
	node_font.append_attribute("blod").set_value(msobjPtr->GetFontBold());
	node_font.append_attribute("underline").set_value(msobjPtr->GetFontUnderline());
	node_font.append_attribute("strikeout").set_value(msobjPtr->GetFontStrikeout());

	xml_attribute attr_character = nodePtr->append_attribute("character");
	attr_character.set_value(msobjPtr->GetCharacter());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMultiLayerMarkSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// itself



	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMultiLayerMarkSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// itself


	return 0;
}




int MSProject::ReadXml(pugi::xml_node* nodePtr, MSSimpleLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	xml_attribute attr_start = nodePtr->attribute("start");
	startCap = (MSLineCap)attr_start.as_uint();
	xml_attribute attr_end = nodePtr->attribute("end");
	endCap = (MSLineCap)attr_end.as_uint();
	xml_attribute attr_dash = nodePtr->attribute("dash");
	dashCap = (MSDashCap)attr_dash.as_uint();
	msobjPtr->SetLineCap(startCap, endCap, dashCap);

	xml_attribute attr_join = nodePtr->attribute("join");
	if (true == attr_join.empty()) return -1;
	msobjPtr->SetLineJoin(MSLineJoin(attr_join.as_uint()));

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_attribute attr_width = nodePtr->attribute("width");
	if (true == attr_width.empty()) return -1;
	msobjPtr->SetWidth(attr_width.as_double());

	xml_attribute attr_offset = nodePtr->attribute("offset");
	if (true == attr_offset.empty()) return -1;
	msobjPtr->SetOffset(attr_offset.as_float());

	/// no itself
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSSimpleLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	msobjPtr->GetLineCap(startCap, endCap, dashCap);
	xml_attribute attr_start = nodePtr->append_attribute("start");
	attr_start.set_value(startCap);
	xml_attribute attr_end = nodePtr->append_attribute("end");
	attr_end.set_value(endCap);
	xml_attribute attr_dash = nodePtr->append_attribute("dash");
	attr_dash.set_value(dashCap);

	xml_attribute attr_join = nodePtr->append_attribute("join");
	attr_join.set_value(msobjPtr->GetLineJoin());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_attribute attr_width = nodePtr->append_attribute("width");
	attr_width.set_value(msobjPtr->GetWidth());

	xml_attribute attr_offset = nodePtr->append_attribute("offset");
	attr_offset.set_value(msobjPtr->GetOffset());

	/// no itself
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSHashLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	xml_attribute attr_start = nodePtr->attribute("start");
	startCap = (MSLineCap)attr_start.as_uint();
	xml_attribute attr_end = nodePtr->attribute("end");
	endCap = (MSLineCap)attr_end.as_uint();
	xml_attribute attr_dash = nodePtr->attribute("dash");
	dashCap = (MSDashCap)attr_dash.as_uint();
	msobjPtr->SetLineCap(startCap, endCap, dashCap);

	xml_attribute attr_join = nodePtr->attribute("join");
	if (true == attr_join.empty()) return -1;
	msobjPtr->SetLineJoin(MSLineJoin(attr_join.as_uint()));

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_attribute attr_width = nodePtr->attribute("width");
	if (true == attr_width.empty()) return -1;
	msobjPtr->SetWidth(attr_width.as_double());

	xml_attribute attr_offset = nodePtr->attribute("offset");
	if (true == attr_offset.empty()) return -1;
	msobjPtr->SetOffset(attr_offset.as_float());

	/// itself
	xml_attribute attr_dashtype = nodePtr->attribute("dashtype");
	if (true == attr_dashtype.empty()) return -1;
	msobjPtr->SetDashStyle(MSDashStyle(attr_dashtype.as_uint()));

	xml_attribute attr_dashpattern = nodePtr->attribute("dashpattern");
	if (true == attr_dashpattern.empty()) return -1;
	std::string str_dashpattern = attr_dashpattern.as_string();

	std::stringstream sss_dashpattern; sss_dashpattern << str_dashpattern;
	std::vector<float> vct_dash; float val = 0.0; char split = 0;
	while (sss_dashpattern >> val)
	{
		vct_dash.push_back(val); sss_dashpattern >> split;
	}
	int dash_cnt = vct_dash.size();
	msobjPtr->SetDashPattern(vct_dash.data(), dash_cnt);

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSHashLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	msobjPtr->GetLineCap(startCap, endCap, dashCap);
	xml_attribute attr_start = nodePtr->append_attribute("start");
	attr_start.set_value(startCap);
	xml_attribute attr_end = nodePtr->append_attribute("end");
	attr_end.set_value(endCap);
	xml_attribute attr_dash = nodePtr->append_attribute("dash");
	attr_dash.set_value(dashCap);

	xml_attribute attr_join = nodePtr->append_attribute("join");
	attr_join.set_value(msobjPtr->GetLineJoin());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_attribute attr_width = nodePtr->append_attribute("width");
	attr_width.set_value(msobjPtr->GetWidth());

	xml_attribute attr_offset = nodePtr->append_attribute("offset");
	attr_offset.set_value(msobjPtr->GetOffset());

	/// itself
	xml_attribute attr_dashtype = nodePtr->append_attribute("dashtype");
	attr_dashtype.set_value(msobjPtr->GetDashStyle());

	std::stringstream sss_dashpattern;
	float* dashArray = 0; int nCount = 0;
	msobjPtr->GetDashPattern(dashArray, nCount);
	for (int i = 0; i < nCount; ++i)
	{
		sss_dashpattern << dashArray[i] << '|';
	}
	std::string str_dashpattern = sss_dashpattern.str();

	xml_attribute attr_dashpattern = nodePtr->append_attribute("dashpattern");
	attr_dashtype.set_value(str_dashpattern.c_str());

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMarkerLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	xml_attribute attr_start = nodePtr->attribute("start");
	startCap = (MSLineCap)attr_start.as_uint();
	xml_attribute attr_end = nodePtr->attribute("end");
	endCap = (MSLineCap)attr_end.as_uint();
	xml_attribute attr_dash = nodePtr->attribute("dash");
	dashCap = (MSDashCap)attr_dash.as_uint();
	msobjPtr->SetLineCap(startCap, endCap, dashCap);

	xml_attribute attr_join = nodePtr->attribute("join");
	if (true == attr_join.empty()) return -1;
	msobjPtr->SetLineJoin(MSLineJoin(attr_join.as_uint()));

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_attribute attr_width = nodePtr->attribute("width");
	if (true == attr_width.empty()) return -1;
	msobjPtr->SetWidth(attr_width.as_double());

	xml_attribute attr_offset = nodePtr->attribute("offset");
	if (true == attr_offset.empty()) return -1;
	msobjPtr->SetOffset(attr_offset.as_float());

	/// itself
	xml_attribute attr_interval = nodePtr->attribute("interval");
	if (true == attr_interval.empty()) return -1;
	msobjPtr->SetInterval(attr_interval.as_double());

	xml_node markersymbol_node = nodePtr->child("markersymbol");
	if (markersymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(markersymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pMarkerSymbol = new MSSimpleMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		MSCharacterMarkerSymbolObj* pMarkerSymbol = new MSCharacterMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		MSPictureMarkerSymbolObj* pMarkerSymbol = new MSPictureMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{
		MSMultiLayerMarkSymbolObj* pMarkerSymbol = new MSMultiLayerMarkSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else
	{
		return -1;
	}


	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMarkerLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	msobjPtr->GetLineCap(startCap, endCap, dashCap);
	xml_attribute attr_start = nodePtr->append_attribute("start");
	attr_start.set_value(startCap);
	xml_attribute attr_end = nodePtr->append_attribute("end");
	attr_end.set_value(endCap);
	xml_attribute attr_dash = nodePtr->append_attribute("dash");
	attr_dash.set_value(dashCap);

	xml_attribute attr_join = nodePtr->append_attribute("join");
	attr_join.set_value(msobjPtr->GetLineJoin());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_attribute attr_width = nodePtr->append_attribute("width");
	attr_width.set_value(msobjPtr->GetWidth());

	xml_attribute attr_offset = nodePtr->append_attribute("offset");
	attr_offset.set_value(msobjPtr->GetOffset());

	/// itself
	xml_attribute attr_interval = nodePtr->append_attribute("interval");
	attr_interval.set_value(msobjPtr->GetInterval());

	xml_node markersymbol_node = nodePtr->append_child("markersymbol");
	if (markersymbol_node.empty() == true)return -1;
	MSSymbolObj* pMarkerSymbol = msobjPtr->GetMarkerSymbol();
	if (pMarkerSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pMarkerSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSSimpleMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSCharacterMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSPictureMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSMultiLayerMarkSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else
	{
		return -1;
	}


	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSPictureLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	xml_attribute attr_start = nodePtr->attribute("start");
	startCap = (MSLineCap)attr_start.as_uint();
	xml_attribute attr_end = nodePtr->attribute("end");
	endCap = (MSLineCap)attr_end.as_uint();
	xml_attribute attr_dash = nodePtr->attribute("dash");
	dashCap = (MSDashCap)attr_dash.as_uint();
	msobjPtr->SetLineCap(startCap, endCap, dashCap);

	xml_attribute attr_join = nodePtr->attribute("join");
	if (true == attr_join.empty()) return -1;
	msobjPtr->SetLineJoin(MSLineJoin(attr_join.as_uint()));

	xml_attribute attr_color = nodePtr->attribute("color");
	if (true == attr_color.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_color.as_uint();
	msobjPtr->SetColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_attribute attr_width = nodePtr->attribute("width");
	if (true == attr_width.empty()) return -1;
	msobjPtr->SetWidth(attr_width.as_double());

	xml_attribute attr_offset = nodePtr->attribute("offset");
	if (true == attr_offset.empty()) return -1;
	msobjPtr->SetOffset(attr_offset.as_float());

	/// itself
	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_scalex = nodePtr->attribute("scalex");
	if (true == attr_scalex.empty()) return -1;
	msobjPtr->SetScaleX(attr_scalex.as_double());

	xml_attribute attr_scaley = nodePtr->attribute("scaley");
	if (true == attr_scaley.empty()) return -1;
	msobjPtr->SetScaleY(attr_scaley.as_double());

	xml_attribute attr_picfile = nodePtr->attribute("picfile");
	if (true == attr_picfile.empty()) return -1;
	msobjPtr->SetPictureName(attr_picfile.as_string());

	xml_attribute attr_wraptype = nodePtr->attribute("wraptype");
	if (true == attr_wraptype.empty()) return -1;
	msobjPtr->SetWrapMode(MSWrapMode(attr_wraptype.as_uint()));

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSPictureLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSLineSymbolBaseObj
	MSLineCap startCap, endCap; MSDashCap dashCap;
	msobjPtr->GetLineCap(startCap, endCap, dashCap);
	xml_attribute attr_start = nodePtr->append_attribute("start");
	attr_start.set_value(startCap);
	xml_attribute attr_end = nodePtr->append_attribute("end");
	attr_end.set_value(endCap);
	xml_attribute attr_dash = nodePtr->append_attribute("dash");
	attr_dash.set_value(dashCap);

	xml_attribute attr_join = nodePtr->append_attribute("join");
	attr_join.set_value(msobjPtr->GetLineJoin());

	xml_attribute attr_color = nodePtr->append_attribute("color");
	attr_color.set_value(msobjPtr->GetColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_attribute attr_width = nodePtr->append_attribute("width");
	attr_width.set_value(msobjPtr->GetWidth());

	xml_attribute attr_offset = nodePtr->append_attribute("offset");
	attr_offset.set_value(msobjPtr->GetOffset());

	/// itself
	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_scalex = nodePtr->append_attribute("scalex");
	attr_scalex.set_value(msobjPtr->GetScaleX());

	xml_attribute attr_scaley = nodePtr->append_attribute("scaley");
	attr_scaley.set_value(msobjPtr->GetScaleY());

	xml_attribute attr_picfile = nodePtr->append_attribute("picfile");
	attr_picfile.set_value(msobjPtr->GetPictureName());

	xml_attribute attr_wraptype = nodePtr->append_attribute("wraptype");
	attr_wraptype.set_value(msobjPtr->GetWrapMode());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMultiLayerLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// itself

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMultiLayerLineSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj

	return 0;
}




int MSProject::ReadXml(pugi::xml_node* nodePtr, MSSimpleFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// no itself
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSSimpleFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// no itself
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSGraduateFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	MSPoint center_pt; MSColor center_color;

	xml_node node_center = nodePtr->child("center");
	if (node_center.empty() == true)return -1;
	xml_attribute attr_centercolor = node_center.attribute("centercolor");
	center_color.rgba = attr_centercolor.as_uint();
	xml_attribute attr_centerptx = node_center.attribute("pointx");
	center_pt.x = attr_centerptx.as_int();
	xml_attribute attr_centerpty = node_center.attribute("pointy");
	center_pt.y = attr_centerpty.as_int();

	msobjPtr->SetCenterPOINTAndColor(center_pt, center_color);

	xml_node node_pointsandcolors = nodePtr->child("pointsandcolors");
	if (node_pointsandcolors.empty() == true)return -1;
	for (pugi::xml_node_iterator it = node_pointsandcolors.begin(); it != node_pointsandcolors.end(); ++it)
	{
		std::string node_name = it->name();
		if (node_name == "pointandcolor")
		{
			MSPoint ptt; MSColor colorr;

			xml_attribute attr_color = it->attribute("color");
			attr_centercolor.set_value(colorr.rgba);
			xml_attribute attr_ptx = it->attribute("pointx");
			attr_ptx.set_value(ptt.x);
			xml_attribute attr_pty = it->attribute("pointy");
			attr_pty.set_value(ptt.y);

			msobjPtr->AddPOINTAndColor(ptt, colorr);
		}
	}


	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSGraduateFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	MSPoint center_pt; MSColor center_color;
	msobjPtr->GetCenterPOINTAndColor(center_pt, center_color);

	xml_node node_center = nodePtr->append_child("center");
	if (node_center.empty() == true)return -1;
	xml_attribute attr_centercolor = node_center.append_attribute("centercolor");
	attr_centercolor.set_value(center_color.rgba);
	xml_attribute attr_centerptx = node_center.append_attribute("pointx");
	attr_centerptx.set_value(center_pt.x);
	xml_attribute attr_centerpty = node_center.append_attribute("pointy");
	attr_centerpty.set_value(center_pt.y);

	xml_node node_pointsandcolors = nodePtr->append_child("pointsandcolors");
	if (node_pointsandcolors.empty() == true)return -1;

	int numm = msobjPtr->GetPOINTNum();
	for (int i = 0; i < numm; ++i)
	{
		MSPoint ptt; MSColor colorr; msobjPtr->GetPOINTAndColor(i, ptt, colorr);

		xml_node node_pointcenter = node_pointsandcolors.append_child("pointandcolor");
		if (node_pointcenter.empty() == true)return -1;

		xml_attribute attr_color = node_pointcenter.append_attribute("color");
		attr_centercolor.set_value(colorr.rgba);
		xml_attribute attr_ptx = node_pointcenter.append_attribute("pointx");
		attr_ptx.set_value(ptt.x);
		xml_attribute attr_pty = node_pointcenter.append_attribute("pointy");
		attr_pty.set_value(ptt.y);
	}

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMarkerFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_intervalx = nodePtr->attribute("intervalx");
	if (true == attr_intervalx.empty()) return -1;
	msobjPtr->SetFillIntVX(attr_intervalx.as_float());

	xml_attribute attr_intervaly = nodePtr->attribute("intervaly");
	if (true == attr_intervaly.empty()) return -1;
	msobjPtr->SetFillIntVY(attr_intervaly.as_float());

	xml_attribute attr_buserandom = nodePtr->attribute("buserandom");
	if (true == attr_buserandom.empty()) return -1;
	msobjPtr->SetVisible(attr_buserandom.as_bool());


	xml_node markersymbol_node = nodePtr->child("markersymbol");
	if (markersymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type1 = MSSymbolType(markersymbol_node.attribute("type").as_uint());
	if (symbol_type1 == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pMarkerSymbol = new MSSimpleMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_CHARACTER_MARKER_SYMBOL)
	{
		MSCharacterMarkerSymbolObj* pMarkerSymbol = new MSCharacterMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_PICTURE_MARKER_SYMBOL)
	{
		MSPictureMarkerSymbolObj* pMarkerSymbol = new MSPictureMarkerSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MULTILAYER_MARKER_SYMBOL)
	{
		MSMultiLayerMarkSymbolObj* pMarkerSymbol = new MSMultiLayerMarkSymbolObj();
		if (pMarkerSymbol == nullptr) return -1;
		msobjPtr->SetMarkerSymbol(pMarkerSymbol);
		if (0 != ReadXml(&markersymbol_node, pMarkerSymbol))
		{

		}
	}
	else
	{
		return -1;
	}


	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMarkerFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_intervalx = nodePtr->append_attribute("intervalx");
	attr_intervalx.set_value(msobjPtr->GetFillIntVX());

	xml_attribute attr_intervaly = nodePtr->append_attribute("intervaly");
	attr_intervaly.set_value(msobjPtr->GetFillIntVY());

	xml_attribute attr_buserandom = nodePtr->append_attribute("buserandom");
	attr_buserandom.set_value(msobjPtr->GetUseRandomMarker());

	xml_node markersymbol_node = nodePtr->append_child("markersymbol");
	if (markersymbol_node.empty() == true)return -1;
	MSSymbolObj* pMarkerSymbol = msobjPtr->GetMarkerSymbol();
	if (pMarkerSymbol == nullptr) return -1;

	MSSymbolType symbol_type1 = pMarkerSymbol->GetSymbolType();
	if (symbol_type1 == MS_SIMPLE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSSimpleMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_CHARACTER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSCharacterMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_PICTURE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSPictureMarkerSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MULTILAYER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&markersymbol_node, (MSMultiLayerMarkSymbolObj*)pMarkerSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSPictureFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_scalex = nodePtr->attribute("scalex");
	if (true == attr_scalex.empty()) return -1;
	msobjPtr->SetScaleX(attr_scalex.as_double());

	xml_attribute attr_scaley = nodePtr->attribute("scaley");
	if (true == attr_scaley.empty()) return -1;
	msobjPtr->SetScaleY(attr_scaley.as_double());

	xml_attribute attr_offsetx = nodePtr->attribute("offsetx");
	if (true == attr_offsetx.empty()) return -1;
	msobjPtr->SetOffsetX(attr_offsetx.as_double());

	xml_attribute attr_offsety = nodePtr->attribute("offsety");
	if (true == attr_offsety.empty()) return -1;
	msobjPtr->SetOffsetY(attr_offsety.as_double());

	xml_attribute attr_separationx = nodePtr->attribute("separationx");
	if (true == attr_separationx.empty()) return -1;
	msobjPtr->SetSeparationX(attr_separationx.as_double());

	xml_attribute attr_separationy = nodePtr->attribute("separationy");
	if (true == attr_separationy.empty()) return -1;
	msobjPtr->SetSeparationY(attr_separationy.as_double());

	xml_attribute attr_picfile = nodePtr->attribute("picfile");
	if (true == attr_picfile.empty()) return -1;
	msobjPtr->SetPictureFilename(attr_picfile.as_string());

	xml_attribute attr_wraptype = nodePtr->attribute("wraptype");
	if (true == attr_wraptype.empty()) return -1;
	msobjPtr->SetWrapMode(MSWrapMode(attr_wraptype.as_uint()));

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSPictureFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_scalex = nodePtr->append_attribute("scalex");
	attr_scalex.set_value(msobjPtr->GetScaleX());

	xml_attribute attr_scaley = nodePtr->append_attribute("scaley");
	attr_scaley.set_value(msobjPtr->GetScaleY());

	xml_attribute attr_offsetx = nodePtr->append_attribute("offsetx");
	attr_offsetx.set_value(msobjPtr->GetOffsetX());

	xml_attribute attr_offsety = nodePtr->append_attribute("offsety");
	attr_offsety.set_value(msobjPtr->GetOffsetY());

	xml_attribute attr_separationx = nodePtr->append_attribute("separationx");
	attr_separationx.set_value(msobjPtr->GetSeparationX());

	xml_attribute attr_separationy = nodePtr->append_attribute("separationy");
	attr_separationy.set_value(msobjPtr->GetSeparationY());

	xml_attribute attr_picfile = nodePtr->append_attribute("picfile");
	attr_picfile.set_value(msobjPtr->GetPictureFilename());

	xml_attribute attr_wraptype = nodePtr->append_attribute("wraptype");
	attr_wraptype.set_value(msobjPtr->GetWrapMode());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSHatchFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_fillcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fillcolor.empty()) return -1;
	MSColor ms_fillcolor; ms_fillcolor.rgba = attr_fillcolor.as_uint();
	msobjPtr->SetForeColor(ms_fillcolor);

	xml_attribute attr_backcolor = nodePtr->attribute("backcolor");
	if (true == attr_backcolor.empty()) return -1;
	MSColor ms_backcolor; ms_backcolor.rgba = attr_backcolor.as_uint();
	msobjPtr->SetBackColor(ms_backcolor);

	xml_attribute attr_filltype = nodePtr->attribute("filltype");
	if (true == attr_filltype.empty()) return -1;
	msobjPtr->SetFillStyle(MSFillStyle(attr_filltype.as_uint()));

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSHatchFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("forecolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_forecolor = nodePtr->append_attribute("forecolor");
	attr_forecolor.set_value(msobjPtr->GetForeColor().rgba);

	xml_attribute attr_backcolor = nodePtr->append_attribute("backcolor");
	attr_backcolor.set_value(msobjPtr->GetBackColor().rgba);

	xml_attribute attr_filltype = nodePtr->append_attribute("filltype");
	attr_filltype.set_value(msobjPtr->GetFillStyle());

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSLineFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fcolor = nodePtr->attribute("fillcolor");
	if (true == attr_fcolor.empty()) return -1;
	MSColor ms_color; ms_color.rgba = attr_fcolor.as_uint();
	msobjPtr->SetFillColor(ms_color);

	xml_attribute attr_highcolor = nodePtr->attribute("highcolor");
	if (true == attr_highcolor.empty()) return -1;
	MSColor ms_hcolor; ms_hcolor.rgba = attr_highcolor.as_uint();
	msobjPtr->SetHighLightColor(ms_hcolor);

	xml_node outlinesymbol_node = nodePtr->child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(outlinesymbol_node.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetOutlineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_angle = nodePtr->attribute("angle");
	if (true == attr_angle.empty()) return -1;
	msobjPtr->SetAngle(attr_angle.as_double());

	xml_attribute attr_interval = nodePtr->attribute("interval");
	if (true == attr_interval.empty()) return -1;
	msobjPtr->SetInteval(attr_interval.as_double());


	xml_node filllinesymbol_node = nodePtr->child("filllinesymbol");
	if (filllinesymbol_node.empty() == true)return -1;
	MSSymbolType symbol_type1 = MSSymbolType(filllinesymbol_node.attribute("type").as_uint());
	if (symbol_type1 == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pOutSymbol = new MSSimpleLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetFilllineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pOutSymbol = new MSHashLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetFilllineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pOutSymbol = new MSMarkerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetFilllineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pOutSymbol = new MSPictureLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetFilllineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MULTILAYER_LINE_SYMBOL)
	{
		MSMultiLayerLineSymbolObj* pOutSymbol = new MSMultiLayerLineSymbolObj();
		if (pOutSymbol == nullptr) return -1;
		msobjPtr->SetFilllineSymbol(pOutSymbol);
		if (0 != ReadXml(&outlinesymbol_node, pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSLineFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	/// from MSSymbolObj
	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	/// from MSFillSymbolBaseObj
	xml_attribute attr_fillcolor = nodePtr->append_attribute("fillcolor");
	attr_fillcolor.set_value(msobjPtr->GetFillColor().rgba);

	xml_attribute attr_highcolor = nodePtr->append_attribute("highcolor");
	attr_highcolor.set_value(msobjPtr->GetHighLightColor().rgba);

	xml_node outlinesymbol_node = nodePtr->append_child("outlinesymbol");
	if (outlinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pOutSymbol = msobjPtr->GetOutlineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pOutSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pOutSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	/// itself
	xml_attribute attr_angle = nodePtr->append_attribute("angle");
	attr_angle.set_value(msobjPtr->GetAngle());

	xml_attribute attr_interval = nodePtr->append_attribute("interval");
	attr_interval.set_value(msobjPtr->GetInterval());


	xml_node filllinesymbol_node = nodePtr->append_child("filllinesymbol");
	if (filllinesymbol_node.empty() == true)return -1;
	MSSymbolObj* pfillSymbol = msobjPtr->GetFilllineSymbol();
	if (pOutSymbol == nullptr) return -1;

	MSSymbolType symbol_type1 = pfillSymbol->GetSymbolType();
	if (symbol_type1 == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSSimpleLineSymbolObj*)pfillSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSHashLineSymbolObj*)pfillSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMarkerLineSymbolObj*)pfillSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSPictureLineSymbolObj*)pfillSymbol))
		{

		}
	}
	else if (symbol_type1 == MS_MULTILAYER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&outlinesymbol_node, (MSMultiLayerLineSymbolObj*)pfillSymbol))
		{

		}
	}
	else
	{
		return -1;
	}

	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSMultiLayerFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());



	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSMultiLayerFillSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());


	return 0;
}




int MSProject::ReadXml(pugi::xml_node* nodePtr, MSRangeSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	xml_attribute attr_label = nodePtr->attribute("label");
	if (true == attr_label.empty()) return -1;
	msobjPtr->SetLabel(attr_label.as_string());

	xml_attribute attr_minvalue = nodePtr->attribute("minvalue");
	if (true == attr_minvalue.empty()) return -1;
	msobjPtr->SetMinValue(attr_minvalue.as_double());

	xml_attribute attr_maxvalue = nodePtr->attribute("maxvalue");
	if (true == attr_maxvalue.empty()) return -1;
	msobjPtr->SetMaxValue(attr_maxvalue.as_double());

	/// Symbol
	xml_node node_symbol = nodePtr->child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(node_symbol.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pSymbol = new MSSimpleMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		MSCharacterMarkerSymbolObj* pSymbol = new MSCharacterMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		MSPictureMarkerSymbolObj* pSymbol = new MSPictureMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pSymbol = new MSSimpleLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pSymbol = new MSHashLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pSymbol = new MSMarkerLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pSymbol = new MSPictureLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSymbol = new MSSimpleFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		MSGraduateFillSymbolObj* pSymbol = new MSGraduateFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		MSMarkerFillSymbolObj* pSymbol = new MSMarkerFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		MSHatchFillSymbolObj* pSymbol = new MSHatchFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		MSLineFillSymbolObj* pSymbol = new MSLineFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		MSPictureFillSymbolObj* pSymbol = new MSPictureFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{

	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSRangeSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	xml_attribute attr_label = nodePtr->append_attribute("label");
	attr_label.set_value(msobjPtr->GetLabel());

	xml_attribute attr_minvalue = nodePtr->append_attribute("minvalue");
	attr_minvalue.set_value(msobjPtr->GetMinValue());

	xml_attribute attr_maxvalue = nodePtr->append_attribute("maxvalue");
	attr_maxvalue.set_value(msobjPtr->GetMaxValue());

	/// Symbol
	xml_node node_symbol = nodePtr->append_child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolObj* pSymbol = msobjPtr->GetSymbol();
	if (pSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSCharacterMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSHashLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSGraduateFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSHatchFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSLineFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{

	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::ReadXml(pugi::xml_node* nodePtr, MSValueSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->attribute("type");
	if (true == attr_type.empty()) return -1;
	msobjPtr->SetSymbolType(MSSymbolType(attr_type.as_uint()));

	xml_attribute attr_visible = nodePtr->attribute("visible");
	if (true == attr_visible.empty()) return -1;
	msobjPtr->SetVisible(attr_visible.as_bool());

	xml_attribute attr_value = nodePtr->attribute("value");
	if (true == attr_value.empty()) return -1;
	msobjPtr->SetValue(attr_value.as_string());

	xml_attribute attr_label = nodePtr->attribute("label");
	if (true == attr_label.empty()) return -1;
	msobjPtr->SetLabel(attr_label.as_string());

	/// Symbol
	xml_node node_symbol = nodePtr->child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolType symbol_type = MSSymbolType(node_symbol.attribute("type").as_uint());
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		MSSimpleMarkerSymbolObj* pSymbol = new MSSimpleMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		MSCharacterMarkerSymbolObj* pSymbol = new MSCharacterMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		MSPictureMarkerSymbolObj* pSymbol = new MSPictureMarkerSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		MSSimpleLineSymbolObj* pSymbol = new MSSimpleLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		MSHashLineSymbolObj* pSymbol = new MSHashLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		MSMarkerLineSymbolObj* pSymbol = new MSMarkerLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		MSPictureLineSymbolObj* pSymbol = new MSPictureLineSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		MSSimpleFillSymbolObj* pSymbol = new MSSimpleFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		MSGraduateFillSymbolObj* pSymbol = new MSGraduateFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		MSMarkerFillSymbolObj* pSymbol = new MSMarkerFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		MSHatchFillSymbolObj* pSymbol = new MSHatchFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		MSLineFillSymbolObj* pSymbol = new MSLineFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		MSPictureFillSymbolObj* pSymbol = new MSPictureFillSymbolObj();
		if (pSymbol == nullptr) return -1;
		msobjPtr->SetSymbol(pSymbol);
		if (0 != ReadXml(&node_symbol, pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{

	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}


int MSProject::WriteXml(pugi::xml_node* nodePtr, MSValueSymbolObj* msobjPtr)
{
	if (nodePtr == 0 || msobjPtr == 0)return -1;

	xml_attribute attr_type = nodePtr->append_attribute("type");
	attr_type.set_value(msobjPtr->GetSymbolType());

	xml_attribute attr_visible = nodePtr->append_attribute("visible");
	attr_visible.set_value(msobjPtr->GetVisible());

	xml_attribute attr_value = nodePtr->append_attribute("value");
	attr_value.set_value(msobjPtr->GetValue());

	xml_attribute attr_label = nodePtr->append_attribute("label");
	attr_label.set_value(msobjPtr->GetLabel());

	/// Symbol
	xml_node node_symbol = nodePtr->append_child("symbol");
	if (node_symbol.empty() == true)return -1;
	MSSymbolObj* pSymbol = msobjPtr->GetSymbol();
	if (pSymbol == nullptr) return -1;

	MSSymbolType symbol_type = pSymbol->GetSymbolType();
	if (symbol_type == MS_SIMPLE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_CHARACTER_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSCharacterMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_MARKER_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureMarkerSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_MARKER_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HASH_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSHashLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_LINE_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureLineSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_LINE_SYMBOL)
	{

	}
	else if (symbol_type == MS_SIMPLE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSSimpleFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_GRADUATE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSGraduateFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MARKER_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSMarkerFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_HATCH_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSHatchFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_LINE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSLineFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_PICTURE_FILL_SYMBOL)
	{
		if (0 != WriteXml(&node_symbol, (MSPictureFillSymbolObj*)pSymbol))
		{

		}
	}
	else if (symbol_type == MS_MULTILAYER_FILL_SYMBOL)
	{

	}
	else
	{
		return -1;
	}

	/// cleanup and return
	return 0;
}