#include "AQP.h"
#include <memory>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>

bool AQP::okToDelete(QWidget *parent, const QString &title, const QString &text, const QString &detailedText)
{
    std::unique_ptr<QMessageBox> messageBox = std::make_unique<QMessageBox>(parent);
    if (parent) {
        messageBox->setWindowModality(Qt::WindowModal);
    }

    messageBox->setIcon(QMessageBox::Question);
    messageBox->setWindowTitle(QString("%1 - %2").arg(QApplication::applicationName()).arg(title));
    messageBox->setText(text);
    if (!detailedText.isEmpty()) {
        messageBox->setInformativeText(detailedText);
    }

    QPushButton *deleteButton = messageBox->addButton(QObject::tr("&Delete"), QMessageBox::AcceptRole);
    messageBox->addButton(QObject::tr("Do & Not Delete"), QMessageBox::RejectRole);
    messageBox->setDefaultButton(deleteButton);
    messageBox->exec();
    return messageBox->clickedButton() == deleteButton;
}

void AQP::information(QWidget *parent, const QString &title, const QString &text, const QString &detailedText)
{
    std::unique_ptr<QMessageBox> messageBox = std::make_unique<QMessageBox>(parent);
    if (parent) {
        messageBox->setWindowModality(Qt::WindowModal);
    }

    messageBox->setWindowTitle(QString("%1 - %2").arg(QApplication::applicationName()).arg(title));
    messageBox->setText(text);
    if (!detailedText.isEmpty()) {
        messageBox->setInformativeText(detailedText);
    }

    messageBox->setIcon(QMessageBox::Information);
    messageBox->addButton(QMessageBox::Ok);
    messageBox->exec();
}

void AQP::warning(QWidget *parent, const QString &title, const QString &text, const QString &detailedText)
{
    std::unique_ptr<QMessageBox> messageBox = std::make_unique<QMessageBox>(parent);
    if (parent) {
        messageBox->setWindowModality(Qt::WindowModal);
    }

    messageBox->setWindowTitle(QString("%1 - %2").arg(QApplication::applicationName()).arg(title));
    messageBox->setText(text);
    if (!detailedText.isEmpty()) {
        messageBox->setInformativeText(detailedText);
    }

    messageBox->setIcon(QMessageBox::Warning);
    messageBox->addButton(QMessageBox::Ok);
    messageBox->exec();
}
