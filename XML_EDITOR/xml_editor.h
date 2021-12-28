#ifndef XML_EDITOR_H
#define XML_EDITOR_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QStack>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <xml_vector.h>
#include <indentation_remover.h>
#include <checkConsistency.h>
#include <compression.h>
#include <xml_to_json.h>
QT_BEGIN_NAMESPACE
namespace Ui { class XML_Editor; }
QT_END_NAMESPACE

class XML_Editor : public QMainWindow
{
    Q_OBJECT

public:
    XML_Editor(QWidget *parent = nullptr);
    ~XML_Editor();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_Us_triggered();

    void on_actionChoose_Font_triggered();

    void on_textEdit_textChanged();

    void on_actionMinify_triggered();

    void on_actionPrettify_triggered();

    void on_actionCheck_Consistency_triggered();

    void on_actionShow_Errors_triggered();

    void on_actionFix_Errors_triggered();

    void on_actionCompress_triggered();

    void on_actionConvert_To_JSON_triggered();

private:
    Ui::XML_Editor *ui;
    QString filePath = "";
    QVector<QString> linesVec;
};
#endif // XML_EDITOR_H
