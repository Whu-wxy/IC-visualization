#ifndef MAINDLG_H
#define MAINDLG_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QPolygon>
#include <QtAlgorithms>
#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>

#include <QDebug>

#include "imagewidget.h"
#include "utils.h"

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    MainDlg(QWidget *parent = nullptr);
    ~MainDlg();

protected:
    void    setupUI();
    void    initial();
    void    setupConnection();

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    bool    processGT(bool newDir);
    bool    processPred(bool newDir);
    void    changeNum();
    void    ListSort(int);

private:
    ImgWidget*      m_imgLabel;
    QPushButton*    m_ChooseImgDirBtn;
    QPushButton*    m_ChooseGTDirBtn;
    QPushButton*    m_ChoosePredDirBtn;
    QComboBox*      m_dataTypeBox;
    QCheckBox*      m_ShowGTBox;
    QCheckBox*      m_markInterBox;
    QCheckBox*      m_FiltGTBox;
    QCheckBox*      m_ShowPredBox;

    QLineEdit*      m_pageEdit;
    QPushButton*    m_pageBtn;
    QLabel*         m_numLab;
    QPushButton*    m_UpBtn;
    QPushButton*    m_DownBtn;
    QPushButton*    m_SaveBtn;
    QComboBox*      m_styleBox;

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
    void    onShowInter(int);
    void    onFiltGT(int);
    void    onShowPred(int);
    void    onChangePage();
    void    onUp();
    void    onDown();
    void    onSave();
    void    onChangeStyle(int index);

};
#endif // MAINDLG_H
