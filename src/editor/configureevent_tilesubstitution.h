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

#ifndef CONFIGUREEVENT_TILESUBSTITUTION_H
#define CONFIGUREEVENT_TILESUBSTITUTION_H

#include "ui_configureevent_tilesubstitution.h"

class ConfigureEvent_TileSubstitution : public QDialog, public Ui::ConfigureEvent_TileSubstitution
{
	Q_OBJECT
	public:
		enum Layer {ALL=0, LOWER, HERO, UPPER};
		ConfigureEvent_TileSubstitution(QWidget *parent=0);
		~ConfigureEvent_TileSubstitution();
		int getLayer();
		void setLayer(int value);
		int getTileset1ID();
		int getTile1ID();
		void setTile1(int tilesetID, int tileID);
		int getTileset2ID();
		int getTile2ID();
		void setTile2(int tilesetID, int tileID);
		bool isTile1ReferencedInVariables();
		void setTile1ReferencedInVariables(bool referenced);
		bool isTile2ReferencedInVariables();
		void setTile2ReferencedInVariables(bool referenced);
	protected slots:
		void on_bBrowseTile1Variables_clicked();
		void on_bBrowseTile2Variables_clicked();
		void on_bBrowseTileset1Variables_clicked();
		void on_bBrowseTileset2Variables_clicked();
		void on_rbSpecificTile1_toggled(bool checked);
		void on_rbSpecificTile2_toggled(bool checked);
};

#endif // CONFIGUREEVENT_TILESUBSTITUTION_H
