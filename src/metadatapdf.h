/**
 * @file <src/metadatapdf.h>
 *
 * @author Fernando Mendiburu - <fernando.mendiburu@ee.ufcg.edu.br>
 *
 * @package MetadataPDF
 *
 */

#ifndef METADATAPDF_H
#define METADATAPDF_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <iostream>
#include <fstream>
#include <QDate>

#include "aboutdialog.h"

namespace Ui {
class MetadataPDF;
}

class MetadataPDF : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetadataPDF(QWidget *parent = 0);
    ~MetadataPDF();

private slots:
    void on_buttonSelectPDF_clicked();
    void on_buttonConvertMetadata_clicked();
    void on_buttonExit_clicked();
    void on_buttonHelp_clicked();
    void on_actionAbout_triggered();
    void on_actionHelp_triggered();
    void on_actionExit_triggered();

private:

    void MessageChangeMetadata(QMessageBox::StandardButton&);
    void CreatePdfmarksFile();
    void ChangeMetadata();
    void DeletePdfMarks();
    void ExitProgram();
    void HelpMessage();
    void GetDateModAndCurrent();

    Ui::MetadataPDF *ui;

    QString m_filename;
    std::string m_ModDate;
    std::string m_CreationDate;
};

#endif // METADATAPDF_H
