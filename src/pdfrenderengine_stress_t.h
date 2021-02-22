/***************************************************************************
 *   copyright       : (C) 2021 by Andreas Stöckel                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PDFRENDERENGINE_STRESS_T_H
#define PDFRENDERENGINE_STRESS_T_H

#include <QObject>

/**
 * Implements a stress test triggering issue #1409, i.e., a crash in
 * Poppler.
 */
class PDFRenderEngineStressTest: public QObject
{
	Q_OBJECT
public slots:
	void test_multithreading(const char *pdf_filename);
};

#endif  // PDFRENDERENGINE_STRESS_T_H
