/**
 * @file <src/metadatapdf.cpp>
 *
 * @author Fernando Mendiburu - <fernando.mendiburu@ee.ufcg.edu.br>
 *
 * @package MetadataPDF
 *
 * https://github.com/fermendi
 */

#include "metadatapdf.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <iostream>
#include <fstream>
#include <QDate>
#include <QPropertyAnimation>
#include <QTimer>

#include "ui_metadatapdf.h"

MetadataPDF::MetadataPDF(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MetadataPDF)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeoutHandler()));

    ui->dateMod->setDateTime(QDateTime::currentDateTime());
    ui->dateCreation->setDateTime(QDateTime::currentDateTime());
    ui->linePDFFile->setDisabled(true);
    StackedWidgetHandler(Init);
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
    if(!m_filename.isEmpty()) ResetGUI();
}

void MetadataPDF::ResetGUI() {
    StatusBarHandler(Clear);
    ui->linePDFFile->setStyleSheet("");
}

void MetadataPDF::on_buttonConvertMetadata_clicked()
{
    m_filename = ui->linePDFFile->text();
    if(!m_filename.isEmpty()) {

        std::string str2(" ");
        std::size_t found = m_filename.toStdString().find(str2);
        if (found!=std::string::npos) {
            StatusBarHandler(WrongPath);
        }
        else {
            GetDateModAndCurrent();

            bool isChangedMetadata;
            isChangedMetadata = MessageChangeMetadata();

            if(isChangedMetadata) {
                ResetGUI();
                StatusBarHandler(Progress);
                CreatePdfmarksFile();
                ChangeMetadata();
                DeletePdfMarks();
                StatusBarHandler(Success);
            }
            else {
                StatusBarHandler(Clear);
            }
        }
    } else {
        StatusBarHandler(SelectFile);
    }
}

void MetadataPDF::on_buttonHelp_clicked()
{
    StackedWidgetHandler(Help);
    StatusBarHandler(Clear);
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
}

bool MetadataPDF::MessageChangeMetadata() {
    //define styles buttons (TODO: make it in .qss files)
    QString QStyleButtons = " QPushButton {"
                           "background-color: rgb(78, 154, 6);"
                           "border: 2px solid rgb(50, 88, 50);"
                           "border-radius: 5px;"
                           "color:black;"
                           "font-size: 12pt;"
                           "font-weight: bold;"
                           "height: 30px;"
                           "width: 80px;"
                       "}"
                      " QPushButton:hover {"
                           "background-color: rgb(50, 88, 50);"
                           "border: 2px solid rgb(45, 60, 45);"
                           "color:white;"
                       "}"
                       "QPushButton:pressed {"
                           "background-color: rgb(85, 170, 255);"
                           "border: 2px solid rgb(160, 170, 255);"
                           "color: rgb(35, 35, 35);"
                       "}";

    QFileInfo info(m_filename);
    QMessageBox mb("Change metadata",
                       QString("File: %1.pdf\n"
                       "Path: %2\n\n"
                       "Do you want to change to this data?\n\n"
                       "PDF Output: %3\n"
                       "Path to be saved: %4\n"
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
                                   QMessageBox::NoIcon,
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::NoButton);
    mb.setIconPixmap(QPixmap("://icons/questionmark_30x30.png"));
    mb.setWindowIcon(QIcon(":/logo/logo40x40.png"));
    mb.setStyleSheet("background-color: rgb(52, 101, 164);color: rgb(255, 255, 255);");
    mb.button(QMessageBox::Yes)->setStyleSheet(QStyleButtons);
    mb.button(QMessageBox::No)->setStyleSheet(QStyleButtons);
    if(mb.exec() == QMessageBox::Yes){
        return true;
    }
    return false;
}

void MetadataPDF::on_buttonMenuD_clicked()
{
    int witdthMenu = ui->left_menu->width();
    int widthExtended;
    int standard = 50;
    int maxExtend = 160;
    QRect SizeIconXY = ui->Icon_Move->geometry();
    QRect SizeIconXYExtended;
    QRect StdPosSizeIconXY(4,440,42,42);
    QRect MaxPosSizeIconXY(38,420,84,84);

    if(witdthMenu == 50) {
        widthExtended = maxExtend;
        SizeIconXYExtended = MaxPosSizeIconXY;
    }
    else {
        widthExtended = standard;
        SizeIconXYExtended = StdPosSizeIconXY;
    }

    // Create the menu animation.
    QPropertyAnimation* MenuAnimation = new QPropertyAnimation(ui->left_menu, "minimumWidth");
    MenuAnimation->setDuration(300);
    MenuAnimation->setStartValue(witdthMenu);
    MenuAnimation->setEndValue(widthExtended);
    MenuAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    MenuAnimation->start();

    // Create the icon animation.
    QPropertyAnimation* IconAnimation = new QPropertyAnimation(ui->Icon_Move, "geometry");
    IconAnimation->setStartValue(SizeIconXY);
    IconAnimation->setEndValue(SizeIconXYExtended);
    IconAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    IconAnimation->start();
}

void MetadataPDF::on_buttonApplication_clicked()
{
    StackedWidgetHandler(App);
}

void MetadataPDF::on_buttonAbout_clicked()
{
    StackedWidgetHandler(About);
    StatusBarHandler(Clear);
}

void MetadataPDF::StatusBarHandler(statusBarState status) {
    switch (status) {
        case WrongPath:
            ui->statusLabel->setStyleSheet("color:rgb(255, 0, 0);");
            ui->statusLabel->setText("Please avoid spaces (\" \") in the path or file name and try again!");
            ui->linePDFFile->setStyleSheet("border: 2px solid rgb(255, 0, 0); border-radius: 5px;");
            break;
        case SelectFile:
            ui->statusLabel->setStyleSheet("color:rgb(255, 0, 0);");
            ui->statusLabel->setText("Please select a file!");
            ui->linePDFFile->setStyleSheet("border: 2px solid rgb(255, 0, 0); border-radius: 5px;");
            break;
        case Progress:
            ui->statusLabel->setStyleSheet("color:rgb(255, 255, 0);");
            ui->statusLabel->setText("Changing Metadata, please wait...");
            break;
        case Success:
            ui->statusLabel->setStyleSheet("color:rgb(0, 255, 0);");
            ui->statusLabel->setText("Metadata has been changed successfully!");
            timer.start(10000);     // 10s to erase the message
            break;
        case Clear:
            ui->statusLabel->clear();
            break;
        default:
            break;
    }
}

void MetadataPDF::timeoutHandler()
{
    StatusBarHandler(Clear);
}

void MetadataPDF::StackedWidgetHandler(stackedWidgetState status) {
    ui->stackedWidget->setCurrentIndex(status);
    switch (status) {
    case Init:
    case About:
        ui->Icon_Move->setHidden(true);
        break;
    case App:
    case Help:
        ui->Icon_Move->setVisible(true);
        break;
    default:
        break;
    }
}
