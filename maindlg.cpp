#include "maindlg.h"

MainDlg::MainDlg(QWidget *parent)
    : QDialog(parent)
{
    m_curFileName = "";
    m_PredDir = "";
    m_GTDir = "";
    m_iIndex = 0;

    setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

    setWindowState(Qt::WindowMaximized);

    m_imgLabel = new tiffImg(this);
    m_ChooseImgDirBtn = new QPushButton("选择图片文件");
    m_ChooseGTDirBtn = new QPushButton("选择GT文件夹");
    m_ChoosePredDirBtn = new QPushButton("选择Pred文件夹");
    m_ShowGTBox = new QCheckBox("显示GT", this);
    m_ShowPredBox = new QCheckBox("显示Pred",this);
    m_ShowGTBox->setChecked(true);
    m_ShowPredBox->setChecked(true);
    m_bShowGT = true;
    m_bShowPred = true;
    m_UpBtn = new QPushButton("上一个");
    m_DownBtn = new QPushButton("下一个");
    m_SaveBtn = new QPushButton("保存");

    m_imgLabel->setFocusPolicy(Qt::NoFocus);
    m_ChooseImgDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChooseGTDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ChoosePredDirBtn->setFocusPolicy(Qt::NoFocus);
    m_ShowGTBox->setFocusPolicy(Qt::NoFocus);
    m_ShowPredBox->setFocusPolicy(Qt::NoFocus);
    m_UpBtn->setFocusPolicy(Qt::NoFocus);
    m_DownBtn->setFocusPolicy(Qt::NoFocus);
    m_SaveBtn->setFocusPolicy(Qt::NoFocus);

    QGroupBox* groupBox = new QGroupBox(this);

    QVBoxLayout* rightLay = new QVBoxLayout;
    rightLay->addWidget(m_ChooseImgDirBtn);
    rightLay->addWidget(m_ChooseGTDirBtn);
    rightLay->addWidget(m_ChoosePredDirBtn);
    rightLay->addWidget(m_ShowGTBox);
    rightLay->addWidget(m_ShowPredBox);
    rightLay->addStretch();
    rightLay->addWidget(m_SaveBtn);
    rightLay->addStretch();
    rightLay->addWidget(m_UpBtn);
    rightLay->addWidget(m_DownBtn);

    groupBox->setLayout(rightLay);

    QHBoxLayout* mainLay = new QHBoxLayout(this);
    mainLay->addWidget(m_imgLabel);
    mainLay->addWidget(groupBox);
    mainLay->setStretch(0, 20);
    mainLay->setStretch(1, 1);

    connect(m_ChooseImgDirBtn, SIGNAL(clicked()), this, SLOT(onChooseImgFile()));
    connect(m_ChooseGTDirBtn, SIGNAL(clicked()), this, SLOT(onChooseGTFile()));
    connect(m_ChoosePredDirBtn, SIGNAL(clicked()), this, SLOT(onChoosePredFile()));
    connect(m_ShowGTBox, SIGNAL(stateChanged(int)), this, SLOT(onShowGT(int)));
    connect(m_ShowPredBox, SIGNAL(stateChanged(int)), this, SLOT(onShowPred(int)));
    connect(m_SaveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(m_UpBtn, SIGNAL(clicked()), this, SLOT(onUp()));
    connect(m_DownBtn, SIGNAL(clicked()), this, SLOT(onDown()));
}

MainDlg::~MainDlg()
{

}

void MainDlg::keyPressEvent(QKeyEvent *event)
{
    if(m_curFileName.length() == 0)
        return;
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
    {
        onUp();
    }
    else if(event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)
    {
        onDown();
    }

    // 是否按下Ctrl键      特殊按键
    if(event->modifiers() == Qt::ControlModifier)
    {
        // 是否按下M键    普通按键  类似
        if(event->key() == Qt::Key_S)
        {
            onSave();
        }

    }
    //else QWidget::keyPressEvent(event);   //保存默认事件
}

QPolygon MainDlg::readICLabel(QString label)
{
    QStringList labelList = label.split(",");
    QPolygon polygon;
    QPoint pt;
    int count = labelList.count();
    if(labelList.count() > 8)
        count -= 1;
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
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                line = line.simplified();
                if(!QChar(line.at(0)).isDigit())
                    line = line.remove(0, 3);
                QPolygon poly = readICLabel(QString(line));
                m_imgLabel->addPolygon(poly, true);
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

void MainDlg::onChooseImgFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("选择图片"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    QFileInfo info(fileName);
    m_curFileName = info.fileName();
    m_imgFileDir.setPath(info.path());
    m_fileNameList.clear();
    m_fileNameList = m_imgFileDir.entryList(QDir::Files, QDir::Name);

    for(int i=0; i<m_fileNameList.count(); i++)
    {
        if(m_fileNameList.at(i) == m_curFileName)
        {
            m_iIndex = i;
            break;
        }
    }

    m_imgLabel->setImage(fileName);

    if(m_GTDir.length() != 0)
        processGT();
    if(m_PredDir.length() != 0)
        processPred();
}

void MainDlg::onChooseGTFile()
{
    m_GTDir = QFileDialog::getExistingDirectory(this, tr("选择GT文件路径"),"/");

    QDir dir(m_GTDir);
    m_GTNameList.clear();
    m_GTNameList = dir.entryList(QDir::Files, QDir::Name);

    processGT();
}

void MainDlg::onChoosePredFile()
{
    m_PredDir = QFileDialog::getExistingDirectory(this, tr("选择Pred文件路径"),"/");

    QDir dir(m_PredDir);
    m_PredNameList.clear();
    m_PredNameList = dir.entryList(QDir::Files, QDir::Name);

    processPred();
}

void MainDlg::onShowGT(int state)
{
    if(state == 0)
    {
        m_bShowGT = false;
        m_imgLabel->m_bShowGT = false;
    }
    else if(state == 1)
    {
        m_bShowGT = true;
        m_imgLabel->m_bShowGT = true;
    }
}

void MainDlg::onShowPred(int state)
{
    if(state == 0)
    {
        m_bShowPred = false;
        m_imgLabel->m_bShowPred = false;
    }
    else if(state == 1)
    {
        m_bShowPred = true;
        m_imgLabel->m_bShowPred = true;
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
    if(m_curFileName.length() == 0)
        return;
}
