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

#include <QWidget>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "configuremapeventmovementpattern.h"
#include "configuremapeventpagespritegraphic.h"
#include "configuremapeventprecondition.h"
#include "mapeventpageeditor.h"
#include "projectdata_mapevent.h"
#include "selectmapevent.h"

QList<int> MapEventPageEditor::movementFrequencyTable = QList<int>() << 250 << 212 << 174 << 136 << 98 << 60;

MapEventPageEditor::MapEventPageEditor(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	animationTimer = new QTimer(this);
	animationTimer->setInterval(movementFrequencyTable[MapEvent::FREQUENCY_NORMAL]);
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateSpriteGraphic()));
	QObject::connect(lwEventCommands, SIGNAL(eventCopied()), this, SIGNAL(eventCopied()));
	scene = new QGraphicsScene(0.0, 0.0, 48.0, 64.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvEventGraphic->setScene(scene);
	spriteItem = new QGraphicsPixmapItem(QPixmap());
	spriteItem->setPos(0.0, 0.0);
	scene->addItem(spriteItem);
	bNumConditionsSideButton->setSpinBox(sbNumConditions);
	bSpriteOpacitySideButton->setSpinBox(sbSpriteOpacity);
	wNumConditions->setEnabled(false);
	bDeletePrecondition->setEnabled(false);
	bConfigureMovementPattern->setEnabled(false);
	wMoveSpeed->setEnabled(false);
	mapEvent = NULL;
	currentPage = NULL;
	spriteFrames << QPointF(-48.0, -128.0);
	currentSpriteFrame = 0;
}

MapEventPageEditor::~MapEventPageEditor()
{
	mapEvent = NULL;
	currentPage = NULL;
}

void MapEventPageEditor::setupEditor(int map, MapEvent *event, int pageID)
{
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(map, __FILE__, __LINE__);
	for (int i = 0; i < (*mapRef)->numLayers(); ++i)
		cboxLayer->addItem((*mapRef)->getLayerName(i));
	mapEvent = event;
	currentPage = mapEvent->getPage(pageID);
	mapID = map;
	lwEventCommands->setupView(map, 0, currentPage->getEvent());
	spriteItem->setPixmap(QPixmap::fromImage(currentPage->getImage()));
	if (currentPage->getSpriteLocation().startsWith("sprite:"))
	{
		animationTimer->stop();
		currentSpriteFrame = 0;
		spriteItem->setPos(spriteFrames[0]);
		animationTimer->start();
	}
	else
		spriteItem->setPos(8.0, 16.0);
	spriteItem->setOpacity(currentPage->getSpriteOpacity() / 100.0);
	sbSpriteOpacity->setValue(currentPage->getSpriteOpacity());
	cboxTriggerCondition->setCurrentIndex(currentPage->getTriggerCondition());
	cboxLayer->setCurrentIndex(currentPage->getLayer());
	cboxRelationToHero->setCurrentIndex(currentPage->getRelationToHero());
	cbForbidEventOverlap->setChecked(currentPage->getForbidEventOverlap());
	cboxAnimationType->setCurrentIndex(currentPage->getAnimationType());
	cboxMovementFrequency->setCurrentIndex(currentPage->getMovementFrequency());
	cboxMovementType->setCurrentIndex(currentPage->getMovementType());
	cboxMovementSpeed->setCurrentIndex(currentPage->getMovementSpeed() - 1);
	cboxConditionsMet->setCurrentIndex(currentPage->getPreconditionsMet());
	sbNumConditions->setValue(currentPage->getNumPreconditionsMet());
	lwPreconditions->clear();
	for (int i = 0; i < currentPage->numPreconditions(); ++i)
		lwPreconditions->addItem(currentPage->getPrecondition(i)->getViewableText());
	if (currentPage->numPreconditions() == 0)
		bDeletePrecondition->setEnabled(false);
	else
		bDeletePrecondition->setEnabled(true);
	delete mapRef;
}

void MapEventPageEditor::applyChanges()
{
	currentPage->setSpriteOpacity(sbSpriteOpacity->value());
	currentPage->setTriggerCondition(cboxTriggerCondition->currentIndex());
	currentPage->setLayer(cboxLayer->currentIndex());
	currentPage->setRelationToHero(cboxRelationToHero->currentIndex());
	currentPage->setForbidEventOverlap(cbForbidEventOverlap->isChecked());
	currentPage->setAnimationType(cboxAnimationType->currentIndex());
	currentPage->setMovementFrequency(cboxMovementFrequency->currentIndex());
	currentPage->setMovementType(cboxMovementType->currentIndex());
	currentPage->setMovementSpeed(cboxMovementSpeed->currentIndex() + 1);
	currentPage->setPreconditionsMet(cboxConditionsMet->currentIndex());
	currentPage->setNumPreconditionsMet(sbNumConditions->value());
}

void MapEventPageEditor::on_cboxConditionsMet_currentIndexChanged(int index)
{
	if (index == 1)
		wNumConditions->setEnabled(true);
	else
		wNumConditions->setEnabled(false);
}

void MapEventPageEditor::on_bDeletePrecondition_clicked()
{
	int row = lwPreconditions->currentRow();
	currentPage->deletePrecondition(row);
	delete (lwPreconditions->takeItem(row));
	if (currentPage->numPreconditions() == 0)
		bDeletePrecondition->setEnabled(false);
}

void MapEventPageEditor::on_bAddPrecondition_clicked()
{
	ConfigureMapEventPrecondition *configurePrecondition = new ConfigureMapEventPrecondition(window());
	MapEvent::Precondition *condition = new MapEvent::Precondition();
	configurePrecondition->setCondition(condition);
	if (configurePrecondition->exec())
	{
		configurePrecondition->getCondition(condition);
		currentPage->addPrecondition(condition);
		lwPreconditions->addItem(condition->getViewableText());
		lwPreconditions->setCurrentRow(currentPage->numPreconditions() - 1);
	}
	else
		delete condition;
	delete configurePrecondition;
	if (currentPage->numPreconditions() > 0)
		bDeletePrecondition->setEnabled(true);
}

void MapEventPageEditor::on_lwPreconditions_itemDoubleClicked(QListWidgetItem *item)
{
	ConfigureMapEventPrecondition *configurePrecondition = new ConfigureMapEventPrecondition(window());
	MapEvent::Precondition *condition = currentPage->getPrecondition(lwPreconditions->row(item));
	configurePrecondition->setCondition(condition);
	if (configurePrecondition->exec())
	{
		configurePrecondition->getCondition(condition);
		item->setText(condition->getViewableText());
	}
	delete configurePrecondition;
}

void MapEventPageEditor::on_sbSpriteOpacity_valueChanged(int value)
{
	spriteItem->setOpacity(value / 100.0);
}

void MapEventPageEditor::on_bSetEventGraphic_clicked()
{
	ConfigureMapEventPageSpriteGraphic *configureSpriteGraphic = new ConfigureMapEventPageSpriteGraphic(currentPage->getSpriteLocation(), window());
	if (configureSpriteGraphic->exec())
	{
		currentPage->setSpriteLocation(configureSpriteGraphic->getSpriteLocation());
		spriteItem->setPixmap(QPixmap::fromImage(currentPage->getImage()));
		if (currentPage->getSpriteLocation().startsWith("sprite:"))
		{
			animationTimer->stop();
			currentSpriteFrame = 0;
			spriteItem->setPos(spriteFrames[0]);
			animationTimer->start();
		}
		else
			spriteItem->setPos(8.0, 16.0);
	}
	delete configureSpriteGraphic;
}

void MapEventPageEditor::on_cboxAnimationType_currentIndexChanged(int index)
{
	animationTimer->stop();
	spriteFrames.clear();
	if (index == MapEvent::ANIMTYPE_NONCONTINUOUS)
		spriteFrames << QPointF(-48.0, -128.0);
	else if (index == MapEvent::ANIMTYPE_CONTINUOUS)
		spriteFrames << QPointF(0.0, -128.0) << QPointF(-48.0, -128.0) << QPointF(-96.0, -128.0) << QPointF(-48.0, -128.0);
	else if (index == MapEvent::ANIMTYPE_FIXEDNONCONTINUOUS)
		spriteFrames << QPointF(-48.0, -128.0);
	else if (index == MapEvent::ANIMTYPE_FIXEDCONTINUOUS)
		spriteFrames << QPointF(0.0, -128.0) << QPointF(-48.0, -128.0) << QPointF(-96.0, -128.0) << QPointF(-48.0, -128.0);
	else if (index == MapEvent::ANIMTYPE_FIXEDGRAPHIC)
		spriteFrames << QPointF(-48.0, -128.0);
	else if (index == MapEvent::ANIMTYPE_SPINAROUND)
		spriteFrames << QPointF(-48.0, 0.0) << QPointF(-48.0, -64.0) << QPointF(-48.0, -128.0) << QPointF(-48.0, -192.0);
	currentSpriteFrame = 0;
	animationTimer->start();
}

void MapEventPageEditor::on_cboxMovementFrequency_currentIndexChanged(int index)
{
	animationTimer->stop();
	animationTimer->setInterval(movementFrequencyTable[index]);
	currentSpriteFrame = 0;
	animationTimer->start();
}

void MapEventPageEditor::on_cboxMovementType_currentIndexChanged(int index)
{
	if (index == MapEvent::MOVETYPE_CUSTOMPATTERN)
		bConfigureMovementPattern->setEnabled(true);
	else
		bConfigureMovementPattern->setEnabled(false);
	if (index == MapEvent::MOVETYPE_STATIONARY)
		wMoveSpeed->setEnabled(false);
	else
		wMoveSpeed->setEnabled(true);
}

void MapEventPageEditor::on_cboxTriggerCondition_currentIndexChanged(int index)
{
	if (index == MapEvent::TRIGGER_COLLIDESWITHEVENT)
		bSetEventID->setEnabled(true);
	else
		bSetEventID->setEnabled(false);
}

void MapEventPageEditor::on_bSetEventID_clicked()
{
	SelectMapEvent *selectEvent = new SelectMapEvent(mapID, window());
	selectEvent->setEventID(currentPage->getCollidesWithEventID());
	if (selectEvent->exec())
		currentPage->setCollidesWithEventID(selectEvent->getEventID());
	delete selectEvent;
}

void MapEventPageEditor::on_bConfigureMovementPattern_clicked()
{
	ConfigureMapEventMovementPattern *configMovementPattern = new ConfigureMapEventMovementPattern(window());
	for (int i = 0; i < currentPage->customMovementPatternSize(); ++i)
		configMovementPattern->addPattern(currentPage->getMovementPattern(i));
	configMovementPattern->setRepeatPattern(currentPage->getRepeatMovementPattern());
	configMovementPattern->setIgnoreImpossibleMoves(currentPage->getIgnoreImpossibleMoves());
	if (configMovementPattern->exec())
	{
		for (int i = 0, count = currentPage->customMovementPatternSize(); i < count; ++i)
			currentPage->deleteMovementPattern(0);
		for (int i = 0, count = configMovementPattern->numPatterns(); i < count; ++i)
			currentPage->addMovementPattern(new MapEvent::MovementPattern(configMovementPattern->getPattern(i)));
		currentPage->setRepeatMovementPattern(configMovementPattern->getRepeatPattern());
		currentPage->setIgnoreImpossibleMoves(configMovementPattern->getIgnoreImpossibleMoves());
	}
	delete configMovementPattern;
}

void MapEventPageEditor::animateSpriteGraphic()
{
	if (currentPage->getSpriteLocation().startsWith("sprite:"))
	{
		currentSpriteFrame = (currentSpriteFrame + 1) % spriteFrames.size();
		spriteItem->setPos(spriteFrames[currentSpriteFrame]);
	}
}
