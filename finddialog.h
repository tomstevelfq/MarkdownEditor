#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include<QLayout>
#include<QMessageBox>
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>
#include<QCheckBox>
class FindDialog:public QDialog{
    Q_OBJECT
public:
    FindDialog(QWidget* parent=nullptr);
signals:
    void start_find(QString text,bool caseSensitive,bool wholeWords);
    void start_replace(QString what,QString with,bool caseSensitive,bool wholeWords);
    void start_replaceAll(QString what,QString with,bool caseSensitive,bool wholeWords);
public slots:
    void on_findNext();
    void on_replace();
    void on_findRequestReady(QString message);
private:
    QLabel* findLabel;
    QLabel* replaceLabel;
    QPushButton* findNextButton;
    QPushButton* replaceButton;
    QPushButton* replaceAllButton;
    QLineEdit* findLineEdit;
    QLineEdit* replaceLineEdit;
    QCheckBox* caseSensitiveBox;
    QCheckBox* wholeWordsBox;

    QHBoxLayout* findHorizontal;
    QHBoxLayout* replaceHorizontal;
    QHBoxLayout* optionsLayout;
    QVBoxLayout* verticalLayout;
};

#endif // FINDDIALOG_H
