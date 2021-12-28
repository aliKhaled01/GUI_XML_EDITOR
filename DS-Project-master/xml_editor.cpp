#include "xml_editor.h"
#include "ui_xml_editor.h"

XML_Editor::XML_Editor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XML_Editor)
{
    ui->setupUi(this);
    ui->actionShow_Errors->setDisabled(true);
    ui->actionFix_Errors->setDisabled(true);
}

XML_Editor::~XML_Editor()
{
    delete ui;
}

//************************************************** Open ****************************************************

void XML_Editor::on_actionOpen_triggered()
{
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        QString f = "All Files *.* ;; XML *.xml ;; JSON *.json";
        QString path = QFileDialog::getOpenFileName(this, "Open a File", QDir::currentPath(), f);
        filePath = path;
        QFile file(path);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "No File Opened!");
            return;
        }
        else{
            QTextStream in(&file);
            QString file_text = "";
            if(!linesVec.empty()){
                linesVec.erase(linesVec.begin(), linesVec.end());
            }
            file_text = in.readAll();
            ui->textEdit->setText(file_text);
            file.close();
        }
}
//************************************************** Save As ****************************************************

void XML_Editor::on_actionSave_As_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save File", QDir::currentPath());
        filePath = path;
        QFile file(path);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "You did NOT Save The File!");
            return;
        }
        else{
            QTextStream output(&file);
            QString file_text = ui->textEdit->toPlainText();
            output << file_text;
            file.flush();
            file.close();
        }
}

//************************************************** Exit ****************************************************

void XML_Editor::on_actionExit_triggered()
{
    QApplication::quit();
}


//************************************************** About Us ****************************************************
void XML_Editor::on_actionAbout_Us_triggered()
{
    QMessageBox::information(this,"About Us","Mahmoud Yasser\nMahmoud Mohsen\nAli Khaled\nMohamed Nasser");
}

//************************************************** Font ****************************************************
void XML_Editor::on_actionChoose_Font_triggered()
{
    bool done;
        QFont font = QFontDialog::getFont(&done,QFont("Helvetica[Cronyx]",11),this);
        if(done){
            ui->textEdit->setFont(font);
        }
}

//************************************************** Text Changed ****************************************************
void XML_Editor::on_textEdit_textChanged()
{
    if(linesVec.size() > 0){
        linesVec.erase(linesVec.begin(), linesVec.end());
    }
    ui->statusbar->showMessage("");
    ui->actionShow_Errors->setDisabled(true);
    ui->actionFix_Errors->setDisabled(true);
}
//************************************************** Minify ****************************************************

void XML_Editor::on_actionMinify_triggered()
{
    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    QString output = "";
    if(linesVec.empty()){
        QString in = ui->textEdit->toPlainText();
        if(in != ""){
            QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save the Minified File", QDir::currentPath());
            filePath = path;
            QFile file(path);
            if(!file.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::warning(this, "Warning", "You did NOT Save The File!");
                return;
            }
            //Minify Operation
            linesVec = xmlToVec(in);
            for(int i=0; i<linesVec.size(); i++){
                if(!(linesVec[i][0] == '<' && linesVec[i][1] == '!')){
                    output += linesVec[i];
                }
            }
            QTextStream outputFile(&file);
            outputFile << output;
            file.flush();
            file.close();
            QMessageBox::information(this, "Info", "Minified Successfully!\nThe Minified File Location is: " + path);
            QFile file2(path);
            if(!file2.open(QFile::ReadOnly | QFile::Text)){
                QMessageBox::warning(this, "Warning", "Can NOT Open Result File!");
                return;
            }
            else{
                QTextStream in(&file2);
                QString file_text = "";
                file_text = in.readAll();
                ui->textEdit->setText(file_text);
                file2.close();
            }
        }
        else{
            QMessageBox::warning(this, "Warning", "There is NO Text!");
            return;
        }
    }
    else{
        QString fPath = QFileDialog::getSaveFileName(this, "Choose Location to Save the Minified File", QDir::currentPath());
        filePath = fPath;
        QFile file(fPath);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "You did NOT Save The File!");
            return;
        }
        for(int i=0; i<linesVec.size(); i++){
            if(!(linesVec[i][0] == '<' && linesVec[i][1] == '!')){
                output += linesVec[i];
            }
        }
        QTextStream file3(&file);
        file3 << output;
        file.flush();
        file.close();
        QMessageBox::information(this, "Info", "Minified Successfully!\nThe Minified File Location is: " + fPath);
        QFile f(fPath);
        if(!f.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Can NOT Open Result File!");
            return;
        }
        else{
            QTextStream in(&f);
            QString file_text = "";
            file_text = in.readAll();
            ui->textEdit->setText(file_text);
            f.close();
        }
    }
}

//************************************************** Prettify ****************************************************

void XML_Editor::on_actionPrettify_triggered()
{
        ui->statusbar->showMessage("");

        QString indentationChar = "     ";
        if(linesVec.size() == 0){
            linesVec = xmlToVec(ui->textEdit->toPlainText());
        }
        if(linesVec.size() == 0){
            QMessageBox::warning(this, "Warning", "There is NO Text!");
            return;
        }
        QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save File", QDir::currentPath());
        filePath = path;
        QFile file(path);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "You did NOT Save The File!");
            return;
        }
        //prettify operation
        if(checkConsistency(linesVec)){
            QString indentStr = "";
            QString outputStr = "";
            for(int i=0; i<linesVec.size(); i++){
               QString lineStr = linesVec[i];
               if(lineStr[0] == '<'){
                   if(lineStr[1] == '/'){
                        indentStr = indentation_remover(indentStr, indentationChar);
                        outputStr += indentStr+lineStr;
                        outputStr += "\n";
                   }
                   else if(lineStr[1] != '!' && lineStr[1] != '?'){
                       if(!(lineStr[lineStr.length()-2] == '/' && lineStr[lineStr.length()-1] == '>')){
                           outputStr += indentStr+lineStr;
                           outputStr += "\n";
                           indentStr += indentationChar;
                       }
                       else{
                           outputStr += indentStr+lineStr;
                           outputStr += "\n";
                       }
                   }
                   else{
                       outputStr += indentStr+lineStr;
                       outputStr += "\n";
                   }
               }
               else{
                   outputStr += indentStr+lineStr;
                   outputStr += "\n";
               }
            }
            QTextStream outputFile(&file);
            outputFile << outputStr;
            file.flush();
            file.close();
            QMessageBox::information(this, "Info", "Prettified Successfully!\nThe Prettified File Location is: " + path);
            ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
            QFile f(path);
            if(!f.open(QFile::ReadOnly | QFile::Text)){
                QMessageBox::warning(this, "Warning", "Can NOT Open Result File!");
                return;
            }
            else{
                QTextStream inp(&f);
                QString text = "";
                if(f.size() > 3*1024*1024){
                    ui->textEdit->setText("");
                    QMessageBox::warning(this, "Warning", "Only the First 100 Lines of Your File Are viewed Because Its Size is Large");
                    for(int q=0; q<100; q++){
                        text = inp.readLine();
                        ui->textEdit->append(text);
                    }
                }
                else{
                    text = inp.readAll();
                    ui->textEdit->setText(text);
                }
                f.close();
                ui->statusbar->showMessage("Done!");
            }

        }
        else{
            QMessageBox::warning(this, "Warning", "Your File is Inconsistent!");
            ui->actionShow_Errors->setDisabled(false);
            ui->actionFix_Errors->setDisabled(false);
            return;
        }
}

//************************************************** Check Consistency ****************************************************

void XML_Editor::on_actionCheck_Consistency_triggered()
{
    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    bool isConsistent;
    if(linesVec.empty()){
        QString input = ui->textEdit->toPlainText();
        if(input != ""){
            linesVec = xmlToVec(input);
            isConsistent = checkConsistency(linesVec);
        }
        else{
            QMessageBox::warning(this, "Warning", "There is NO Text!");
            return;
        }
    }
    else{

        isConsistent = checkConsistency(linesVec);
    }
    if(isConsistent == true){
        QMessageBox::information(this, "Info", "Your XML File Is Consistent");
        ui->statusbar->showMessage("Done!");
    }
    else{
        QMessageBox::warning(this, "Warning", "Your XML File is  Inconsistent!");
        ui->actionShow_Errors->setDisabled(false);
        ui->actionFix_Errors->setDisabled(false);
        return;
    }
}

//************************************************** Show Errors ****************************************************

void XML_Editor::on_actionShow_Errors_triggered()
{
    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    if(linesVec.size() == 0){
        linesVec = xmlToVec(ui->textEdit->toPlainText());
    }
    QVector<QString> tmp = linesVec;
    QMap<qint32, QString> errorsMap = detectErrors(tmp);
    ui->textEdit->setText("");
    int i = 0;
    for(i=0; i<tmp.size(); i++){
        if(errorsMap.find(i) != errorsMap.end()){
            ui->textEdit->setTextBackgroundColor(Qt::yellow);
            ui->textEdit->append(tmp[i]);
            ui->textEdit->setTextBackgroundColor(Qt::transparent);
        }
        else{
            ui->textEdit->append(tmp[i]);
        }
    }
    linesVec = tmp;
    ui->actionShow_Errors->setDisabled(false);
    ui->actionFix_Errors->setDisabled(false);
}
//************************************************** Fix Errors ****************************************************

void XML_Editor::on_actionFix_Errors_triggered()
{
    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    //-----------------------------------------------------------------------------
    QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save the Consistent File", QDir::currentPath());
    filePath = path;
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "You did NOT Save the File!");
        return;
    }
    if(linesVec.size() == 0){
        linesVec = xmlToVec(ui->textEdit->toPlainText());
    }
    QVector<QString> tmpVec = linesVec;
    QString result = "";
    QMap<qint32, QString> errorsMap = detectErrors(tmpVec);
    ui->textEdit->setText("");
    int i = 0;
    for(i=0; i<tmpVec.size(); i++){
        if(errorsMap.find(i) != errorsMap.end()){
            if(errorsMap[i][0] == 'A'){
                result += tmpVec[i];
                result += '\n';
            }
            else if(errorsMap[i][0] == 'U'){
                QString value = errorsMap[i];
                QString viewValue = "";
                for(int k = 1; k < value.length(); k++){
                    viewValue += value[k];
                }
                result += tmpVec[i];
                result += '\n';
                result += viewValue;
                result += '\n';
            }
            else if(errorsMap[i][0] != 'D'){
                result += errorsMap[i];
                result += '\n';
            }
        }
        else{
            result += tmpVec[i];
            result += '\n';
        }
    }

    QVector<int> lineNumber;
    for(auto j = errorsMap.begin(); j != errorsMap.end(); j++){
        QString value = j.value();
        if(value[0] == 'A'){
            lineNumber.push_back(j.key());
        }
    }
    for(int l = lineNumber.length()-1; l >= 0; l--){
        QString strValue = errorsMap[lineNumber[l]];
        QString viewValue = "";
        for(int k = 1; k < strValue.length(); k++){
            viewValue += strValue[k];
        }
        viewValue = "</" + viewValue;
        result += viewValue;
        result += '\n';
    }
    QTextStream out(&file);
    out << result;
    file.flush();
    file.close();
    QMessageBox::information(this, "Info", "Fixed Successfully!\nThe Consistent File Location is: " + path);
    QFile f(path);
    if(!f.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
        return;
    }
    else{
        QTextStream input(&f);
        QString text = "";
        if(f.size() > 3*1024*1024){ // file larger than 3 MB
            ui->textEdit->setText("");
            QMessageBox::warning(this, "Warning", "Only the First 100 Lines of Your File Are viewed Because Its Size is Large");
            for(int q=0; q<100; q++){
                text = input.readLine();
                ui->textEdit->append(text);
            }
        }
        else{
            text = input.readAll();
            ui->textEdit->setText(text);
        }
        f.close();
        ui->statusbar->showMessage("Done!");
    }
}
//************************************************** Compress ****************************************************

int size2 = 0;
map<char, string> newCode;

void XML_Editor::on_actionCompress_triggered()
{
    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    queue<unsigned char> que;
    double oldSize = 0;
    double newSize = 0;
    QString filename = QFileDialog::getOpenFileName(this, "Choose a File", QDir::currentPath());
    QFile old(filename);
    if(!old.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Open File!");
        return;
    }
    QTextStream oldIn(&old);
    QString oldInStr = oldIn.readAll();
    string c = oldInStr.toLocal8Bit().constData();
    oldSize = old.size()/1024.0;
    old.close();
    QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save the Compressed File", QDir::currentPath());
    QFile new1(path);
    if(!new1.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    QString codeName = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Compression Key File", QDir::currentPath());
    QFile key(codeName);
    if(!key.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    fstream writeToOutput;
    fstream WriteToKey;
    writeToOutput.open(path.toLocal8Bit().constData(), ios::out);
    WriteToKey.open(codeName.toLocal8Bit().constData(), ios::out);
    tree t1;
    que = t1.assembleTree(c, newCode, size2);
    unsigned char zx;
    string finalStr = "";
    for (unsigned int i = 0; que.size() != 0; i++) {
        zx = que.front();
        finalStr += zx;
        que.pop();
    }
    writeToOutput << finalStr;
    new1.flush();
    new1.close();
    WriteToKey << size2 << endl;
    for(auto i = newCode.begin(); i!=newCode.end(); i++){
        string temp = i->second + '&' + i->first;
        WriteToKey << temp;
    }
    writeToOutput.close();
    WriteToKey.close();
    ui->statusbar->showMessage("Done!");
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Can NOT Open Result File!");
        return;
    }
    else{
        newSize = file.size()/1024.0;
        QMessageBox::information(this, "Info", "File Compressed Successfully!\nThe Compressed File Location is: " + path + "\nOld Size is " + QString::number(oldSize) + " KB\nNew Size is " + QString::number(newSize) + " KB");
        QTextStream in(&file);
        QString text = "";
        if(newSize > 3*1024){
            ui->textEdit->setText("");
            QMessageBox::warning(this, "Warning", "Only the First 100 Lines of Your File Are viewed Because Its Size is Large");
            for(int q=0; q<100; q++){
                text = in.readLine();
                ui->textEdit->append(text);
            }
        }
        else{
            text = in.readAll();
            ui->textEdit->setText(text);
        }
        file.close();
        ui->statusbar->showMessage("Done!");
    }
}
//************************************************** Convert To JSON ****************************************************

void XML_Editor::on_actionConvert_To_JSON_triggered()
{
    QString input = ui->textEdit->toPlainText();
    if(input == ""){
        QMessageBox::warning(this, "Warning", "There is NO Text!");
        return;
    }

    linesVec = xmlToVec(input);
    if(!checkConsistency(linesVec)){
        QMessageBox::warning(this, "Warning", "Your File is Inconsistent!");
        return;
    }
    string inN = input.toLocal8Bit().constData();
    vector<string> linesN = xmlToVecN(inN);
    int linesNSize = linesN.size();
    node *root;
    root = tree_creator(linesN, linesNSize);
    string output = "";
    output  = iteration(root, output);
    QString result = QString::fromStdString(output);

    ui->textEdit->setText(result);
    QString path = QFileDialog::getSaveFileName(this, "Choose Location to Save the New File", QDir::currentPath());
    QFile fN(path);
    if(!fN.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    QTextStream outStream(&fN);
    outStream << result;
    fN.flush();
    fN.close();
    QMessageBox::information(this, "Info", "Coverted Successfully!\nNew file Location is: " + path);
    ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    QFile inn(path);
    if(!fN.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Can NOT Open Result File!");
        return;
    }
    QTextStream inp(&inn);
    QString txtArea;
    if(inn.size() > 3*1024*1024){
        ui->textEdit->setText("");
        QMessageBox::warning(this, "Warning", "Only the First 100 Lines of Your File Are viewed Because Its Size is Large");
        for(int q=0; q<100; q++){
            txtArea = inp.readLine();
            ui->textEdit->append(txtArea);
        }
    }
    else{
        txtArea = inp.readAll();
        ui->textEdit->setText(txtArea);
    }
    inn.close();
}

