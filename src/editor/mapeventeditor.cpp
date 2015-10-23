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

#include <QTabWidget>
#include <QDialog>
#include "clipboard.h"
#include "mapeventeditor.h"
#include "mapeventpageeditor.h"
#include "projectdata.h"
#include "projectdata_map.h"

MapEventEditor::MapEventEditor(int mapID, int eventID, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	mapRef->saveToStorage();
	mapRef->backupStorage();
	event = (*mapRef)->getEvent(eventID);
	leName->setText(event->getName());
	for (int i = 0; i < event->numPages(); ++i)
	{
		pageWidgets.append(new MapEventPageEditor(this));
		tabsPages->addTab(pageWidgets[i], QString::number(i + 1));
		pageWidgets[i]->setupEditor(mapID, event, i);
		QObject::connect(pageWidgets[i], SIGNAL(eventCopied()), this, SLOT(eventCopied()));
	}
	if (pageWidgets.size() > 1)
		bDeletePage->setEnabled(true);
	else
		bDeletePage->setEnabled(false);
	if (pageWidgets.size() < 99)
		bNewPage->setEnabled(true);
	else
		bNewPage->setEnabled(false);
	if (Clipboard::dataType == Clipboard::TYPE_MAPEVENTPAGE && pageWidgets.size() < 99)
		bPastePage->setEnabled(true);
	else
		bPastePage->setEnabled(false);
}

MapEventEditor::~MapEventEditor()
{
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
}

void MapEventEditor::on_leName_textChanged(QString text)
{
	event->setName(text);
}

void MapEventEditor::on_bNewPage_clicked()
{
	int pageID = pageWidgets.size();
	event->addPage(new MapEvent::Page());
	pageWidgets.append(new MapEventPageEditor(this));
	tabsPages->addTab(pageWidgets[pageID], QString::number(pageID + 1));
	pageWidgets[pageID]->setupEditor(mapRef->getDataID(), event, pageID);
	QObject::connect(pageWidgets[pageID], SIGNAL(eventCopied()), this, SLOT(eventCopied()));
	if (pageWidgets.size() == 99)
	{
		bNewPage->setEnabled(false);
		bPastePage->setEnabled(false);
	}
	bDeletePage->setEnabled(true);
	tabsPages->setCurrentIndex(pageID);
}

void MapEventEditor::on_bDeletePage_clicked()
{
	int pageID = tabsPages->currentIndex();
	QObject::disconnect(pageWidgets[pageID], SIGNAL(eventCopied()), this, SLOT(eventCopied()));
	tabsPages->removeTab(pageID);
	delete (pageWidgets[pageID]);
	pageWidgets.removeAt(pageID);
	event->deletePage(pageID);
	for (int i = 0; i < pageWidgets.size(); ++i)
		tabsPages->setTabText(i, QString::number(i + 1));
	if (pageWidgets.size() == 1)
		bDeletePage->setEnabled(false);
	bNewPage->setEnabled(true);
	if (Clipboard::dataType == Clipboard::TYPE_MAPEVENTPAGE)
		bPastePage->setEnabled(true);
}

void MapEventEditor::on_bCopyPage_clicked()
{
	int pageID = tabsPages->currentIndex();
	Clipboard::clear();
	Clipboard::dataType = Clipboard::TYPE_MAPEVENTPAGE;
	Clipboard::data.mapEventPage = new MapEvent::Page(event->getPage(pageID));
	if (pageWidgets.size() < 99)
		bPastePage->setEnabled(true);
}

void MapEventEditor::on_bPastePage_clicked()
{
	int pageID = pageWidgets.size();
	event->addPage(new MapEvent::Page(Clipboard::data.mapEventPage));
	pageWidgets.append(new MapEventPageEditor(this));
	tabsPages->addTab(pageWidgets[pageID], QString::number(pageID + 1));
	pageWidgets[pageID]->setupEditor(mapRef->getDataID(), event, pageID);
	QObject::connect(pageWidgets[pageID], SIGNAL(eventCopied()), this, SLOT(eventCopied()));
	if (pageWidgets.size() == 99)
	{
		bNewPage->setEnabled(false);
		bPastePage->setEnabled(false);
	}
	bDeletePage->setEnabled(true);
	tabsPages->setCurrentIndex(pageID);
}

void MapEventEditor::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	for (int i = 0; i < pageWidgets.size(); ++i)
		pageWidgets[i]->applyChanges();
	if (buttonClicked == QDialogButtonBox::Ok)
		mapRef->deleteBackupStorage();
	else if (buttonClicked == QDialogButtonBox::Apply)
		mapRef->backupStorage();
	else if (buttonClicked == QDialogButtonBox::Cancel)
		mapRef->reloadBackupStorage();
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("MapEvent");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void MapEventEditor::eventCopied()
{
	bPastePage->setEnabled(false);
}
