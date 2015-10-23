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

#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include "menuobject.h"
#include "menupreview.h"
#include "menuscrollarea.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"

MenuPreview::MenuPreview(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

MenuPreview::~MenuPreview()
{
}

void MenuPreview::setupPreview(int menuID, int menuType)
{
	QPainter painter;
	if (menuType == MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		lblPreview->setMinimumSize(640, 480);
		lblPreview->setMaximumSize(640, 480);
		resize(sizeHint());
		setMinimumSize(width(), height());
		setMaximumSize(width(), height());
		menuPixmap = QPixmap(640, 480);
		menuPixmap.fill(QColor(255, 0, 255));
		painter.begin(&menuPixmap);
		if (ProjectData::systemGraphics.stretchToFit)
			painter.drawTiledPixmap(0, 0, 640, 480, QPixmap(ProjectData::systemGraphics.menuBGLocation));
		else
			painter.drawPixmap(0, 0, QPixmap(ProjectData::systemGraphics.menuBGLocation).scaled(640, 480));
		for (int i = 0; i < (*layoutRef)->numTopLevelObjects(); ++i)
			renderObject((*layoutRef)->getTopLevelObject(i), painter);
		painter.end();
		lblPreview->setPixmap(menuPixmap);
		delete layoutRef;
		layoutRef = NULL;
	}
	else if (menuType == MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		int menuWidth = (*layoutRef)->getWidth();
		int menuHeight = (*layoutRef)->getHeight();
		lblPreview->setMinimumSize(menuWidth, menuHeight);
		lblPreview->setMaximumSize(menuWidth, menuHeight);
		resize(sizeHint());
		setMinimumSize(width(), height());
		setMaximumSize(width(), height());
		menuPixmap = QPixmap(menuWidth, menuHeight);
		menuPixmap.fill(QColor(255, 0, 255));
		painter.begin(&menuPixmap);
		for (int i = 0; i < (*layoutRef)->numTopLevelObjects(); ++i)
			renderObject((*layoutRef)->getTopLevelObject(i), painter);
		painter.end();
		lblPreview->setPixmap(menuPixmap);
		delete layoutRef;
		layoutRef = NULL;
	}
	else if (menuType == MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		lblPreview->setMinimumSize(640, 480);
		lblPreview->setMaximumSize(640, 480);
		resize(sizeHint());
		setMinimumSize(width(), height());
		setMaximumSize(width(), height());
		menuPixmap = QPixmap(640, 480);
		menuPixmap.fill(QColor(255, 0, 255));
		painter.begin(&menuPixmap);
		if (ProjectData::titleLocation.isEmpty())
		{
			if (ProjectData::systemGraphics.stretchToFit)
				painter.drawTiledPixmap(0, 0, 640, 480, QPixmap(ProjectData::systemGraphics.menuBGLocation));
			else
				painter.drawPixmap(0, 0, QPixmap(ProjectData::systemGraphics.menuBGLocation).scaled(640, 480));
		}
		else
			painter.drawPixmap(0, 0, QPixmap(ProjectData::titleLocation));
		for (int i = 0; i < layout->numTopLevelObjects(); ++i)
			renderObject(layout->getTopLevelObject(i), painter);
		painter.end();
		lblPreview->setPixmap(menuPixmap);
		layout = NULL;
	}
}

void MenuPreview::on_buttonBox_clicked(QAbstractButton *button)
{
	if (buttonBox->standardButton(button) == QDialogButtonBox::Save)
	{
		QString filelocation = QFileDialog::getSaveFileName(0, "Save Location", QDir::homePath(), "PNG Images (*.png)");
		if (!filelocation.isEmpty())
			menuPixmap.save(filelocation);
	}
}

void MenuPreview::renderObject(MenuObject *object, QPainter &painter)
{
	QPixmap objectPixmap;
	QPainter objectPainter;
	if (object->getType() == MenuObject::TYPE_SELECTABLEAREA || object->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		objectPixmap = QPixmap(object->getWidth(), object->getHeight());
		objectPixmap.fill(QColor(0, 0, 0, 0));
	}
	else
		objectPixmap = QPixmap(object->getRenderedPixmap());
	if (object->numChildObjects() > 0)
	{
		objectPainter.begin(&objectPixmap);
		for (int i = 0; i < object->numChildObjects(); ++i)
			renderObject(object->getChildObject(i), objectPainter);
		objectPainter.end();
	}
	if (object->getParent() == NULL)
		painter.drawPixmap(object->getX(), object->getY(), objectPixmap);
	else if (object->getParent()->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		MenuScrollArea *scrollArea = (MenuScrollArea*)(object->getParent());
		painter.drawPixmap(object->getX() - scrollArea->getCurrentScrollX(), object->getY() - scrollArea->getCurrentScrollY(), objectPixmap);
	}
	else
		painter.drawPixmap(object->getX(), object->getY(), objectPixmap);
}
