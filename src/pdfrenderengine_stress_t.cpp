/***************************************************************************
 *   copyright       : (C) 2021 by Andreas Stöckel                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QCoreApplication>
#include <QThread>
#include <iostream>
#include <random>

#include "pdfrenderengine_stress_t.h"
#include "pdfrendermanager.h"

void PDFRenderEngineStressTest::test_multithreading(const char *pdf_filename)
{
	const size_t N_THREADS = 8;
	const size_t N_IT = 10000;
    const size_t N_REPEAT = 1;

	std::default_random_engine re(5892190);  // Random engine with fixed seed
	std::uniform_int_distribution<int> dist_page(0, 20);

    PDFRenderManager manager(this, N_THREADS);

    int j=0;

    if (!manager.loadDocument(pdf_filename)) {
        std::cerr << "Error loading PDF!" << std::endl;
        return;
    }

    for (size_t i = 0; i < N_IT; i++) {
        if (i % 100 == 0) {
            std::cerr << "\rIteration " << (j * N_IT + i) << "/"
				          << (N_REPEAT * N_IT) << " ("
				          << int(((j * N_IT + i + 1) * 100) / (N_REPEAT * N_IT))
                          << "%)" << std::flush;
        }

        // Submit a render job with no receiver object
        manager.renderToImage(dist_page(re));

        // Process signal/slot events
        QCoreApplication::processEvents();

        QThread::currentThread()->usleep(100);  // 0.1ms
	}
    QThread::currentThread()->msleep(10000); // let the renderqueue work
}

