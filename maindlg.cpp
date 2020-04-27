#include "maindlg.h"

MainDlg::MainDlg(QWidget *parent)
    : QDialog(parent)
{
    QWidget::setWindowIcon(QIcon(":/icon.ico"));

    //加载样式表
    QFile file(":/qss/flatwhite.css");
    if(file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    initial();
    setupUI();
    setupConnection();
}

MainDlg::~MainDlg()
{

}

void MainDlg::initial()
{
    m_curFileName = "";
    m_PredDir = "";
    m_GTDir = "";
    m_iIndex = 0;

    setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

    setWindowState(Qt::WindowMaximized);
}

void MainDlg::setupUI()
{
    m_imgLabel = new ImgWidget(this);
    m_ChooseImgDirBtn = new QPushButton("选择图片文件");
    m_dataTypeBox = new QComboBox(this);
    m_dataTypeBox->addItem("IC15/13");
    m_dataTypeBox->addItem("CTW");
    m_dataTypeBox->addItem("TotalText");
    m_dataTypeBox->addItem("IC17MLT");
    m_ChooseGTDirBtn = new QPushButton("选择GT文件夹");
    m_ChoosePredDirBtn = new QPushButton("选择Pred文件夹");
    m_ShowGTBox = new QCheckBox("显示GT", this);
    m_markInterBox = new QCheckBox("标出标签交叉区域", this);
    m_FiltGTBox = new QCheckBox("过滤GT", this);
    m_ShowPredBox = new QCheckBox("显示Pred",this);
    m_ShowGTBox->setChecked(true);
    m_markInterBox->setChecked(true);
    m_FiltGTBox->setChecked(false);
    m_ShowPredBox->setChecked(true);
    m_bShowGT = true;
    m_bShowPred = true;

    m_pageEdit = new QLineEdit(this);
    m_pageEdit->setPlaceholderText("输入页码跳转");
    m_pageBtn = new QPushButton("跳转", this);
    m_numLab = new QLabel("0/0", this);
    m_numLab->setAlignment(Qt::AlignCenter);
    m_UpBtn = new QPushButton("上一个");
    m_DownBtn = new QPushButton("下一个");
    m_SaveBtn = new QPushButton("保存");
    m_styleBox = new QComboBox(this);
    m_styleBox->addItem("白色");
    m_styleBox->addItem("黑色");
    m_styleBox->addItem("蓝色");

    m_pageEdit->setFocusPolicy(Qt::ClickFocus);
    m_pageBtn->setFocusPolicy(Qt::NoFocus);
    m_imgLabel->setFocusPolicy(Qt::NoFocus);
    m_ChooseImgDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChooseGTDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChoosePredDirBtn->setFocusPolicy(Qt::NoFocus);
    m_dataTypeBox->setFocusPolicy(Qt::NoFocus);
    m_ShowGTBox->setFocusPolicy(Qt::NoFocus);
    m_markInterBox->setFocusPolicy(Qt::NoFocus);
    m_FiltGTBox->setFocusPolicy(Qt::NoFocus);
    m_ShowPredBox->setFocusPolicy(Qt::NoFocus);
    m_numLab->setFocusPolicy(Qt::NoFocus);
    m_UpBtn->setFocusPolicy(Qt::NoFocus);
    m_DownBtn->setFocusPolicy(Qt::NoFocus);
    m_SaveBtn->setFocusPolicy(Qt::NoFocus);
    m_styleBox->setFocusPolicy(Qt::NoFocus);

    QGroupBox* groupBox = new QGroupBox(this);

    QVBoxLayout* rightLay = new QVBoxLayout;
    rightLay->addWidget(m_ChooseImgDirBtn);
    rightLay->addWidget(m_dataTypeBox);
    rightLay->addWidget(m_ChooseGTDirBtn);
    rightLay->addWidget(m_ChoosePredDirBtn);
    rightLay->addWidget(m_ShowGTBox);
    rightLay->addWidget(m_markInterBox);
    rightLay->addWidget(m_FiltGTBox);
    rightLay->addWidget(m_ShowPredBox);
    rightLay->addStretch();
    rightLay->addWidget(m_styleBox);
    rightLay->addWidget(m_SaveBtn);
    rightLay->addStretch();
    rightLay->addWidget(m_pageEdit);
    rightLay->addWidget(m_pageBtn);
    rightLay->addWidget(m_numLab);
    rightLay->addWidget(m_UpBtn);
    rightLay->addWidget(m_DownBtn);

    groupBox->setLayout(rightLay);

    QHBoxLayout* mainLay = new QHBoxLayout(this);
    mainLay->addWidget(m_imgLabel);
    mainLay->addWidget(groupBox);
    mainLay->setStretch(0, 20);
    mainLay->setStretch(1, 1);
}

void MainDlg::setupConnection()
{
    connect(m_ChooseImgDirBtn, SIGNAL(clicked()), this, SLOT(onChooseImgFile()));
    connect(m_ChooseGTDirBtn, SIGNAL(clicked()), this, SLOT(onChooseGTFile()));
    connect(m_ChoosePredDirBtn, SIGNAL(clicked()), this, SLOT(onChoosePredFile()));
    connect(m_ShowGTBox, SIGNAL(stateChanged(int)), this, SLOT(onShowGT(int)));
    connect(m_markInterBox, SIGNAL(stateChanged(int)), this, SLOT(onShowInter(int)));
    connect(m_FiltGTBox, SIGNAL(stateChanged(int)), this, SLOT(onFiltGT(int)));
    connect(m_ShowPredBox, SIGNAL(stateChanged(int)), this, SLOT(onShowPred(int)));
    connect(m_SaveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(m_UpBtn, SIGNAL(clicked()), this, SLOT(onUp()));
    connect(m_DownBtn, SIGNAL(clicked()), this, SLOT(onDown()));
    connect(m_styleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeStyle(int)));
    connect(m_pageBtn, SIGNAL(clicked()), this, SLOT(onChangePage()));
}

void MainDlg::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
    {
        onUp();
    }
    else if(event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)
    {
        onDown();
    }
    //    else
    //        return QDialog::keyPressEvent(event);

    // 是否按下Ctrl键      特殊按键
    if(event->modifiers() == Qt::ControlModifier)
    {
        // 是否按下M键    普通按键  类似
        if(event->key() == Qt::Key_S)
        {
            onSave();
        }
        else
            return QDialog::keyPressEvent(event);
    }
    else
        return QDialog::keyPressEvent(event);
}

void MainDlg::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
}


bool MainDlg::processGT(bool newDir)
{
    if(m_GTDir.length() == 0)
        return false;

    if(newDir)
        m_imgLabel->clearGTList();

    int dataType = m_dataTypeBox->currentIndex();
    if(m_iIndex < m_GTNameList.count() && m_iIndex >= 0)
    {
        QFile file(m_GTDir+QDir::separator()+m_GTNameList.at(m_iIndex));
        if(file.open(QIODevice::ReadOnly))
        {
            int i=0;
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();

                line = line.simplified();
                line =line.trimmed();
//                if(QChar(line.at(0)) == '\n')
//                    line = line.remove(0, 1);
                if(!QChar(line.at(0)).isDigit())    // remove \xef\xbb\xbf
                    line = line.remove(0, 3);

                QPolygon poly;
                if(dataType == 0)
                {
                    if(line.count(",")>10)
                    {
                        QMessageBox::critical(this, "读取GT标签", "请选择正确的IC15/13的GT路径");
                        return false;
                    }
                    poly = readICLabel(QString(line));
                }
                else if(dataType == 1)
                {
                    if(line.count(",")<10)
                    {
                        QMessageBox::critical(this, "读取GT标签", "请选择正确的CTW的GT路径");
                        return false;
                    }
                    poly = readCTWTXTLabel(QString(line));
                     // poly = readCTWTXTE2ELabel(QString(line));
                }
                else if(dataType == 2)
                {
                    poly = readTotalText2ICLabel(QString(line));
                }
                else if(dataType == 3)
                {
                    poly = readIC17Label(QString(line));
                }

                if(line.endsWith("###") || line.endsWith("\"###\""))
                    m_imgLabel->addFiltGTIndex(i);

                m_imgLabel->addPolygon(poly, true);
                i++;
            }
            file.close();

            m_imgLabel->calInterPoly();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool MainDlg::processPred(bool newDir)
{
    if(m_PredDir.length() == 0)
        return false;

    if(newDir)
        m_imgLabel->clearPredList();

    int dataType = m_dataTypeBox->currentIndex();
    if(m_iIndex < m_PredNameList.count() && m_iIndex >= 0)
    {
        QFile file(m_PredDir+QDir::separator()+m_PredNameList.at(m_iIndex));
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                line = line.simplified();
                if(!QChar(line.at(0)).isDigit())
                    line = line.remove(0, 3);
                QPolygon poly;
                if(dataType == 0)
                {
                    if(line.count(",")>10)
                    {
                        QMessageBox::critical(this, "读取Pred标签", "请选择正确的IC15/13的Pred路径");
                        return false;
                    }
                    poly = readICLabel(QString(line));
                }
                else if(dataType == 1)
                {
                    if(line.count(",")<10)
                    {
                        QMessageBox::critical(this, "读取Pred标签", "请选择正确的CTW的Pred路径");
                        return false;
                    }
                    poly = readCTWTXTLabel(QString(line));
                }
                else if(dataType == 2)
                {
                    poly = readTotalText2ICLabel(QString(line));
                }
                else if(dataType == 3)
                {
                    poly = readIC17Label(QString(line));
                }

                m_imgLabel->addPolygon(poly, false);
            }
            file.close();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void MainDlg::changeNum()
{
    QString num = QString("%1/%2").arg(m_iIndex+1).arg(m_fileNameList.count());
    m_numLab->setText(num);
}

bool caseInsensitiveLessThan(QString s1, QString s2)
{
    QStringList s1List = s1.split("_");
    QStringList s2List = s2.split("_");
    s1 = s1List.at(s1List.count()-1);
    if(s1.endsWith(".jpg"))
        s1.remove(".jpg");
    else if(s1.endsWith(".txt"))
        s1.remove(".txt");
    s2 = s2List.at(s2List.count()-1);
    if(s2.endsWith(".jpg"))
        s2.remove(".jpg");
    else if(s2.endsWith(".txt"))
        s2.remove(".txt");

    return s1.toInt() < s2.toInt();
}

void MainDlg::ListSort(int index)
{
    if(index == 0)
    {
        std::sort(m_fileNameList.begin(), m_fileNameList.end(), caseInsensitiveLessThan);
    }
    else if(index == 1)
    {
        std::sort(m_GTNameList.begin(), m_GTNameList.end(), caseInsensitiveLessThan);
    }
    else if(index == 2)
    {
        std::sort(m_PredNameList.begin(), m_PredNameList.end(), caseInsensitiveLessThan);
    }
}



void MainDlg::onChooseImgFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("选择图片"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    QFileInfo info(fileName);
    m_curFileName = info.fileName();
    m_imgFileDir.setPath(info.path());
    m_fileNameList.clear();
    m_fileNameList = m_imgFileDir.entryList(QDir::Files, QDir::Name);
    ListSort(0);

    for(int i=0; i<m_fileNameList.count(); i++)
    {
        if(m_fileNameList.at(i) == m_curFileName)
        {
            m_iIndex = i;
            break;
        }
    }

    m_imgLabel->setImage(fileName);
    changeNum();

    m_imgLabel->clearGTList();
    m_imgLabel->clearPredList();

//    if(m_GTDir.length() != 0)
//        processGT(false);
//    if(m_PredDir.length() != 0)
//        processPred(false);

    m_imgLabel->update();
}

void MainDlg::onChooseGTFile()
{
    m_GTDir = QFileDialog::getExistingDirectory(this, tr("选择GT文件路径"),"/",QFileDialog::ShowDirsOnly);
    if(m_GTDir.length() == 0)
        return;

    QDir dir(m_GTDir);
    m_GTNameList.clear();
    QStringList nameFilters;
    nameFilters<<"*.txt";
    m_GTNameList = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    if(m_GTNameList.count() == 0)
    {
        QMessageBox::critical(this, "选择GT路径", "请选择正确的GT路径");
        return;
    }
    ListSort(1);

    processGT(true);
    m_imgLabel->update();
}

void MainDlg::onChoosePredFile()
{
    m_PredDir = QFileDialog::getExistingDirectory(this, tr("选择Pred文件路径"),"/");
    if(m_PredDir.length() == 0)
        return;

    QDir dir(m_PredDir);
    m_PredNameList.clear();
    QStringList nameFilters;
    nameFilters<<"*.txt";
    m_PredNameList = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    if(m_PredNameList.count() == 0)
    {
        QMessageBox::critical(this, "选择Pred路径", "请选择正确的Pred路径");
        return;
    }
    ListSort(2);

    processPred(true);
    m_imgLabel->update();
}

void MainDlg::onShowGT(int state)
{
    if(state == 0)
    {
        m_bShowGT = false;
        m_imgLabel->m_bShowGT = false;
        m_imgLabel->update();
    }
    else if(state == 2)
    {
        m_bShowGT = true;
        m_imgLabel->m_bShowGT = true;
        m_imgLabel->update();
    }
}

void MainDlg::onShowInter(int state)
{
    m_imgLabel->showInterPoly(state);
}

void MainDlg::onFiltGT(int state)
{
    if(state == 0)
    {
        m_imgLabel->m_bFiltGT = false;
        m_imgLabel->update();
    }
    else if(state == 2)
    {
        m_imgLabel->m_bFiltGT = true;
        m_imgLabel->update();
    }
}

void MainDlg::onShowPred(int state)
{
    if(state == 0)
    {
        m_bShowPred = false;
        m_imgLabel->m_bShowPred = false;
        m_imgLabel->update();
    }
    else if(state == 2)
    {
        m_bShowPred = true;
        m_imgLabel->m_bShowPred = true;
        m_imgLabel->update();
    }
}

void MainDlg::onUp()
{
    if(m_curFileName.length() == 0)
        return;

    for(int i=0; i<m_fileNameList.count(); i++)
    {
        if(m_fileNameList.at(i) == m_curFileName)
        {
            if(i > 0)
            {
                m_iIndex -= 1;
                changeNum();

                m_curFileName = m_fileNameList.at(i-1);
            }
            else if(i == 0)
            {
                m_iIndex = m_fileNameList.count()-1;
                changeNum();
                m_curFileName = m_fileNameList.at(m_fileNameList.count()-1);
            }
            else
                return;
            QString filePath = m_imgFileDir.path()+QDir::separator()+m_curFileName;
            if(QFile::exists(filePath))
                m_imgLabel->setImage(filePath);

            if(m_bShowGT)
            {
                processGT(false);
            }
            if(m_bShowPred)
            {
                processPred(false);
            }

            return;
        }
    }
}


void MainDlg::onDown()
{
    if(m_curFileName.length() == 0)
        return;

    for(int i=0; i<m_fileNameList.count(); i++)
    {
        if(m_fileNameList.at(i) == m_curFileName)
        {
            if(m_fileNameList.count()-1 > i)
            {
                m_iIndex += 1;
                changeNum();
                m_curFileName = m_fileNameList.at(i+1);
            }
            else if(i == m_fileNameList.count()-1)
            {
                m_iIndex = 0;
                changeNum();
                m_curFileName = m_fileNameList.at(0);
            }
            else
                return;

            QString filePath = m_imgFileDir.path()+QDir::separator()+m_curFileName;
            if(QFile::exists(filePath))
                m_imgLabel->setImage(filePath);

            if(m_bShowGT)
            {
                processGT(false);
            }
            if(m_bShowPred)
            {
                processPred(false);
            }

            return;
        }
    }
}


void MainDlg::onSave()
{
    if(m_curFileName.length() == 0 || m_imgFileDir.isEmpty() || !m_imgFileDir.exists())
        return;

    QDir savePath = m_imgFileDir;
    savePath.cdUp();

    savePath.mkdir("Contrast_Save");
    m_imgLabel->saveResult(savePath.path()+QDir::separator()+"Contrast_Save", m_iIndex+1);

    QMessageBox::information(this, "保存图片", "保存成功，保存于当前图片的上一级目录\"Contrast_save\"！");
}

void MainDlg::onChangeStyle(int index)
{
    QFile file;
    if(index == 0)
        file.setFileName(":/qss/flatwhite.css");
    else if(index == 1)
        file.setFileName(":/qss/psblack.css");
    else if(index == 2)
        file.setFileName(":/qss/lightblue.css");
    if(file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}


void MainDlg::onChangePage()
{
    this->setFocus();
    if(m_curFileName.length() == 0)
        return;

    int newPage = m_pageEdit->text().toInt();
    m_iIndex = newPage-1;
    changeNum();
    m_curFileName = m_fileNameList.at(m_iIndex);

    QString filePath = m_imgFileDir.path()+QDir::separator()+m_curFileName;
    if(QFile::exists(filePath))
        m_imgLabel->setImage(filePath);

    if(m_bShowGT)
    {
        processGT(false);
    }
    if(m_bShowPred)
    {
        processPred(false);
    }
}
