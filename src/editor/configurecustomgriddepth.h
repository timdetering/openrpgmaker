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

#ifndef CONFIGURECUSTOMGRIDDEPTH_H
#define CONFIGURECUSTOMGRIDDEPTH_H

class Terrain;

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "ui_configurecustomgriddepth.h"
#include "projectdata_terrain.h"

class ConfigureCustomGridDepth : public QDialog, public Ui::ConfigureCustomGridDepth
{
	Q_OBJECT
	public:
		ConfigureCustomGridDepth(Terrain::GridDepthValues *gridDepth, QString backgroundLocation, QWidget *parent=0);
		~ConfigureCustomGridDepth();
		void applyValues(Terrain::GridDepthValues *gridDepth);
		enum SelectedLine
		{
			LINE_HTOP=0,
			LINE_HBOTTOM,
			LINE_VMONSTERTOP,
			LINE_VMONSTERBOTTOM,
			LINE_VMONSTERSPACING,
			LINE_VCHARACTERTOP,
			LINE_VCHARACTERBOTTOM,
			LINE_VCHARACTERSPACING
		};
	protected slots:
		void on_rbTopLine_toggled(bool on);
		void on_rbBottomLine_toggled(bool on);
		void on_rbMonsterLines_Top_toggled(bool on);
		void on_rbMonsterLines_Bottom_toggled(bool on);
		void on_rbMonsterLines_Spacing_toggled(bool on);
		void on_rbCharacterLines_Top_toggled(bool on);
		void on_rbCharacterLines_Bottom_toggled(bool on);
		void on_rbCharacterLines_Spacing_toggled(bool on);
		void on_hsLineValue_valueChanged(int value);
		void on_sbLineValue_valueChanged(int value);
		void on_buttonBox_helpRequested();
	private:
		void updateLines();
		QGraphicsScene *previewScene;
		QGraphicsPixmapItem *backgroundItem;
		QGraphicsLineItem *hLine_Top;
		QGraphicsLineItem *hLine_CenterTop;
		QGraphicsLineItem *hLine_CenterBottom;
		QGraphicsLineItem *hLine_Bottom;
		QGraphicsLineItem *vLine_Monsters_Left;
		QGraphicsLineItem *vLine_Monsters_Right;
		QGraphicsLineItem *vLine_Monsters_SpacingTop;
		QGraphicsLineItem *vLine_Monsters_SpacingCenterTop;
		QGraphicsLineItem *vLine_Monsters_SpacingCenterBottom;
		QGraphicsLineItem *vLine_Monsters_SpacingBottom;
		QGraphicsLineItem *vLine_Center;
		QGraphicsLineItem *vLine_Characters_Left;
		QGraphicsLineItem *vLine_Characters_Right;
		QGraphicsLineItem *vLine_Characters_SpacingTop;
		QGraphicsLineItem *vLine_Characters_SpacingCenterTop;
		QGraphicsLineItem *vLine_Characters_SpacingCenterBottom;
		QGraphicsLineItem *vLine_Characters_SpacingBottom;
		QGraphicsEllipseItem *ellipseItem;
		QPen regularLinePen;
		QPen selectedLinePen;
		int hTopLineLocation;
		int hBottomLineLocation;
		int vMonsterLine_TopLocation;
		int vMonsterLine_BottomLocation;
		int vMonsterLine_Spacing;
		int vCharacterLine_TopLocation;
		int vCharacterLine_BottomLocation;
		int vCharacterLine_Spacing;
		int selectedLine;
		bool ignoreEvents;
};

#endif // CONFIGURECUSTOMGRIDDEPTH_H
