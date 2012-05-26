/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "MainPage.h"

#include <MApplication>
#include <MApplicationWindow>

int main(int argc, char *argv[])
{
	MApplication app(argc, argv);
	app.setOrganizationDomain("hannu.mallat.fi");
	app.setOrganizationName("Hannu Mallat");
	app.setApplicationName("Tagbuilder");

	MApplicationWindow window;
	window.setPortraitOrientation();
	window.lockOrientation();

	MainPage page;
	page.appear(&window);
	window.show();
	return app.exec();
}
