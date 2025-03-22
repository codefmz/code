#ifndef AQP_H
#define AQP_H

#include <QObject>
#include <QMessageBox>
#include <QApplication>

namespace AQP {
    void information(QWidget *parent, const QString &title, const QString &text,
        const QString &detailedText=QString());

    void warning(QWidget *parent, const QString &title, const QString &text,
        const QString &detailedText=QString());

    bool question(QWidget *parent, const QString &title, const QString &text,
        const QString &detailedText = QString(),
        const QString &yesText = QObject::tr("&Yes"),
        const QString &noText = QObject::tr("&No"));

    bool okToDelete(QWidget *parent, const QString &title, const QString &text,
        const QString &detailedText = QString());

    class Error : public std::exception {
    public:
        explicit Error(const QString &message) throw() : message(message.toUtf8()) {}
        ~Error() throw() {}
        const char *what() const throw() {
            return message;
        }
    private:
        const char *message;
    };

    template<typename T>
    bool okToClearData(bool (T::*saveData)(), T *parent, const QString &title,
        const QString &text, const QString &detailedText=QString())
    {
        Q_ASSERT(saveData && parent);

        std::unique_ptr<QMessageBox> messageBox = std::make_unique<QMessageBox>(parent);

        messageBox->setWindowModality(Qt::WindowModal);
        messageBox->setIcon(QMessageBox::Question);
        messageBox->setWindowTitle(QString("%1 - %2").arg(QApplication::applicationName()).arg(title));
        messageBox->setText(text);

        if (!detailedText.isEmpty()) {
            messageBox->setInformativeText(detailedText);
        }

        messageBox->addButton(QMessageBox::Save);
        messageBox->addButton(QMessageBox::Discard);
        messageBox->addButton(QMessageBox::Cancel);
        messageBox->setDefaultButton(QMessageBox::Save);
        messageBox->exec();

        if (messageBox->clickedButton() == messageBox->button(QMessageBox::Cancel)) {
            return false;
        }

        if (messageBox->clickedButton() == messageBox->button(QMessageBox::Save)) {
            return (parent->*saveData)();
        }

        return true;
    }

    const int MSecPerSecond = 1000;
};



#endif // AQP_H
