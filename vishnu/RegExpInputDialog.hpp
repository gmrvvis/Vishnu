#ifndef VISHNU_REGEXPINPUTDIALOG_HPP
#define VISHNU_REGEXPINPUTDIALOG_HPP

#include <QDialog>

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QRegularExpressionValidator>

namespace vishnu
{

  //class QLabel;
  //class QLineEdit;
  //class QDialogButtonBox;
  //class QRegExpValidator;

  class RegExpInputDialog : public QDialog
  {

  Q_OBJECT
  public:
    explicit RegExpInputDialog(QWidget *parent = 0, Qt::WindowFlags=0);

    void setTitle(const QString &title);
    void setLabelText(const QString &label);
    void setText(const QString &text);
    void setRegularExpression(const QRegularExpression &regularExpression);

    QString getLabelText();
    QString getText();

    static QString getText(QWidget *parent, const QString &title,
      const QString &label, const QString &text,
      const QRegularExpression &regularExpression, bool *ok,
      Qt::WindowFlags flags=0);
  signals:

  private slots:
    void checkValid(const QString &text);

  private:

    QLabel* _label;
    QLineEdit* _text;
    QDialogButtonBox* _buttonBox;
    QRegularExpression _regularExpression;
    QRegularExpressionValidator* _validator;

  };

}

#endif
