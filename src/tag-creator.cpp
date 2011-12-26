/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "MainPage.h"

#include <MApplication>
#include <MApplicationWindow>

int main(int argc, char *argv[])
{
	MApplication app(argc, argv);
	app.setOrganizationDomain("hannu.mallat.fi");
	app.setOrganizationName("Hannu Mallat");
	app.setApplicationName("Tag Creator");

	MApplicationWindow window;
	window.setPortraitOrientation();
	window.lockOrientation();

	MainPage page;
	page.appear(&window);
	window.show();
	return app.exec();
}
