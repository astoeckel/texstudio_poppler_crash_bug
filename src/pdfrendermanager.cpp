/***************************************************************************
 *   copyright       : (C) 2011 by Jan Sundermeyer                         *
 *   http://texstudio.sf.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pdfrendermanager.h"

PDFQueue::PDFQueue(QObject *parent)
    : QObject(parent), stopped(true), num_renderQueues(1)
{
	m_ref = 1;
}

void PDFQueue::deref()
{
	if (!m_ref.deref()) {
		delete this;
	}
}

PDFRenderManager::PDFRenderManager(QObject *parent, int num_renderQueues)
    : QObject(parent), cachedNumPages(0)
{
	queueAdministration = new PDFQueue();
	queueAdministration->num_renderQueues = num_renderQueues;

	for (int i = 0; i < queueAdministration->num_renderQueues; i++) {
		auto *renderQueue = new PDFRenderEngine(nullptr, queueAdministration);
		queueAdministration->renderQueues.append(renderQueue);
	}

	queueAdministration->stopped = false;
}

PDFRenderManager::~PDFRenderManager()
{
	stopRendering();
	queueAdministration->deref();
}

void PDFRenderManager::stopRendering()
{
	queueAdministration->stopped = true;
	for (int i = 0; i < queueAdministration->num_renderQueues; i++) {
		if (queueAdministration->renderQueues[i]) {
			queueAdministration->renderQueues[i]->wait();
			delete queueAdministration->renderQueues[i];
		}
		queueAdministration->renderQueues[i] = nullptr;
	}
	queueAdministration->mCommandsAvailable.release(
	    queueAdministration->num_renderQueues);
	document.clear();
	cachedNumPages = 0;
}

bool PDFRenderManager::loadDocument(const QString &fileName)
{
	Poppler::Document *docPtr;
	try {
		document = QSharedPointer<Poppler::Document>(
		    Poppler::Document::load(fileName));
	}
	catch (...) {
		return false;
	}

	if (!document) {
		return false;
	}

	cachedNumPages = document->numPages();

	for (int i = 0; i < queueAdministration->num_renderQueues; i++) {
		queueAdministration->renderQueues[i]->setDocument(document);
		if (!queueAdministration->renderQueues[i]->isRunning()) {
			queueAdministration->renderQueues[i]->start();
		}
	}

	return true;
}

void PDFRenderManager::renderToImage(int pageNr)
{
	if (document.isNull() || (pageNr < 0 || pageNr >= cachedNumPages)) {
		return;
	}
	RenderCommand cmd(pageNr);
	enqueue(cmd);
}

void PDFRenderManager::enqueue(RenderCommand cmd)
{
	queueAdministration->mQueueLock.lock();
	queueAdministration->mCommands.enqueue(cmd);
	queueAdministration->mQueueLock.unlock();
	queueAdministration->mCommandsAvailable.release();
}

