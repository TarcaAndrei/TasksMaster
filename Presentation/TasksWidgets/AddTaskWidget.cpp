//
// Created by Andrei on 30.08.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddTaskWidget.h" resolved

#include "AddTaskWidget.h"
#include "ui_AddTaskWidget.h"


AddTaskWidget::AddTaskWidget(ServiceApp &serviceApp,QWidget *parent) :
        QWidget(parent), ui(new Ui::AddTaskWidget), serviceApp(serviceApp) {
    ui->setupUi(this);
    this->load_connections();
    this->refresh_form();
}

AddTaskWidget::~AddTaskWidget() {
    delete ui;
}

void AddTaskWidget::load_connections() {
    //fara vector
    this->ui->closeButton->setIcon(QIcon(":/Icons/exit.png"));
    this->ui->closeButton->setFlat(true);
    this->ui->prioritycombo->addItem(QIcon(":/Icons/n_priority"), "None");
    this->ui->prioritycombo->addItem(QIcon(":/Icons/m_priority"), "Medium");
    this->ui->prioritycombo->addItem(QIcon(":/Icons/h_priority"), "High");
    this->ui->prioritycombo->setCurrentIndex(0);
    QObject::connect(this->ui->saveTask, &QPushButton::clicked, [&](){
        auto name_table = this->ui->list_combo->currentText().toStdString();
        auto id_table = this->ui->list_combo->currentData().toInt();
//        qDebug()<<name_table<<id_table;
        auto name_txt = this->ui->nameLineEdit->text().toStdString();
        auto details_txt = this->ui->detailsEdit->toPlainText().toStdString();
        QDateTime dateTime;
        auto time_due_txt = this->ui->timeEdit->time();
        auto date_due_txt = this->ui->calendarWidget->selectedDate();
        dateTime.setTime(time_due_txt);
        dateTime.setDate(date_due_txt);
        auto priority_txt = this->ui->prioritycombo->currentText().toStdString();
        ///aici la priority sa setez un combobox o lista dinaia cu cateva optiuni
        this->serviceApp.add_new_task(id_table, name_txt, details_txt, dateTime, priority_txt);
        this->refresh_form();
        this->notify_all(ADD_DONE);
    });
    QObject::connect(this->ui->closeButton, &QPushButton::clicked, [&](){
        QMessageBox msgBox;
        msgBox.setText("Are you sure you want to exit?");
        msgBox.setInformativeText("The information that you have just written will be lost!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/Icons/edit.png"));
        msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
            default:
                break;
        }
        this->notify_all(ADD_DONE);
    });
    this->ui->saveTask->setIcon(QIcon(":/Icons/plus.png"));
}

void AddTaskWidget::refresh_form() {
    this->ui->timeEdit->clear();
    this->ui->calendarWidget->setDateEditEnabled(true);
    this->ui->calendarWidget->setSelectedDate(this->add1hour().date());
    this->ui->timeEdit->setTime(this->add1hour().time());
    this->ui->nameLineEdit->clear();
    this->ui->detailsEdit->clear();
    //aici clear option sau ce idk trebe sa adaug listele posibile;
    //apoi optiunile posibile
    this->ui->list_combo->clear();
    auto lista_liste = this->serviceApp.get_all_lists();
    for(const auto& it : lista_liste){
//        qDebug()<<"Ceva";
        this->ui->list_combo->addItem(QString::fromStdString(it.second), it.first);
    }
    this->ui->list_combo->setCurrentIndex(0);
    //
}

QDateTime AddTaskWidget::add1hour() {
    auto crt_date_time = QDateTime::currentDateTime();
    auto time_only = crt_date_time.time();
    if(time_only.hour() == 23){
        crt_date_time = crt_date_time.addDays(1);
        time_only.setHMS(0, 0, 0, 0);
    }
    else{
        time_only.setHMS(time_only.hour()+1, 0, 0, 0);
    }
    crt_date_time.setTime(time_only);
    return crt_date_time;
}

