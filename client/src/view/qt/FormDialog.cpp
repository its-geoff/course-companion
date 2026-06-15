#include "view/qt/FormDialog.hpp"

/**
 * @file FormDialog.cpp
 * @brief Implementation of FormDialog, a generic input dialog for add/edit operations.
 *
 * Builds form widgets from FieldDef descriptors at construction time. Validation runs
 * before the dialog closes to prevent empty required fields from being accepted.
 */

#include <QMessageBox>
#include <QVBoxLayout>

FormDialog::FormDialog(const QString& title,
                       const std::vector<FieldDef>& fields,
                       QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    setMinimumWidth(360);

    auto* outer = new QVBoxLayout(this);
    auto* form  = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);
    form->setSpacing(10);
    outer->addLayout(form);

    for (const auto& field : fields) {
        int idx = static_cast<int>(widgets_.size());
        keyIndex_[field.key] = idx;
        widgets_.push_back({field.type, nullptr,
                            field.type == FieldDef::Type::Text});

        switch (field.type) {
            case FieldDef::Type::Text:
            case FieldDef::Type::OptionalText:
                addTextField(form, field);
                break;
            case FieldDef::Type::Date:
                addDateField(form, field);
                break;
            case FieldDef::Type::Integer:
                addIntField(form, field);
                break;
            case FieldDef::Type::Bool:
                addBoolField(form, field);
                break;
        }
    }

    outer->addSpacing(8);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    outer->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &FormDialog::onAccepted);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void FormDialog::addTextField(QFormLayout* layout, const FieldDef& field) {
    auto* edit = new QLineEdit(this);
    edit->setPlaceholderText(field.type == FieldDef::Type::OptionalText ? "optional" : "");
    if (field.defaultValue.isValid())
        edit->setText(field.defaultValue.toString());

    widgets_.back().widget = edit;
    layout->addRow(field.label + ":", edit);
}

void FormDialog::addDateField(QFormLayout* layout, const FieldDef& field) {
    auto* edit = new QDateEdit(this);
    edit->setCalendarPopup(true);
    edit->setDisplayFormat("yyyy-MM-dd");
    edit->setDate(field.defaultValue.isValid()
                  ? field.defaultValue.toDate()
                  : QDate::currentDate());

    widgets_.back().widget = edit;
    layout->addRow(field.label + ":", edit);
}

void FormDialog::addIntField(QFormLayout* layout, const FieldDef& field) {
    auto* spin = new QSpinBox(this);
    spin->setMinimum(0);
    spin->setMaximum(99);
    spin->setValue(field.defaultValue.isValid() ? field.defaultValue.toInt() : 0);

    widgets_.back().widget = spin;
    layout->addRow(field.label + ":", spin);
}

void FormDialog::addBoolField(QFormLayout* layout, const FieldDef& field) {
    auto* check = new QCheckBox(this);
    check->setChecked(field.defaultValue.isValid() ? field.defaultValue.toBool() : false);

    widgets_.back().widget = check;
    layout->addRow(field.label + ":", check);
}

bool FormDialog::validateRequired() {
    for (const auto& entry : widgets_) {
        if (!entry.required)
            continue;
        auto* edit = qobject_cast<QLineEdit*>(entry.widget);
        if (edit && edit->text().trimmed().isEmpty())
            return false;
    }
    return true;
}

void FormDialog::onAccepted() {
    if (!validateRequired()) {
        QMessageBox::warning(this, "Missing required fields", "Please fill in all required fields before continuing.");
        return;
    }
    accept();
}

QString FormDialog::textValue(const QString& key) const {
    auto it = keyIndex_.find(key);
    if (it == keyIndex_.end()) return {};
    auto* edit = qobject_cast<QLineEdit*>(widgets_[it.value()].widget);
    return edit ? edit->text().trimmed() : QString{};
}

QDate FormDialog::dateValue(const QString& key) const {
    auto it = keyIndex_.find(key);
    if (it == keyIndex_.end()) return {};
    auto* edit = qobject_cast<QDateEdit*>(widgets_[it.value()].widget);
    return edit ? edit->date() : QDate{};
}

int FormDialog::intValue(const QString& key) const {
    auto it = keyIndex_.find(key);
    if (it == keyIndex_.end()) return 0;
    auto* spin = qobject_cast<QSpinBox*>(widgets_[it.value()].widget);
    return spin ? spin->value() : 0;
}

bool FormDialog::boolValue(const QString& key) const {
    auto it = keyIndex_.find(key);
    if (it == keyIndex_.end()) return false;
    auto* check = qobject_cast<QCheckBox*>(widgets_[it.value()].widget);
    return check ? check->isChecked() : false;
}