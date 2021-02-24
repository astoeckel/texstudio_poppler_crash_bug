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

#include <iostream>

#include "pdfrenderengine.h"
#include "pdfrendermanager.h"

RenderCommand::RenderCommand(int p, double xr, double yr, int x, int y, int w,
                             int h)
    : pageNr(p), xres(xr), yres(yr), x(x), y(y), w(w), h(h), ticket(-1)
{
}

PDFRenderEngine::PDFRenderEngine(QObject *parent, PDFQueue *mQueue)
    : QThread(parent), cachedNumPages(0)
{
	queue = mQueue;
	queue->ref();
}

PDFRenderEngine::~PDFRenderEngine()
{
	wait();
	queue->deref();
}

void PDFRenderEngine::setDocument(const QSharedPointer<Poppler::Document> &doc)
{
	document = doc;
	cachedNumPages = (document.isNull()) ? 0 : document->numPages();
}

void PDFRenderEngine::run()
{
	while (true) {
		RenderCommand command(-1);
		// wait for enqueued lines
		queue->mCommandsAvailable.acquire();
		if (queue->stopped)
			break;
		// get Linedata
		queue->mQueueLock.lock();
		command = queue->mCommands.dequeue();
		queue->mQueueLock.unlock();

		// render Image
		if (!document.isNull() && command.pageNr >= 0 &&
		    command.pageNr < cachedNumPages) {
			queue->mDocumentLock.lock();
			Poppler::Page *page = document->page(command.pageNr);
			queue->mDocumentLock.unlock();
			if (page) {
				QImage image =
				    page->renderToImage(command.xres, command.yres, command.x,
				                        command.y, command.w, command.h);

				queue->mDocumentLock.lock();
				delete page;
				queue->mDocumentLock.unlock();
			}
		}
	}
}

