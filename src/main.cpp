/**
 * @file <src/main.cpp>
 *
 * @author Fernando Mendiburu - <fernando.mendiburu@ee.ufcg.edu.br>
 *
 * @package MetadataPDF
 *
 */

#include "metadatapdf.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MetadataPDF w;
    w.show();

    return a.exec();
}
