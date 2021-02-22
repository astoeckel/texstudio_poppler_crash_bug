#ifndef PDFRENDERMANAGER_H
#define PDFRENDERMANAGER_H

#include <QCache>
#include <QImage>
#include <QPixmap>

#include <atomic>

#include "pdfrenderengine.h"

class RecInfo {
public:
	int pageNr;
};

class PDFQueue : public QObject {
public:
	explicit PDFQueue(QObject *parent = nullptr);

	inline void ref() { m_ref.ref(); }
	void deref();
	int getRef() { return m_ref.fetchAndAddRelaxed(0); }

	QQueue<RenderCommand> mCommands;
	QSemaphore mCommandsAvailable;
	QMutex mQueueLock;
	std::atomic<bool> stopped;

	int num_renderQueues;

	QList<PDFRenderEngine *> renderQueues;

private:
	QAtomicInt m_ref;
};

class PDFRenderManager : public QObject {
	Q_OBJECT

public:
	explicit PDFRenderManager(QObject *parent, int num_renderQueues);
	~PDFRenderManager();

	void renderToImage(int pageNr);
	bool loadDocument(const QString &fileName);
	void stopRendering();

private:
	friend class PDFRenderEngine;
	bool checkDuplicate(int &ticket, RecInfo &info);
	void enqueue(RenderCommand cmd);

	QSharedPointer<Poppler::Document> document;
	int cachedNumPages;

	PDFQueue *queueAdministration;
};

#endif  // PDFRENDERMANAGER_H

