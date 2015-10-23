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

#ifndef TILECHOOSER_H
#define TILECHOOSER_H

#include "ui_tilechooser.h"
#include "projectdata.h"

class TileChooser : public QDialog, public Ui::TileChooser
{
	Q_OBJECT
	public:
		TileChooser(QWidget *parent=0);
		~TileChooser();
		void setupTileChooser(unsigned short currentTile, int selectWidth, int selectHeight);
		unsigned short getSelectedTile(int xOffset=0, int yOffset=0);
	protected slots:
		void on_cboxTilesets_currentIndexChanged(int index);
		void on_gvTileView_mouseButtonPressed(QMouseEvent *event, int tileID);
		void on_gvTileView_scrollBarRangeChanged(int min, int max);
		void on_gvTileView_scrollBarValueChanged(int value);
	private:
		void updateTilesetList();
		QGraphicsRectItem *tileSelection;
		QList<ProjectData::DataReference<Tileset>*> tilesets;
		int currentTilesetID;
		int currentTileID;
		int numTilesets;
		int numTiles;
		int selectionWidth;
		int selectionHeight;
		bool ignoreEvents;
};

#endif // TILECHOOSER_H
