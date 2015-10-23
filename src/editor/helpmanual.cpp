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

#include <QFileInfo>
#include <QWebHistory>
#include "helpmanual.h"
#include "xmlParser.h"

HelpManual::HelpManual(QString folderLocation, QWidget *parent) : QDialog(parent)
{
	XMLNode TempNode1, TempNode2, IndexNode = XMLNode::openFileHelper(QString(folderLocation + "/helpmanual.xml").toLocal8Bit().data(), "openrpgmaker-helpmanual");
	QTreeWidgetItem *item;
	setupUi(this);
	tabWidget->setCurrentIndex(2);
	tabWidget->resize(tabWidget->sizeHint());
	splitter->resize(splitter->sizeHint());
	tabWidget->setCurrentIndex(0);
	helpManualFolder = QString("file://") + folderLocation;
	webView->setUrl(QUrl(helpManualFolder + "/index.html"));
	bBack->setEnabled(false);
	bForward->setEnabled(false);
	TempNode1 = IndexNode.getChildNode("indexList");
	for (int i = 0; i < TempNode1.nChildNode("index"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("index", i);
		lwIndexList->addItem(TempNode2.getAttribute("name"));
		indexList.insert(TempNode2.getAttribute("name"), TempNode2.getAttribute("file"));
	}
	TempNode1 = IndexNode.getChildNode("contentTree");
	for (int i = 0; i < TempNode1.nChildNode("item"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("item", i);
		item = new QTreeWidgetItem();
		item->setText(0, TempNode2.getAttribute("name"));
		item->setText(1, TempNode2.getAttribute("file"));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		twContents->addTopLevelItem(item);
		buildContentTree(item, TempNode2);
	}
	lwIndexList->sortItems();
}

HelpManual::~HelpManual()
{
}

void HelpManual::loadPage(QString file)
{
	webView->setUrl(QUrl(helpManualFolder + file));
}

void HelpManual::loadIndex(QString index)
{
	if (indexList.find(index) != indexList.end())
		webView->setUrl(QUrl(helpManualFolder + indexList[index]));
	else
		webView->setUrl(QUrl(helpManualFolder + "/unknown.html"));
}

void HelpManual::on_twContents_itemClicked(QTreeWidgetItem *item)
{
	QUrl fileLocation = QUrl(helpManualFolder + item->text(1));
	if (QFileInfo(fileLocation.path()).exists())
		webView->setUrl(fileLocation);
	else
		webView->setUrl(QUrl(helpManualFolder + "/unknown.html"));
}

void HelpManual::on_leIndexSearch_textChanged(QString text)
{
	QMap<QString, QString>::iterator iter;
	lwIndexList->clear();
	for (iter = indexList.begin(); iter != indexList.end(); ++iter)
	{
		if (iter.key().startsWith(text, Qt::CaseInsensitive))
			lwIndexList->addItem(iter.key());
	}
	lwIndexList->sortItems();
}

void HelpManual::on_lwIndexList_itemClicked(QListWidgetItem *item)
{
	webView->setUrl(QUrl(helpManualFolder + indexList[item->text()]));
}

void HelpManual::on_webView_urlChanged()
{
	bBack->setEnabled(webView->history()->canGoBack());
	bForward->setEnabled(webView->history()->canGoForward());
}

void HelpManual::on_twContents_customContextMenuRequested()
{
	XMLNode TempNode1, TempNode2, IndexNode = XMLNode::openFileHelper(QString(QUrl(helpManualFolder).path() + "/helpmanual.xml").toLocal8Bit().data(), "openrpgmaker-helpmanual");
	QTreeWidgetItem *item;
	twContents->clear();
	TempNode1 = IndexNode.getChildNode("contentTree");
	for (int i = 0; i < TempNode1.nChildNode("item"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("item", i);
		item = new QTreeWidgetItem();
		item->setText(0, TempNode2.getAttribute("name"));
		item->setText(1, TempNode2.getAttribute("file"));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		twContents->addTopLevelItem(item);
		buildContentTree(item, TempNode2);
	}
}

void HelpManual::on_lwIndexList_customContextMenuRequested()
{
	XMLNode TempNode1, TempNode2, IndexNode = XMLNode::openFileHelper(QString(QUrl(helpManualFolder).path() + "/helpmanual.xml").toLocal8Bit().data(), "openrpgmaker-helpmanual");
	indexList.clear();
	lwIndexList->clear();
	TempNode1 = IndexNode.getChildNode("indexList");
	for (int i = 0; i < TempNode1.nChildNode("index"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("index", i);
		lwIndexList->addItem(TempNode2.getAttribute("name"));
		indexList.insert(TempNode2.getAttribute("name"), TempNode2.getAttribute("file"));
	}
	lwIndexList->sortItems();
}

void HelpManual::buildContentTree(QTreeWidgetItem *parent, XMLNode TreeNode)
{
	XMLNode TempNode;
	QTreeWidgetItem *item;
	for (int i = 0; i < TreeNode.nChildNode("item"); ++i)
	{
		TempNode = TreeNode.getChildNode("item", i);
		item = new QTreeWidgetItem();
		item->setText(0, TempNode.getAttribute("name"));
		item->setText(1, TempNode.getAttribute("file"));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		parent->addChild(item);
		buildContentTree(item, TempNode);
	}
}
