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
    m_ChooseGTDirBtn = new QPushButton("选择GT文件夹");
    m_ChoosePredDirBtn = new QPushButton("选择Pred文件夹");
    m_ShowGTBox = new QCheckBox("显示GT", this);
    m_FiltGTBox = new QCheckBox("过滤GT", this);
    m_ShowPredBox = new QCheckBox("显示Pred",this);
    m_ShowGTBox->setChecked(true);
    m_FiltGTBox->setChecked(false);
    m_ShowPredBox->setChecked(true);
    m_bShowGT = true;
    m_bShowPred = true;
    m_numLab = new QLabel("0/0", this);
    m_numLab->setAlignment(Qt::AlignCenter);
    m_UpBtn = new QPushButton("上一个");
    m_DownBtn = new QPushButton("下一个");
    m_SaveBtn = new QPushButton("保存");
    m_styleBox = new QComboBox(this);
    m_styleBox->addItem("白色");
    m_styleBox->addItem("黑色");
    m_styleBox->addItem("蓝色");

    m_imgLabel->setFocusPolicy(Qt::NoFocus);
    m_ChooseImgDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChooseGTDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChoosePredDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ShowGTBox->setFocusPolicy(Qt::NoFocus);
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
    rightLay->addWidget(m_ChooseGTDirBtn);
    rightLay->addWidget(m_ChoosePredDirBtn);
    rightLay->addWidget(m_ShowGTBox);
    rightLay->addWidget(m_FiltGTBox);
    rightLay->addWidget(m_ShowPredBox);
    rightLay->addStretch();
    rightLay->addWidget(m_styleBox);
    rightLay->addWidget(m_SaveBtn);
    rightLay->addStretch();
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
    connect(m_FiltGTBox, SIGNAL(stateChanged(int)), this, SLOT(onFiltGT(int)));
    connect(m_ShowPredBox, SIGNAL(stateChanged(int)), this, SLOT(onShowPred(int)));
    connect(m_SaveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(m_UpBtn, SIGNAL(clicked()), this, SLOT(onUp()));
    connect(m_DownBtn, SIGNAL(clicked()), this, SLOT(onDown()));
    connect(m_styleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeStyle(int)));
}

void MainDlg::keyPressEvent(QKeyEvent *event)
{
    if(m_curFileName.length() == 0)
        return QDialog::keyPressEvent(event);
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
    {
        onUp();
    }
    else if(event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)
    {
        onDown();
    }
    else
        return QDialog::keyPressEvent(event);

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

QPolygon MainDlg::readICLabel(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    if(label.contains(","))  //IC15 | IC13-TEST
    {
        labelList = label.split(",");

        count = labelList.count();

        if(labelList.count() > 8)
            count -= 1;
        else if(count == 5)
            count -= 1;
    }
    else
    {
        labelList = label.split(" ");  //IC13
        count = labelList.count();
        if(labelList.count() > 4)
            count -= 1;
    }

    if(count == 8)              //IC15
    {
        for(int i=0; i<count; i++)
        {
            if(i % 2 == 0)
            {
                pt.setX(labelList.at(i).toInt());
            }
            else if(i % 2 == 1)
            {
                pt.setY(labelList.at(i).toInt());
                polygon.append(pt);
            }
        }
    }
    else if(count == 4)         //IC13
    {
        QPoint topLeft, bottomRight;
        topLeft.setX(labelList.at(0).toInt());
        topLeft.setY(labelList.at(1).toInt());
        bottomRight.setX(labelList.at(2).toInt());
        bottomRight.setY(labelList.at(3).toInt());
        QRect rect(topLeft,bottomRight);
        polygon = QPolygon(rect);
    }


    return  polygon;
}

bool MainDlg::processGT()
{
    if(m_GTDir.length() == 0)
        return false;
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
                if(!QChar(line.at(0)).isDigit())
                    line = line.remove(0, 3);
                QPolygon poly = readICLabel(QString(line));

                if(line.endsWith("###"))
                    m_imgLabel->addFiltGTIndex(i);

                m_imgLabel->addPolygon(poly, true);
                i++;
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

bool MainDlg::processPred()
{
    if(m_PredDir.length() == 0)
        return false;
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
                QPolygon poly = readICLabel(QString(line));
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

    if(m_GTDir.length() != 0)
        processGT();
    if(m_PredDir.length() != 0)
        processPred();

    m_imgLabel->update();
}

void MainDlg::onChooseGTFile()
{
    m_GTDir = QFileDialog::getExistingDirectory(this, tr("选择GT文件路径"),"/");

    QDir dir(m_GTDir);
    m_GTNameList.clear();
    m_GTNameList = dir.entryList(QDir::Files, QDir::Name);
    ListSort(1);

    processGT();
    m_imgLabel->update();
}

void MainDlg::onChoosePredFile()
{
    m_PredDir = QFileDialog::getExistingDirectory(this, tr("选择Pred文件路径"),"/");

    QDir dir(m_PredDir);
    m_PredNameList.clear();
    m_PredNameList = dir.entryList(QDir::Files, QDir::Name);
    ListSort(2);

    processPred();
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
                QString filePath = m_imgFileDir.path()+QDir::separator()+m_curFileName;
                if(QFile::exists(filePath))
                    m_imgLabel->setImage(filePath);

                if(m_bShowGT)
                {
                    processGT();
                }
                if(m_bShowPred)
                {
                    processPred();
                }

                return;
            }
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
            if(m_fileNameList.count() >i)
            {
                m_iIndex += 1;
                changeNum();

                m_curFileName = m_fileNameList.at(i+1);
                QString filePath = m_imgFileDir.path()+QDir::separator()+m_curFileName;
                if(QFile::exists(filePath))
                    m_imgLabel->setImage(filePath);

                if(m_bShowGT)
                {
                    processGT();
                }
                if(m_bShowPred)
                {
                    processPred();
                }

                return;
            }
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
    m_imgLabel->saveResult(savePath.path()+QDir::separator()+"Contrast_Save", m_iIndex);
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
