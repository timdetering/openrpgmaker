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

#include "projectdata.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "tilebutton.h"
#include "tilechooser.h"

TileButton::TileButton(QWidget *parent) : QToolButton(parent)
{
	QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
	tile = 0u;
}

TileButton::~TileButton()
{
}

unsigned short TileButton::getTile()
{
	return tile;
}

void TileButton::setTile(unsigned short value)
{
	QPixmap tileImage = QPixmap(iconSize().width(), iconSize().height());
	QPainter painter;
	ProjectData::DataReference<Tileset> *tilesetRef;
	int tilesetID, tileID;
	tile = value;
	tilesetID = (int)((tile >> 8) % 256);
	tileID = (int)(tile % 256);
	painter.begin(&tileImage);
	painter.fillRect(0, 0, tileImage.width(), tileImage.height(), Qt::magenta);
	for (int y = 0; y < (tileImage.height() / 32); ++y)
	{
		for (int x = 0; x < (tileImage.width() / 32); ++x)
		{
			int tempTileID = tileID + (y * 6) + x;
			if (tilesetID >= ProjectData::tilesetNames.size())
				painter.drawImage(0, 0, QImage(":/icons/invalidtileset.png"));
			else
			{
				tilesetRef = new ProjectData::DataReference<Tileset>(tilesetID, __FILE__, __LINE__);
				if (tempTileID >= (*tilesetRef)->numTiles())
					painter.drawImage(0, 0, QImage(":/icons/invalidtile.png"));
				else
					(*tilesetRef)->getTile(tempTileID)->drawTile(painter, QPoint(x * 32, y * 32));
				delete tilesetRef;
			}
		}
	}
	painter.end();
	setIcon(QIcon(tileImage));
	emit tileChanged(tile);
}

void TileButton::buttonClicked()
{
	TileChooser *tilechooser = new TileChooser;
	tilechooser->setupTileChooser(tile, iconSize().width() / 32, iconSize().height() / 32);
	if (tilechooser->exec())
		setTile(tilechooser->getSelectedTile());
	delete tilechooser;
}
