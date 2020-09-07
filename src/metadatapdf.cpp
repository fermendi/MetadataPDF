/**
 * @file <src/metadatapdf.cpp>
 *
 * @author Fernando Mendiburu - <fernando.mendiburu@ee.ufcg.edu.br>
 *
 * @package MetadataPDF
 *
 */

#include "metadatapdf.h"
#include "ui_metadatapdf.h"

MetadataPDF::MetadataPDF(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MetadataPDF)
{
    ui->setupUi(this);

    ui->dateMod->setDateTime(QDateTime::currentDateTime());
    ui->dateCreation->setDateTime(QDateTime::currentDateTime());
}

MetadataPDF::~MetadataPDF()
{
    delete ui;
}

void MetadataPDF::on_buttonSelectPDF_clicked()
{
    m_filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                "PDF file (*.pdf)");

    ui->linePDFFile->setText(m_filename);
}

void MetadataPDF::on_buttonConvertMetadata_clicked()
{
    if(!m_filename.isEmpty()) {

        std::string str2(" ");
        std::size_t found = m_filename.toStdString().find(str2);
        if (found!=std::string::npos) {
            QMessageBox::warning(this, "MetadataPDF",
                                     QString("Please avoid spaces (\" \") in the path or file name and try again!"),
                                  QMessageBox::Ok);
        }
        else {
            GetDateModAndCurrent();

            QMessageBox::StandardButton qAnsChangeMetadata;
            MessageChangeMetadata(qAnsChangeMetadata);

            if(qAnsChangeMetadata == QMessageBox::Yes) {
                ui->statusBar->showMessage("Changing the Metadata, please wait...");
                CreatePdfmarksFile();
                ChangeMetadata();
                DeletePdfMarks();
                ui->statusBar->showMessage("");
            }
        }


    } else {
        QMessageBox::warning(this, "MetadataPDF",
                                 QString("Please select a file!"),
                              QMessageBox::Ok);
    }
}

void MetadataPDF::on_buttonExit_clicked()
{
    ExitProgram();
}

void MetadataPDF::on_buttonHelp_clicked()
{
    HelpMessage();
}

void MetadataPDF::GetDateModAndCurrent() {
    QString qDateMod = (ui->dateMod->date()).toString("yyyyMMdd");
    QString qHourMod = (ui->dateMod->time()).toString("hhmmss");
    QString qDateCreation = (ui->dateCreation->date()).toString("yyyyMMdd");
    QString qHourCreation = (ui->dateCreation->time()).toString("hhmmss");

    m_ModDate = std::string("D:") + qDateMod.toStdString() + qHourMod.toStdString();
    m_CreationDate = "D:" + qDateCreation.toStdString() + qHourCreation.toStdString();
}

void MetadataPDF::CreatePdfmarksFile() {
    std::ofstream file;
    QFileInfo info(m_filename);
    QString pathFile = info.path();

    file.open (pathFile.toStdString() + "/pdfmarks");
    file << "[ /Title (" << (ui->lineTitle->text()).toStdString() << ")\n";
    file << "  /Author (" << (ui->lineAuthor->text()).toStdString() << ")\n";
    file << "  /Subject (" << (ui->lineSubject->text()).toStdString() << ")\n";
    file << "  /Keywords (" << (ui->lineKeywords->text()).toStdString() << ")\n";
    file << "  /ModDate (" << m_ModDate.c_str() << ")\n";
    file << "  /CreationDate (" << m_CreationDate.c_str() << ")\n";
    file << "  /Creator (" << (ui->lineCreator->text()).toStdString() << ")\n";
    file << "  /Producer (" << (ui->lineProducer->text()).toStdString() << ")\n";
    file << "  /DOCINFO pdfmark";
    file.close();
}

void MetadataPDF::ChangeMetadata() {
    QFileInfo info(m_filename);
    QString pathFile = info.path();

    QProcess::execute(QString("gs -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=") +
                      QString(pathFile) +
                      QString("/") +
                      QString(ui->linePDFOutput->text()) +
                      QString(" ") +
                      QString(ui->linePDFFile->text()) +
                      QString(" ") +
                      QString(pathFile) +
                      QString("/pdfmarks"));
}

void MetadataPDF::DeletePdfMarks() {
    QFileInfo info(m_filename);
    QString pathFile = info.path();

    QProcess::execute(QString("rm -rf ") +
                      QString(pathFile) +
                      QString("/pdfmarks"));

    QMessageBox::information(this, "End of conversion",QString("Metadata has been changed!"));
}


void MetadataPDF::MessageChangeMetadata(QMessageBox::StandardButton& qAnsChangeMetadata) {
    QFileInfo info(m_filename);
    qAnsChangeMetadata = QMessageBox::question(this, "Change metadata",
                                     QString("File: %1\n"
                                     "Path: %2\n\n"
                                     "Do you want to change to this data?\n\n"
                                     "PDF Output: %3\n"
                                     "Path to be save: %4\n"
                                     "Title: %5\n"
                                     "Author: %6\n"
                                     "Subject: %7\n"
                                     "Keywords: %8\n"
                                     "ModDate: %9\n"
                                     "CreationDate: %10\n"
                                     "Creator: %11\n"
                                     "Producer: %12\n")
                                         .arg(info.baseName())
                                         .arg(info.path())
                                         .arg(ui->linePDFOutput->text())
                                         .arg(info.path())
                                         .arg(ui->lineTitle->text())
                                         .arg(ui->lineAuthor->text())
                                         .arg(ui->lineSubject->text())
                                         .arg(ui->lineKeywords->text())
                                         .arg(QString(m_ModDate.c_str()))
                                         .arg(QString(m_CreationDate.c_str()))
                                         .arg(ui->lineCreator->text())
                                         .arg(ui->lineProducer->text()),
                                         QMessageBox::Yes|QMessageBox::No);
}

void MetadataPDF::ExitProgram() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning", "Do you want to quit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      QApplication::quit();
    }
}

void MetadataPDF::HelpMessage() {
    QMessageBox::information(this, "Help MetadataPDF",
                             QString("1- Press the button \"Select PDF\".\n\n"
                                     "2- Select the PDF tha you want to change the metadata (PDF name or path file without spaces).\n\n"
                                     "3- Change the fields of the metadata file as desired (title, author, etc).\n\n"
                                     "4- Press the button \"Convert Metadata\".\n\n"
                                     "5- Find the output file \"%1\" with the desired metadata.")
                             .arg(ui->linePDFOutput->text()),
                                  QMessageBox::Ok);
}

void MetadataPDF::on_actionAbout_triggered()
{
    AboutDialog AboutW;
    AboutW.setModal(true);
    AboutW.exec();

}

void MetadataPDF::on_actionHelp_triggered()
{
    HelpMessage();
}

void MetadataPDF::on_actionExit_triggered()
{
    ExitProgram();
}
