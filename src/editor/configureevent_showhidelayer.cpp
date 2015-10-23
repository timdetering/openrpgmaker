/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_showhidelayer.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "variableselector.h"

ConfigureEvent_ShowHideLayer::ConfigureEvent_ShowHideLayer(int mapID, bool show, QWidget *parent) : QDialog(parent)
{
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setupUi(this);
	for (int i = 0; i < (*mapRef)->numLayers(); ++i)
		cboxLayers->addItem((*mapRef)->getLayerName(i));
	delete mapRef;
	showLayer = show;
	if (showLayer)
		cboxEraseTransitions->setVisible(false);
	else
		cboxShowTransitions->setVisible(false);
	lblLayerVariable->setText(ProjectData::numberVariableIdToString(0));
	wLayerVariable->setEnabled(false);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ShowHideLayer::~ConfigureEvent_ShowHideLayer()
{
}

void ConfigureEvent_ShowHideLayer::setLayer(int layer)
{
	if (rbSpecificLayer->isChecked())
		cboxLayers->setCurrentIndex(layer);
	else
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(layer));
}

void ConfigureEvent_ShowHideLayer::setTransition(int transition)
{
	if (showLayer)
		cboxShowTransitions->setCurrentIndex(transition);
	else
		cboxEraseTransitions->setCurrentIndex(transition);
}

void ConfigureEvent_ShowHideLayer::setLayerReferencedInVariable(bool layerReferencedInVariable)
{
	if (layerReferencedInVariable)
		rbReferencedLayer->setChecked(true);
	else
		rbSpecificLayer->setChecked(true);
}

int ConfigureEvent_ShowHideLayer::getLayer()
{
	if (rbSpecificLayer->isChecked())
		return cboxLayers->currentIndex();
	return ProjectData::numberVariableIdFromString(lblLayerVariable->text());
}

int ConfigureEvent_ShowHideLayer::getTransition()
{
	if (showLayer)
		return cboxShowTransitions->currentIndex();
	return cboxEraseTransitions->currentIndex();
}

bool ConfigureEvent_ShowHideLayer::isLayerReferencedInVariable()
{
	return rbReferencedLayer->isChecked();
}

void ConfigureEvent_ShowHideLayer::on_bBrowseLayerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblLayerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ShowHideLayer::on_rbSpecificLayer_toggled(bool checked)
{
	cboxLayers->setEnabled(checked);
	wLayerVariable->setEnabled(!checked);
}
