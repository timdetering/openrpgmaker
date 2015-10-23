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

#ifndef CONFIGUREEVENT_CHANGEBORDERTILE_H
#define CONFIGUREEVENT_CHANGEBORDERTILE_H

#include "ui_configureevent_changebordertile.h"

class ConfigureEvent_ChangeBorderTile : public QDialog, public Ui::ConfigureEvent_ChangeBorderTile
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeBorderTile(QWidget *parent=0);
		~ConfigureEvent_ChangeBorderTile();
		int getTilesetID();
		int getTileID();
		void setTile(int tilesetID, int tileID);
		bool isTileReferencedInVariables();
		void setTileReferencedInVariables(bool tileReferencedInVariables);
	protected slots:
		void on_bBrowseTileVariables_clicked();
		void on_bBrowseTilesetVariables_clicked();
		void on_rbSpecificTile_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEBORDERTILE_H
