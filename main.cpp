#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

#include "io/iomanager.h"
#include <iostream>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QCoreApplication::setApplicationName("IndoorMap Editor. ");
    QCoreApplication::setApplicationVersion("1.14");

    //解析命令行参数
    QCommandLineParser parser;
    parser.setApplicationDescription("author: GaiMeng");
    parser.addHelpOption();
    parser.addVersionOption();

    // open and save options -o="../SomeDir" -o="somefile.json" -s=dir
       QCommandLineOption openfileOption(QStringList() << "o" << "open",
               QCoreApplication::translate("main", "open a file or all files under a directory"),
               QCoreApplication::translate("main", "openpath"));
       parser.addOption(openfileOption);

       QCommandLineOption savefileOption(QStringList() << "s" << "save",
               QCoreApplication::translate("main", "save to a file or to a directory"),
               QCoreApplication::translate("main", "savepath"));
       parser.addOption(savefileOption);

    //开始解析
    parser.process(a);
    if(parser.isSet(openfileOption)){
        QString openPath = parser.value(openfileOption);
        QString savePath = parser.value(savefileOption);

        QFileInfo openInfo = QFileInfo(openPath);
        QFileInfo saveInfo = QFileInfo(savePath);
        if(openInfo.isFile() && !(openInfo.suffix().compare("json",  Qt::CaseInsensitive))){ //a json file
            qDebug()<<"loading "<< openPath;
            if(IOManager::loadFile(openPath, w.currentDocument())){
                qDebug()<<"file "<< openPath <<"opened successfully";
                 //TODO: handle more commands
            }else{
                qDebug()<<"fail to open file";
                a.quit();return 0;
            }
        }else if(openInfo.isDir()){ //a dir

            if(saveInfo.isDir()){
                qDebug()<<"open files under "<< openPath ;

                QDir inDir(openPath);
                if(!inDir.exists()){
                    qDebug()<<"dir is not existed "<< openPath ;
                    a.quit();return 0;
                }
                QDir outDir(savePath);
                if(!outDir.exists()){
                    outDir.mkdir(savePath);
                }

                inDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
                QFileInfoList list = inDir.entryInfoList();

                for(int i=0; i<list.size(); i++){
                    QFileInfo fileInfo = list.at(i);
                    QString suffix = fileInfo.suffix();
                    if(!suffix.compare("json",  Qt::CaseInsensitive) ){
                        qDebug()<<"loading "<<fileInfo.absoluteFilePath();
                        if(IOManager::loadFile(fileInfo.absoluteFilePath(), w.currentDocument())){
                            qDebug()<<"file "<< fileInfo.absoluteFilePath() <<"opened successfully";

                            //TODO: handle more commands

                            QString saveFileName = savePath+"/"+fileInfo.fileName();
                            if(IOManager::saveFile(saveFileName, w.currentDocument())){
                                qDebug()<<"file "<< saveFileName <<"saved successfully";
                            }else{
                                qDebug()<<"faile to save file "<<saveFileName;
                            }

                        }else{
                            qDebug()<<"fail to open file"+fileInfo.absoluteFilePath();
                        }
                    }
                }
                qDebug()<<"All done!";
                a.quit();return 0;
            }else{
                qDebug()<<"invalid saving dir";
                a.quit();return 0;
            }
        }else{
            qDebug()<<"wrong -o input";
            a.quit();return 0;
        }
    }


    w.showMaximized();
    return a.exec();
}
