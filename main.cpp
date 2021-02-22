/***************************************************************************
 *   copyright       : (C) 2021 by Andreas Stöckel                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <pdfrenderengine_stress_t.h>

#include <QGuiApplication>

#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " [PDF File]" << std::endl;
		return 1;
	}

	QGuiApplication app(argc, argv);

	PDFRenderEngineStressTest test;
	test.test_multithreading(argv[1]);
}
