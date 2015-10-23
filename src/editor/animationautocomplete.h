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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef ANIMATIONAUTOCOMPLETE_H
#define ANIMATIONAUTOCOMPLETE_H

class BattleAnimation;
class GlobalAnimation;

#include "ui_animationautocomplete.h"
#include "projectdata.h"

class AnimationAutoComplete : public QDialog, public Ui::AnimationAutoComplete
{
	Q_OBJECT
	public:
		enum AnimationType {TYPE_BATTLE=0, TYPE_GLOBAL};
		AnimationAutoComplete(int animType, int animID, QWidget *parent=0);
		~AnimationAutoComplete();
		int getStartFrame();
		int getEndFrame();
		int getStartCell();
		int getEndCell();
		bool carryImageID();
		bool carryScale();
		bool carryRotation();
		bool carryOpacity();
		bool carryTintColor();
		bool carryTintAmount();
	protected slots:
		void on_sbStartFrame_valueChanged(int value);
		void on_sbEndFrame_valueChanged(int value);
		void on_sbStartCell_valueChanged(int value);
		void on_sbEndCell_valueChanged(int value);
	private:
		void setupBattleAnimation();
		void setupGlobalAnimation();
		ProjectData::DataReference<BattleAnimation> *battleAnimation;
		ProjectData::DataReference<GlobalAnimation> *globalAnimation;
		bool ignoreEvents;
};

#endif // ANIMATIONAUTOCOMPLETE_H
