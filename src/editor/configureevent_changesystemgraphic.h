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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef CONFIGUREEVENT_CHANGESYSTEMGRAPHIC_H
#define CONFIGUREEVENT_CHANGESYSTEMGRAPHIC_H

#include "ui_configureevent_changesystemgraphic.h"

class ConfigureEvent_ChangeSystemGraphic : public QDialog, public Ui::ConfigureEvent_ChangeSystemGraphic
{
	Q_OBJECT
	public:
		enum GraphicToChange {FRAME_BG, FRAME_BORDER, SELECTION_BORDER, EQUIPMENT_INDICATORS, MENU_BG, SCROLL_ARROWS, GRAPPLE_ARM, GRAPPLE_HOOK, TIMER_NUMBERS}; 
		ConfigureEvent_ChangeSystemGraphic(QWidget *parent=0);
		~ConfigureEvent_ChangeSystemGraphic();
		void setGraphicToChange(int graphic);
		void setGraphicLocation(QString location);
		int getGraphicToChange();
		QString getGraphicLocation();
	protected slots:
		void on_bBrowseGraphics_clicked();
		void on_cboxSystemGraphic_currentIndexChanged(int index);
	private:
		QString graphicLocation;
};

#endif // CONFIGUREEVENT_CHANGESYSTEMGRAPHIC_H
