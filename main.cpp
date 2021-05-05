#include <QtWidgets>
#include "myServer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    myServer server(2324);
    server.show();
    return app.exec();
}
