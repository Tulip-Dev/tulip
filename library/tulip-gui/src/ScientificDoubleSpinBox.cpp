// original code is written in Python and is taken from
// https://jdreaver.com/posts/2014-07-28-scientific-notation-spin-box-pyside.html
// see also https://gist.github.com/jdreaver/0be2e44981159d0854f5

#include <cfloat>

#include <tulip/ScientificDoubleSpinBox.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TlpQtTools.h>

#include <QRegularExpression>
#include <QLineEdit>

using namespace tlp;

static QRegularExpression floatRe("(([+-]?\\d+(\\.\\d*)?|\\.\\d+)([eE][+-]?\\d+)?)");
static FloatValidator floatValidator;

bool validFloatString(const QString &s) {
  QRegularExpressionMatch match;
  auto pos = s.indexOf(floatRe, 0, &match);

  if (pos != -1) {
    return match.captured(1) == s;
  } else {
    return false;
  }
}

QValidator::State FloatValidator::validate(QString &input, int &pos) const {
  if (validFloatString(input)) {
    return QValidator::Acceptable;
  }

  if (input.isEmpty() || input.mid(pos - 1, 1).count(QRegularExpression("[e.-+]+")) == 1) {
    return QValidator::Intermediate;
  }

  return QValidator::Invalid;
}

void FloatValidator::fixup(QString &text) const {
  QRegularExpressionMatch match;
  auto pos = text.indexOf(floatRe, 0, &match);

  if (pos != -1) {
    text = match.captured(1);
  } else {
    text = "";
  }
}

ScientificDoubleSpinBox::ScientificDoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {
  setMinimum(-DBL_MAX);
  setMaximum(DBL_MAX);
  setDecimals(1000);
}

QValidator::State ScientificDoubleSpinBox::validate(QString &input, int &pos) const {
  return floatValidator.validate(input, pos);
}

void ScientificDoubleSpinBox::fixup(QString &input) const {
  floatValidator.fixup(input);
}

double ScientificDoubleSpinBox::valueFromText(const QString &text) const {
  double val = 0;
  tlp::DoubleType::fromString(val, QStringToTlpString(text));
  return val;
}

QString ScientificDoubleSpinBox::textFromValue(double value) const {
  return tlpStringToQString(tlp::DoubleType::toString(value));
}

void ScientificDoubleSpinBox::stepBy(int steps) {
  QString text = cleanText();
  QRegularExpressionMatch match;
  auto pos = text.indexOf(floatRe, 0, &match);
  if (pos != -1) {
    QStringList groups = match.capturedTexts();
    double decimal = 0;
    tlp::DoubleType::fromString(decimal, QStringToTlpString(groups[2]));
    decimal += steps;
    text = tlpStringToQString(tlp::DoubleType::toString(decimal));
    if (groups.count() > 4) {
      text += groups[4];
    }
  }
  lineEdit()->setText(text);
}
