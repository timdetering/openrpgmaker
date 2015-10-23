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

#ifndef TILEBUTTON_H
#define TILEBUTTON_H

#include <QToolButton>
#include "projectdata.h"
#include "tilechooser.h"

class TileButton : public QToolButton
{
	Q_OBJECT
	public:
		TileButton(QWidget *parent=0);
		~TileButton();
		unsigned short getTile();
	signals:
		void tileChanged(unsigned short value);
	public slots:
		void setTile(unsigned short value);
	protected slots:
		void buttonClicked();
	private:
		unsigned short tile;
};

#endif // TILEBUTTON_H
