/* Copyright 2013 MultiMC Contributors
 *
 * Authors: Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>

#include <QApplication>
#include <QDir>

#include "gui/mainwindow.h"

#include "data/appsettings.h"
#include "data/loginresponse.h"

#include "util/cmdutils.h"

using namespace Util::Commandline;

int main(int argc, char *argv[])
{
    // initialize Qt
	QApplication app(argc, argv);
	app.setOrganizationName("Forkk");
	app.setApplicationName("MultiMC 5");

    // Print app header
    std::cout << "MultiMC 5" << std::endl;
    std::cout << "(c) 2013 MultiMC contributors" << std::endl << std::endl;

    // Commandline parsing
    Parser parser(FlagStyle::GNU, ArgumentStyle::SpaceAndEquals);

    // --help
    parser.addSwitch("help");
    parser.addShortOpt("help", 'h');
    parser.addDocumentation("help", "displays help on command line parameters");
    // --dir
    parser.addOption("dir", app.applicationDirPath());
    parser.addShortOpt("dir", 'd');
    parser.addDocumentation("dir", "use the supplied directory as MultiMC root instead of the binary location (use '.' for current)");
    // --update
    parser.addOption("update");
    parser.addShortOpt("update", 'u');
    parser.addDocumentation("update", "replaces the given file with the running executable", "<path>");
    // --quietupdate
    parser.addSwitch("quietupdate");
    parser.addShortOpt("quietupdate", 'U');
    parser.addDocumentation("quietupdate", "doesn't restart MultiMC after installing updates");
    // --launch
    parser.addOption("launch");
    parser.addShortOpt("launch", 'l');
    parser.addDocumentation("launch", "tries to launch the given instance", "<inst>");

    // parse the arguments
    QHash<QString, QVariant> args;
    try {
        args = parser.parse(app.arguments());
    } catch(ParsingError e) {
        std::cerr << "CommandLineError: " << e.what() << std::endl;
        return 1;
    }

    // display help and exit
    if (args["help"].toBool()) {
        std::cout << qPrintable(parser.compileHelp(app.arguments()[0]));
        return 0;
    }

    // update
    // Note: cwd is always the current executable path!
    if (!args["update"].isNull())
    {
        std::cout << "Performing MultiMC update: " << qPrintable(args["update"].toString()) << std::endl;
        QDir::setCurrent(app.applicationDirPath());
        QFile file(app.applicationFilePath());
        file.copy(args["update"].toString());
        if(args["quietupdate"].toBool())
            return 0;
    }

    // change directory
    QDir::setCurrent(args["dir"].toString());

    // launch instance.
    if (!args["launch"].isNull())
    {
        std::cout << "Launching instance: " << qPrintable(args["launch"].toString()) << std::endl;
        // TODO: make it launch the an instance.
        // needs the new instance model to be complete
        std::cerr << "Launching Instances is not implemented yet!" << std::endl;
        return 255;
    }

    // load settings
	settings = new AppSettings(&app);
	
	// Register meta types.
	qRegisterMetaType<LoginResponse>("LoginResponse");
	
    // show window
	MainWindow mainWin;
	mainWin.show();
	
    // loop
	return app.exec();
}
