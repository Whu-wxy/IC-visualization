#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QPolygon>

#include <QDebug>

#include "tiffimg.h"

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    MainDlg(QWidget *parent = nullptr);
    ~MainDlg();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    QPolygon    readICLabel(QString);

    bool    processGT();
    bool    processPred();

private:
    tiffImg*        m_imgLabel;
    QPushButton*    m_ChooseImgDirBtn;
    QPushButton*    m_ChooseGTDirBtn;
    QPushButton*    m_ChoosePredDirBtn;
    QCheckBox*      m_ShowGTBox;
    QCheckBox*      m_ShowPredBox;
    QPushButton*    m_UpBtn;
    QPushButton*    m_DownBtn;
    QPushButton*    m_SaveBtn;

    int     m_iIndex;
    QString m_curFileName;
    QDir    m_imgFileDir;
    QStringList m_fileNameList;

    QString     m_GTDir;
    QStringList m_GTNameList;

    QString     m_PredDir;
    QStringList m_PredNameList;

    bool    m_bShowGT;
    bool    m_bShowPred;

protected slots:
    void    onChooseImgFile();
    void    onChooseGTFile();
    void    onChoosePredFile();
    void    onShowGT(int);
    void    onShowPred(int);
    void    onUp();
    void    onDown();
    void    onSave();

};
#endif // MAINDLG_H
