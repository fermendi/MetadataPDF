/**
 * @file <src/metadatapdf.h>
 *
 * @author Fernando Mendiburu - <fernando.mendiburu@ee.ufcg.edu.br>
 *
 * @package MetadataPDF
 *
 * https://github.com/fermendi
 */

#ifndef METADATAPDF_H
#define METADATAPDF_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>

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
    void on_buttonHelp_clicked();
    void on_buttonMenuD_clicked();
    void on_buttonApplication_clicked();
    void on_buttonAbout_clicked();
    void timeoutHandler();
    void on_buttonAddPDF_clicked();
    void on_buttonMerge_clicked();
    void on_buttonRemovePDF_clicked();

    void on_buttonMergePDF_clicked();

private:
    enum stackedWidgetState {Init, App, Merge, Help, About};
    enum statusBarState {SelectFile, WrongPath, WrongNullPDFs, WrongPDFsPath, Progress_Metadata,
                         Progress_Merge, Success_Metadata, Success_Merge, Clear};

    bool MessageChangeMetadata();
    void CreatePdfmarksFile();
    void ChangeMetadata();
    void DeletePdfMarks();
    void HelpMessage();
    void GetDateModAndCurrent();
    void ResetGUI();
    void PrintPDFs();
    void StatusBarHandler(statusBarState status);
    void StackedWidgetHandler(stackedWidgetState status);

    Ui::MetadataPDF *ui;

    QString m_filename;
    QStringList qFilesAdd;
    std::string m_ModDate;
    std::string m_CreationDate;
    QTimer timer;
};

#endif // METADATAPDF_H
