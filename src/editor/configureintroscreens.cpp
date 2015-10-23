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

#include <QDialog>
#include <QMessageBox>
#include "configureintroscreeneffect.h"
#include "configureintroscreens.h"
#include "projectdata.h"

ConfigureIntroScreens::ConfigureIntroScreens(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

ConfigureIntroScreens::~ConfigureIntroScreens()
{
}

void ConfigureIntroScreens::on_bAddEffect_clicked()
{
}

void ConfigureIntroScreens::on_bAddScreen_clicked()
{
}

void ConfigureIntroScreens::on_bBottomBGColor_colorChanged(QColor color)
{
}

void ConfigureIntroScreens::on_bConfigureEffect_clicked()
{
}

void ConfigureIntroScreens::on_bDeleteEffect_clicked()
{
}

void ConfigureIntroScreens::on_bDeleteScreen_clicked()
{
}

void ConfigureIntroScreens::on_bMoveEffectDown_clicked()
{
}

void ConfigureIntroScreens::on_bMoveEffectUp_clicked()
{
}

void ConfigureIntroScreens::on_bMoveScreenDown_clicked()
{
}

void ConfigureIntroScreens::on_bMoveScreenUp_clicked()
{
}

void ConfigureIntroScreens::on_bPreviewScreen_clicked()
{
}

void ConfigureIntroScreens::on_bTopBGColor_colorChanged(QColor color)
{
}

void ConfigureIntroScreens::on_buttonBox_accepted()
{
}

void ConfigureIntroScreens::on_leScreenName_textEdited(const QString &text)
{
}

void ConfigureIntroScreens::on_lwEffects_itemSelectionChanged()
{
}

void ConfigureIntroScreens::on_lwScreens_itemSelectionChanged()
{
}
