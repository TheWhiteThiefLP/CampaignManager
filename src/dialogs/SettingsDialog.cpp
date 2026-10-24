#include "SettingsDialog.h"
#include "Settings.h"
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    ui.setupUi(this);
    ui.pathEdit->setText(Settings::instance().campaignPath());

    connect(ui.browseBtn, &QPushButton::clicked, this, &SettingsDialog::onBrowse);
    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onAccepted);
    connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void SettingsDialog::onBrowse() {
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Select Campaign Folder", ui.pathEdit->text());
    if (!dir.isEmpty())
        ui.pathEdit->setText(dir);
}

void SettingsDialog::onAccepted() {
    const QString path = ui.pathEdit->text().trimmed();
    if (!path.isEmpty())
        Settings::instance().setCampaignPath(path);
    accept();
}