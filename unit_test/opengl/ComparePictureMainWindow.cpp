/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "ComparePictureMainWindow.h"

#include <QtCore/QDir> 
#include <QtGui/QPixmap>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>

#include <iostream>

using namespace std;

ComparePictureMainWindow::ComparePictureMainWindow(){
  setupUi(this);

  connect(differentButton,SIGNAL(clicked()),this,SLOT(differentButtonClicked()));
  connect(sameAndRejectButton,SIGNAL(clicked()),this,SLOT(sameAndRejectButtonClicked()));

  QDir dir(QCoreApplication::applicationDirPath());
  QStringList filesList=dir.entryList(QDir::Files);
  for(QStringList::iterator it=filesList.begin();it!=filesList.end();++it){
    if((*it).startsWith("model_"))
      modelFilesList.push_back((*it).remove(0,QString("model_").length()));
    if((*it).startsWith("result_"))
      resultFilesList.push_back((*it).remove(0,QString("result_").length()));
  }

  checkPicture();

}

void ComparePictureMainWindow::checkPicture() {
  QDir dir(QCoreApplication::applicationDirPath());

  if(resultFilesList.empty()){
    cout << "check finish" << endl;

    QFile logFile(dir.filePath("all_tests_run.log"));
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text))
      return;
    QTextStream in(&logFile);
    while (!in.atEnd()) {
      QString line = in.readLine();
      if(line.contains("test OK ***"))
	okTestsList.push_back(line);
    }


    cout << endl << "Result : " << endl;
    cout << ">>> " << okTestsList.size()+failedTestsList.size() << " tests passed" << endl << endl;
    
    if(failedTestsList.size()!=0)
      cout << "!!! " << failedTestsList.size() << " tests failed" << endl;

    for(QStringList::iterator it=failedTestsList.begin();it!=failedTestsList.end();++it){
      cout << "*** " << (*it).toStdString() << " : Failed ***" << endl;
    }
    
    exit(0);
  }

  QString file=resultFilesList.front();
  resultFilesList.pop_front();

  if(modelFilesList.contains(file)){
    currentTest = file;
    currentTest.remove(".jpg");

    QString modelFileStr(dir.filePath(QString("model_")+file));
    QString resultFileStr(dir.filePath(QString("result_")+file));
    
    QProcess diff;
    diff.start("diff", QStringList() << modelFileStr << resultFileStr);
    if (!diff.waitForStarted())
      cout << "error in start diff" << endl;
    
    if (!diff.waitForFinished())
      cout << "error in end diff" << endl;
    
    QByteArray result = diff.readAll();
    
    if(result.size()!=0){
      QPixmap modelPicture,resultPicture;
      modelPicture.load(dir.filePath(modelFileStr));
      resultPicture.load(dir.filePath(resultFileStr));
      modelLabel->setPixmap(modelPicture);
      testResultLabel->setPixmap(resultPicture);
      testNameLabel->setText(currentTest);
    }else{
      sameAndRejectButtonClicked();
    }
  }
}

void ComparePictureMainWindow::differentButtonClicked(){
  failedTestsList.push_back(currentTest);
  checkPicture();
}

void ComparePictureMainWindow::sameAndRejectButtonClicked(){
  okTestsList.push_back(currentTest);
  checkPicture();
}

