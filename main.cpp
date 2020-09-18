#include <QApplication>
#include "framelesswindow/framelesswindow.h"
#include "src/metadatapdf.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  FramelessWindow framelessWindow;
  framelessWindow.setWindowState(Qt::WindowNoState);
  framelessWindow.setWindowIcon(QPixmap(":/logo/logo_80x802.png"));

  MetadataPDF *mainWindow = new MetadataPDF;

  framelessWindow.setContent(mainWindow);
  framelessWindow.show();

  return a.exec();
}
