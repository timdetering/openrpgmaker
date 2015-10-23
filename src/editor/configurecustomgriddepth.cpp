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
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QDialog>
#include "configurecustomgriddepth.h"
#include "projectdata.h"
#include "projectdata_terrain.h"

ConfigureCustomGridDepth::ConfigureCustomGridDepth(Terrain::GridDepthValues *gridDepth, QString backgroundLocation, QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	setupUi(this);
	regularLinePen = QPen(QBrush(Qt::white), 1.0, Qt::SolidLine, Qt::FlatCap);
	selectedLinePen = QPen(QBrush(Qt::red), 1.0, Qt::SolidLine, Qt::FlatCap);
	hTopLineLocation = gridDepth->hTopLineLocation;
	hBottomLineLocation = gridDepth->hBottomLineLocation;
	vMonsterLine_TopLocation = gridDepth->vMonsterLine_TopLocation;
	vMonsterLine_BottomLocation = gridDepth->vMonsterLine_BottomLocation;
	vMonsterLine_Spacing = gridDepth->vMonsterLine_Spacing;
	vCharacterLine_TopLocation = gridDepth->vCharacterLine_TopLocation;
	vCharacterLine_BottomLocation = gridDepth->vCharacterLine_BottomLocation;
	vCharacterLine_Spacing = gridDepth->vCharacterLine_Spacing;
	bLineValueSideButton->setSpinBox(sbLineValue);
	previewScene = new QGraphicsScene(0.0, 0.0, 640.0, 480.0);
	if (backgroundLocation.isEmpty())
		backgroundItem = new QGraphicsPixmapItem();
	else
		backgroundItem = new QGraphicsPixmapItem(QPixmap(backgroundLocation));
	backgroundItem->setFlags(0x0);
	backgroundItem->setPos(0.0, 0.0);
	backgroundItem->setZValue(0.0);
	hLine_Top = new QGraphicsLineItem;
	hLine_Top->setFlags(0x0);
	hLine_CenterTop = new QGraphicsLineItem;
	hLine_CenterTop->setFlags(0x0);
	hLine_CenterTop->setPen(regularLinePen);
	hLine_CenterBottom = new QGraphicsLineItem;
	hLine_CenterBottom->setFlags(0x0);
	hLine_CenterBottom->setPen(regularLinePen);
	hLine_Bottom = new QGraphicsLineItem;
	hLine_Bottom->setFlags(0x0);
	vLine_Monsters_Left = new QGraphicsLineItem;
	vLine_Monsters_Left->setFlags(0x0);
	vLine_Monsters_Left->setPen(regularLinePen);
	vLine_Monsters_Right = new QGraphicsLineItem;
	vLine_Monsters_Right->setFlags(0x0);
	vLine_Monsters_Right->setPen(regularLinePen);
	vLine_Monsters_SpacingTop = new QGraphicsLineItem;
	vLine_Monsters_SpacingTop->setFlags(0x0);
	vLine_Monsters_SpacingCenterTop = new QGraphicsLineItem;
	vLine_Monsters_SpacingCenterTop->setFlags(0x0);
	vLine_Monsters_SpacingCenterBottom = new QGraphicsLineItem;
	vLine_Monsters_SpacingCenterBottom->setFlags(0x0);
	vLine_Monsters_SpacingBottom = new QGraphicsLineItem;
	vLine_Monsters_SpacingBottom->setFlags(0x0);
	vLine_Center = new QGraphicsLineItem;
	vLine_Center->setFlags(0x0);
	vLine_Center->setPen(regularLinePen);
	vLine_Characters_Left = new QGraphicsLineItem;
	vLine_Characters_Left->setFlags(0x0);
	vLine_Characters_Left->setPen(regularLinePen);
	vLine_Characters_Right = new QGraphicsLineItem;
	vLine_Characters_Right->setFlags(0x0);
	vLine_Characters_Right->setPen(regularLinePen);
	vLine_Characters_SpacingTop = new QGraphicsLineItem;
	vLine_Characters_SpacingTop->setFlags(0x0);
	vLine_Characters_SpacingCenterTop = new QGraphicsLineItem;
	vLine_Characters_SpacingCenterTop->setFlags(0x0);
	vLine_Characters_SpacingCenterBottom = new QGraphicsLineItem;
	vLine_Characters_SpacingCenterBottom->setFlags(0x0);
	vLine_Characters_SpacingBottom = new QGraphicsLineItem;
	ellipseItem = new QGraphicsEllipseItem;
	ellipseItem->setFlags(0x0);
	ellipseItem->setPen(selectedLinePen);
	selectedLine = LINE_HTOP;
	hsLineValue->setRange(0, hBottomLineLocation);
	sbLineValue->setRange(0, hBottomLineLocation);
	hsLineValue->setValue(hTopLineLocation);
	sbLineValue->setValue(hTopLineLocation);
	updateLines();
	previewScene->addItem(backgroundItem);
	previewScene->addItem(hLine_Top);
	previewScene->addItem(hLine_CenterTop);
	previewScene->addItem(hLine_CenterBottom);
	previewScene->addItem(hLine_Bottom);
	previewScene->addItem(vLine_Monsters_Left);
	previewScene->addItem(vLine_Monsters_Right);
	previewScene->addItem(vLine_Monsters_SpacingTop);
	previewScene->addItem(vLine_Monsters_SpacingCenterTop);
	previewScene->addItem(vLine_Monsters_SpacingCenterBottom);
	previewScene->addItem(vLine_Monsters_SpacingBottom);
	previewScene->addItem(vLine_Center);
	previewScene->addItem(vLine_Characters_Left);
	previewScene->addItem(vLine_Characters_Right);
	previewScene->addItem(vLine_Characters_SpacingTop);
	previewScene->addItem(vLine_Characters_SpacingCenterTop);
	previewScene->addItem(vLine_Characters_SpacingCenterBottom);
	previewScene->addItem(vLine_Characters_SpacingBottom);
	previewScene->addItem(ellipseItem);
	gvGridPreview->setScene(previewScene);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ignoreEvents = false;
}

ConfigureCustomGridDepth::~ConfigureCustomGridDepth()
{
}

void ConfigureCustomGridDepth::applyValues(Terrain::GridDepthValues *gridDepth)
{
	gridDepth->hTopLineLocation = hTopLineLocation;
	gridDepth->hBottomLineLocation = hBottomLineLocation;
	gridDepth->vMonsterLine_TopLocation = vMonsterLine_TopLocation;
	gridDepth->vMonsterLine_BottomLocation = vMonsterLine_BottomLocation;
	gridDepth->vMonsterLine_Spacing = vMonsterLine_Spacing;
	gridDepth->vCharacterLine_TopLocation = vCharacterLine_TopLocation;
	gridDepth->vCharacterLine_BottomLocation = vCharacterLine_BottomLocation;
	gridDepth->vCharacterLine_Spacing = vCharacterLine_Spacing;
}

void ConfigureCustomGridDepth::on_rbTopLine_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_HTOP;
		updateLines();
		hsLineValue->setRange(0, hBottomLineLocation);
		sbLineValue->setRange(0, hBottomLineLocation);
		hsLineValue->setValue(hTopLineLocation);
		sbLineValue->setValue(hTopLineLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbBottomLine_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_HBOTTOM;
		updateLines();
		hsLineValue->setRange(hTopLineLocation, 479);
		sbLineValue->setRange(hTopLineLocation, 479);
		hsLineValue->setValue(hBottomLineLocation);
		sbLineValue->setValue(hBottomLineLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbMonsterLines_Top_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VMONSTERTOP;
		updateLines();
		hsLineValue->setRange(vMonsterLine_BottomLocation, 319);
		sbLineValue->setRange(vMonsterLine_BottomLocation, 319);
		hsLineValue->setValue(vMonsterLine_TopLocation);
		sbLineValue->setValue(vMonsterLine_TopLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbMonsterLines_Bottom_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VMONSTERBOTTOM;
		updateLines();
		hsLineValue->setRange(vMonsterLine_Spacing, vMonsterLine_TopLocation);
		sbLineValue->setRange(vMonsterLine_Spacing, vMonsterLine_TopLocation);
		hsLineValue->setValue(vMonsterLine_BottomLocation);
		sbLineValue->setValue(vMonsterLine_BottomLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbMonsterLines_Spacing_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VMONSTERSPACING;
		updateLines();
		hsLineValue->setRange(0, vMonsterLine_BottomLocation);
		sbLineValue->setRange(0, vMonsterLine_BottomLocation);
		hsLineValue->setValue(vMonsterLine_Spacing);
		sbLineValue->setValue(vMonsterLine_Spacing);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbCharacterLines_Top_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VCHARACTERTOP;
		updateLines();
		hsLineValue->setRange(321, vCharacterLine_BottomLocation);
		sbLineValue->setRange(321, vCharacterLine_BottomLocation);
		hsLineValue->setValue(vCharacterLine_TopLocation);
		sbLineValue->setValue(vCharacterLine_TopLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbCharacterLines_Bottom_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VCHARACTERBOTTOM;
		updateLines();
		hsLineValue->setRange(vCharacterLine_TopLocation, 639 - vCharacterLine_Spacing);
		sbLineValue->setRange(vCharacterLine_TopLocation, 639 - vCharacterLine_Spacing);
		hsLineValue->setValue(vCharacterLine_BottomLocation);
		hsLineValue->setValue(vCharacterLine_BottomLocation);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_rbCharacterLines_Spacing_toggled(bool on)
{
	if (on && !ignoreEvents)
	{
		ignoreEvents = true;
		selectedLine = LINE_VCHARACTERSPACING;
		updateLines();
		hsLineValue->setRange(0, 639 - vCharacterLine_BottomLocation);
		sbLineValue->setRange(0, 639 - vCharacterLine_BottomLocation);
		hsLineValue->setValue(vCharacterLine_Spacing);
		sbLineValue->setValue(vCharacterLine_Spacing);
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_hsLineValue_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		sbLineValue->setValue(value);
		if (selectedLine == LINE_HTOP)
			hTopLineLocation = value;
		else if (selectedLine == LINE_HBOTTOM)
			hBottomLineLocation = value;
		else if (selectedLine == LINE_VMONSTERTOP)
			vMonsterLine_TopLocation = value;
		else if (selectedLine == LINE_VMONSTERBOTTOM)
			vMonsterLine_BottomLocation = value;
		else if (selectedLine == LINE_VMONSTERSPACING)
			vMonsterLine_Spacing = value;
		else if (selectedLine == LINE_VCHARACTERTOP)
			vCharacterLine_TopLocation = value;
		else if (selectedLine == LINE_VCHARACTERBOTTOM)
			vCharacterLine_BottomLocation = value;
		else if (selectedLine == LINE_VCHARACTERSPACING)
			vCharacterLine_Spacing = value;
		updateLines();
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_sbLineValue_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		hsLineValue->setValue(value);
		if (selectedLine == LINE_HTOP)
			hTopLineLocation = value;
		else if (selectedLine == LINE_HBOTTOM)
			hBottomLineLocation = value;
		else if (selectedLine == LINE_VMONSTERTOP)
			vMonsterLine_TopLocation = value;
		else if (selectedLine == LINE_VMONSTERBOTTOM)
			vMonsterLine_BottomLocation = value;
		else if (selectedLine == LINE_VMONSTERSPACING)
			vMonsterLine_Spacing = value;
		else if (selectedLine == LINE_VCHARACTERTOP)
			vCharacterLine_TopLocation = value;
		else if (selectedLine == LINE_VCHARACTERBOTTOM)
			vCharacterLine_BottomLocation = value;
		else if (selectedLine == LINE_VCHARACTERSPACING)
			vCharacterLine_Spacing = value;
		updateLines();
		ignoreEvents = false;
	}
}

void ConfigureCustomGridDepth::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Database::Terrain::GridDepth");
}

void ConfigureCustomGridDepth::updateLines()
{
	QPointF p1, p2;
	if (selectedLine == LINE_HTOP)
	{
		hLine_Top->setPen(selectedLinePen);
		hLine_Top->setZValue(2.0);
	}
	else
	{
		hLine_Top->setPen(regularLinePen);
		hLine_Top->setZValue(1.0);
	}
	p1.setX(0.0);
	p1.setY((float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY((float)hTopLineLocation);
	hLine_Top->setLine(QLineF(p1, p2));
	hLine_CenterTop->setPen(regularLinePen);
	p1.setX(0.0);
	p1.setY((((float)hBottomLineLocation - (float)hTopLineLocation) / 3.0) + (float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY((((float)hBottomLineLocation - (float)hTopLineLocation) / 3.0) + (float)hTopLineLocation);
	hLine_CenterTop->setLine(QLineF(p1, p2));
	hLine_CenterBottom->setPen(regularLinePen);
	p1.setX(0.0);
	p1.setY(((((float)hBottomLineLocation - (float)hTopLineLocation) * 2.0) / 3.0) + (float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY(((((float)hBottomLineLocation - (float)hTopLineLocation) * 2.0) / 3.0) + (float)hTopLineLocation);
	hLine_CenterBottom->setLine(QLineF(p1, p2));
	if (selectedLine == LINE_HBOTTOM)
	{
		hLine_Bottom->setPen(selectedLinePen);
		hLine_Bottom->setZValue(2.0);
	}
	else
	{
		hLine_Bottom->setPen(regularLinePen);
		hLine_Bottom->setZValue(1.0);
	}
	p1.setX(0.0);
	p1.setY((float)hBottomLineLocation);
	p2.setX(640.0);
	p2.setY((float)hBottomLineLocation);
	hLine_Bottom->setLine(QLineF(p1, p2));
	p1.setX((float)vMonsterLine_TopLocation - (float)vMonsterLine_Spacing);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vMonsterLine_BottomLocation - (float)vMonsterLine_Spacing);
	p2.setY((float)hBottomLineLocation);
	vLine_Monsters_Left->setLine(QLineF(p1, p2));
	vLine_Monsters_Right->setPen(regularLinePen);
	p1.setX((float)vMonsterLine_TopLocation);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vMonsterLine_BottomLocation);
	p2.setY((float)hBottomLineLocation);
	vLine_Monsters_Right->setLine(QLineF(p1, p2));
	if (selectedLine == LINE_VMONSTERSPACING)
	{
		vLine_Monsters_SpacingTop->setPen(selectedLinePen);
		vLine_Monsters_SpacingCenterTop->setPen(selectedLinePen);
		vLine_Monsters_SpacingCenterBottom->setPen(selectedLinePen);
		vLine_Monsters_SpacingBottom->setPen(selectedLinePen);
		vLine_Monsters_SpacingTop->setZValue(2.0);
		vLine_Monsters_SpacingCenterTop->setZValue(2.0);
		vLine_Monsters_SpacingCenterBottom->setZValue(2.0);
		vLine_Monsters_SpacingBottom->setZValue(2.0);
	}
	else
	{
		vLine_Monsters_SpacingTop->setPen(regularLinePen);
		vLine_Monsters_SpacingCenterTop->setPen(regularLinePen);
		vLine_Monsters_SpacingCenterBottom->setPen(regularLinePen);
		vLine_Monsters_SpacingBottom->setPen(regularLinePen);
		vLine_Monsters_SpacingTop->setZValue(1.0);
		vLine_Monsters_SpacingCenterTop->setZValue(1.0);
		vLine_Monsters_SpacingCenterBottom->setZValue(1.0);
		vLine_Monsters_SpacingBottom->setZValue(1.0);
	}
	vLine_Monsters_Left->line().intersect(hLine_Top->line(), &p1);
	vLine_Monsters_Right->line().intersect(hLine_Top->line(), &p2);
	vLine_Monsters_SpacingTop->setLine(QLineF(p1, p2));
	vLine_Monsters_Left->line().intersect(hLine_CenterTop->line(), &p1);
	vLine_Monsters_Right->line().intersect(hLine_CenterTop->line(), &p2);
	vLine_Monsters_SpacingCenterTop->setLine(QLineF(p1, p2));
	vLine_Monsters_Left->line().intersect(hLine_CenterBottom->line(), &p1);
	vLine_Monsters_Right->line().intersect(hLine_CenterBottom->line(), &p2);
	vLine_Monsters_SpacingCenterBottom->setLine(QLineF(p1, p2));
	vLine_Monsters_Left->line().intersect(hLine_Bottom->line(), &p1);
	vLine_Monsters_Right->line().intersect(hLine_Bottom->line(), &p2);
	vLine_Monsters_SpacingBottom->setLine(QLineF(p1, p2));
	p1.setX(320.0);
	p1.setY((float)hTopLineLocation);
	p2.setX(320.0);
	p2.setY((float)hBottomLineLocation);
	vLine_Center->setLine(QLineF(p1, p2));
	p1.setX((float)vCharacterLine_TopLocation);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vCharacterLine_BottomLocation);
	p2.setY((float)hBottomLineLocation);
	vLine_Characters_Left->setLine(QLineF(p1, p2));
	p1.setX((float)vCharacterLine_TopLocation + (float)vCharacterLine_Spacing);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vCharacterLine_BottomLocation + (float)vCharacterLine_Spacing);
	p2.setY((float)hBottomLineLocation);
	vLine_Characters_Right->setLine(QLineF(p1, p2));
	if (selectedLine == LINE_VCHARACTERSPACING)
	{
		vLine_Characters_SpacingTop->setPen(selectedLinePen);
		vLine_Characters_SpacingCenterTop->setPen(selectedLinePen);
		vLine_Characters_SpacingCenterBottom->setPen(selectedLinePen);
		vLine_Characters_SpacingBottom->setPen(selectedLinePen);
		vLine_Characters_SpacingTop->setZValue(2.0);
		vLine_Characters_SpacingCenterTop->setZValue(2.0);
		vLine_Characters_SpacingCenterBottom->setZValue(2.0);
		vLine_Characters_SpacingBottom->setZValue(2.0);
	}
	else
	{
		vLine_Characters_SpacingTop->setPen(regularLinePen);
		vLine_Characters_SpacingCenterTop->setPen(regularLinePen);
		vLine_Characters_SpacingCenterBottom->setPen(regularLinePen);
		vLine_Characters_SpacingBottom->setPen(regularLinePen);
		vLine_Characters_SpacingTop->setZValue(1.0);
		vLine_Characters_SpacingCenterTop->setZValue(1.0);
		vLine_Characters_SpacingCenterBottom->setZValue(1.0);
		vLine_Characters_SpacingBottom->setZValue(1.0);
	}
	vLine_Characters_Left->line().intersect(hLine_Top->line(), &p1);
	vLine_Characters_Right->line().intersect(hLine_Top->line(), &p2);
	vLine_Characters_SpacingTop->setLine(QLineF(p1, p2));
	vLine_Characters_Left->line().intersect(hLine_CenterTop->line(), &p1);
	vLine_Characters_Right->line().intersect(hLine_CenterTop->line(), &p2);
	vLine_Characters_SpacingCenterTop->setLine(QLineF(p1, p2));
	vLine_Characters_Left->line().intersect(hLine_CenterBottom->line(), &p1);
	vLine_Characters_Right->line().intersect(hLine_CenterBottom->line(), &p2);
	vLine_Characters_SpacingCenterBottom->setLine(QLineF(p1, p2));
	vLine_Characters_Left->line().intersect(hLine_Bottom->line(), &p1);
	vLine_Characters_Right->line().intersect(hLine_Bottom->line(), &p2);
	vLine_Characters_SpacingBottom->setLine(QLineF(p1, p2));
	if (selectedLine == LINE_VMONSTERTOP)
	{
		p1.setX((float)vMonsterLine_TopLocation - 8.0);
		p1.setY((float)hTopLineLocation - 8.0);
		ellipseItem->setRect(p1.x(), p1.y(), 16.0, 16.0);
		ellipseItem->setZValue(2.0);
		ellipseItem->setVisible(true);
	}
	else if (selectedLine == LINE_VMONSTERBOTTOM)
	{
		p1.setX((float)vMonsterLine_BottomLocation - 8.0);
		p1.setY((float)hBottomLineLocation - 8.0);
		ellipseItem->setRect(p1.x(), p1.y(), 16.0, 16.0);
		ellipseItem->setZValue(2.0);
		ellipseItem->setVisible(true);
	}
	else if (selectedLine == LINE_VCHARACTERTOP)
	{
		p1.setX((float)vCharacterLine_TopLocation - 8.0);
		p1.setY((float)hTopLineLocation - 8.0);
		ellipseItem->setRect(p1.x(), p1.y(), 16.0, 16.0);
		ellipseItem->setZValue(2.0);
		ellipseItem->setVisible(true);
	}
	else if (selectedLine == LINE_VCHARACTERBOTTOM)
	{
		p1.setX((float)vCharacterLine_BottomLocation - 8.0);
		p1.setY((float)hBottomLineLocation - 8.0);
		ellipseItem->setRect(p1.x(), p1.y(), 16.0, 16.0);
		ellipseItem->setZValue(2.0);
		ellipseItem->setVisible(true);
	}
	else
	{
		ellipseItem->setRect(0.0, 0.0, 16.0, 16.0);
		ellipseItem->setZValue(1.0);
		ellipseItem->setVisible(false);
	}
}
