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

#ifndef SPINBOXSIDEBUTTON_H
#define SPINBOXSIDEBUTTON_H

class QSpinBox;
class QDoubleSpinBox;

#include <QToolButton>

class SpinBoxSideButton : public QToolButton
{
	Q_OBJECT
	public:
		SpinBoxSideButton(QWidget *parent=0);
		~SpinBoxSideButton();
		void setSpinBox(QSpinBox *box);
		void setDoubleSpinBox(QDoubleSpinBox *box);
	protected:
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
	private:
		QSpinBox *spinBox;
		QDoubleSpinBox *doubleSpinBox;
		int previousY;
		bool processMovement;
};

#endif // SPINBOXSIDEBUTTON_H
