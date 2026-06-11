#ifndef FORMDIALOG_HPP
#define FORMDIALOG_HPP

/**
 * @file FormDialog.hpp
 * @brief Definition of FormDialog, a generic input dialog for add/edit operations.
 *
 * Accepts a list of FieldDef descriptors at construction time and builds a form
 * layout dynamically. Callers retrieve typed values by field key after the dialog
 * is accepted.
 *
 * Provides declarations only; see FormDialog.cpp for implementations.
 */

#include <QCheckBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QVariant>

#include <vector>

struct FieldDef {
    enum class Type {
        Text,
        OptionalText,
        Date,
        Integer,
        Bool
    };

    QString  key;
    QString  label;
    Type     type;
    QVariant defaultValue;
};

class FormDialog : public QDialog {
    Q_OBJECT

public:
    explicit FormDialog(const QString& title,
                        const std::vector<FieldDef>& fields,
                        QWidget* parent = nullptr);

    QString textValue(const QString& key) const;
    QDate   dateValue(const QString& key) const;
    int     intValue(const QString& key)  const;
    bool    boolValue(const QString& key) const;

private slots:
    void onAccepted();

private:
    void addTextField(QFormLayout* layout, const FieldDef& field);
    void addDateField(QFormLayout* layout, const FieldDef& field);
    void addIntField(QFormLayout*  layout, const FieldDef& field);
    void addBoolField(QFormLayout* layout, const FieldDef& field);

    bool validateRequired();

    struct WidgetEntry {
        FieldDef::Type type;
        QWidget* widget  = nullptr;
        bool     required = false;
    };

    std::vector<WidgetEntry> widgets_;
    QMap<QString, int>       keyIndex_;
};

#endif // FORMDIALOG_HPP