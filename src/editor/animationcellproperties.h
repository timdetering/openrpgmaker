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

#ifndef ANIMATIONCELLPROPERTIES_H
#define ANIMATIONCELLPROPERTIES_H

#include <QGraphicsColorizeEffect>
#include "ui_animationcellproperties.h"
#include "colorbutton.h"
#include "mygraphicstinteffect.h"

class AnimationCellProperties : public QDialog, public Ui::AnimationCellProperties
{
	Q_OBJECT
	public:
		AnimationCellProperties(QStringList images, int imageID, int scale, int rotation, int opacity, QColor tintColor, int tintAmount, QWidget *parent=0);
		~AnimationCellProperties();
		int getImageID();
		void setImageID(int imageID);
		int getScale();
		void setScale(int scale);
		int getRotation();
		void setRotation(int rotation);
		int getOpacity();
		void setOpacity(int opacity);
		QColor getTintColor();
		void setTintColor(QColor color);
		int getTintAmount();
		void setTintAmount(int tintAmount);
	protected slots:
		void on_bSetTintColor_colorChanged(QColor color);
		void on_sbImageID_valueChanged();
		void on_sbOpacity_valueChanged(int value);
		void on_sbRotation_valueChanged(int value);
		void on_sbScale_valueChanged(int value);
		void on_sbTintAmount_valueChanged(int value);
	private:
		QGraphicsScene *previewScene;
		QGraphicsPixmapItem *previewItem;
		MyGraphicsTintEffect *previewTintEffect;
		QStringList imageLocations;
};

#endif // ANIMATIONCELLPROPERTIES_H
