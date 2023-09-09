//
// Created by Andrei on 26.08.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWidget.h" resolved

#include "MainWidget.h"
#include "ui_MainWidget.h"


MainWidget::MainWidget(ServiceApp &serviceApp, AddTaskWidget *addTaskWidget,
                       ViewTaskWidget *viewTaskWidget, QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWidget), serviceApp(serviceApp), addTaskWidget(addTaskWidget),
        viewTaskWidget(viewTaskWidget) {
    ui->setupUi(this);
    nr_lists_before = 0;
    this->serviceApp.add_follower(this);
    this->addTaskWidget->add_follower(this);
    this->viewTaskWidget->add_follower(this);
    this->load_widget();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::load_widget() {
//    this->addTaskWidget->set_parent(this);
    this->ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    this->ui->gridLayout->addWidget(this->addTaskWidget);
    this->addTaskWidget->setVisible(false);
    this->ui->gridLayout->addWidget(this->viewTaskWidget);
    this->viewTaskWidget->setVisible(false);
    this->load_lists();
//    this->clear_form();
}

void MainWidget::load_lists() {
//    this->ui->listView->setVisible(false);
//    this->create_list();
    this->myFirstModel = new MyFirstModel(this->serviceApp);
    this->ui->listView->setModel(myFirstModel);
    this->ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    this->mySecondModel = new MySecondModel(this->serviceApp);
    this->ui->listView_2->setModel(mySecondModel);
    this->ui->listView->viewport()->setAutoFillBackground(false);
    this->ui->listView_2->viewport()->setAutoFillBackground(false);
    this->ui->listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(this->ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, [&](){
        if(this->ui->listView->selectionModel()->selectedIndexes().isEmpty()){
            this->mySecondModel->set_list_id();
            this->close_edit_buttons();
            return;
        }
        auto selected = this->ui->listView->selectionModel()->selectedIndexes()[0];
        this->close_edit_buttons();
        this->ui->listView->indexWidget(myFirstModel->index(selected.row()))->setVisible(true);
        auto id_table = selected.data(Qt::UserRole).toInt();
        this->mySecondModel->set_list_id(id_table);
        return;
    });
    QObject::connect(this->ui->listView_2->selectionModel(), &QItemSelectionModel::selectionChanged, [&](){
        if(this->ui->listView_2->selectionModel()->selectedIndexes().isEmpty()){
            this->ui->add_new_task_button->setVisible(true);
            this->viewTaskWidget->setVisible(false);
            return;
        }
        auto selected = this->ui->listView_2->selectionModel()->selectedIndexes()[0];
        auto name_task = selected.data(Qt::DisplayRole).toString();
        auto id_task = selected.data(Qt::UserRole).toInt();
        auto id_list = this->ui->listView->selectionModel()->selectedIndexes()[0].data(Qt::UserRole).toInt();
        this->viewTaskWidget->set_ids(id_task, id_list);
        this->ui->add_new_task_button->setVisible(false);
        this->addTaskWidget->setVisible(false);
        this->viewTaskWidget->setVisible(true);
///asta acolo sa fac ceva public s-o setez idk..
    });
    this->ui->add_new_task_button->setIcon(QIcon(":/Icons/plus.png"));
    QObject::connect(this->ui->add_new_task_button, &QPushButton::clicked, [&](){
        this->ui->add_new_task_button->setVisible(false);
        this->viewTaskWidget->setVisible(false);
        this->addTaskWidget->setVisible(true);
        this->addTaskWidget->refresh_form();
        this->notify_all(TASK_DUE, TASK_DUE, {});
    });
}


void MainWidget::update_lists() {
    this->myFirstModel->change();
    this->update_buttons_list();
    this->mySecondModel->change();
}

void MainWidget::check_tasks_due() {
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
        this->serviceApp.check_task_due();
        timer->stop();
        timer->deleteLater();
    });
    auto crt_date_time = QDateTime::currentDateTime();
    auto time_crt = crt_date_time.time();
    time_crt.setHMS(time_crt.hour(), time_crt.minute()+1, 0, 0);

    auto time = crt_date_time.time().msecsTo(time_crt);
    timer->start(time);
}

void MainWidget::run_app() {
    this->check_tasks_due();
    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, [&](){
        this->close_edit_buttons();
//        qDebug()<<"nu?";
        timer2->stop();
        timer2->deleteLater();
    });
    timer2->start(35);//bun
}


void MainWidget::update(const std::string &option, const std::string &option2, const Task &task) {
    if(option == V_CLOSED){
        this->ui->listView_2->clearSelection();
        this->viewTaskWidget->setVisible(false);
        this->ui->add_new_task_button->setVisible(true);
        return;
    }
    if(option == TASK_DUE){
        auto id_task = std::stoi(option2);
        auto taskk = this->serviceApp.get_task_from_id(id_task);
        this->notify_all(TASK_DUE, TASK_DUE, taskk);
        return;
    }
    this->update_lists();
//    this->create_list();
}

void MainWidget::create_list() {
    auto lista_liste = this->serviceApp.get_all_lists();
    for(const auto&it:lista_butoane){
        it->deleteLater();
    }
    this->lista_butoane.clear();
    for(const auto&it:lista_liste){
        qDebug()<<"Idk";
        auto listObject = new ListObject;
//        buttonGroup->addButton(listObject->get_buton());
        ui->verticalLayout_3->addWidget(listObject);
    }
}

void MainWidget::update_buttons_list() {
    if(lista_butoane.size() != this->myFirstModel->rowCount()){
        auto i = 0;
        while(lista_butoane.size()  > this->myFirstModel->rowCount()){
            lista_butoane.pop_back();
            ++i;
        }
        i = 0;
        while(lista_butoane.size() < this->myFirstModel->rowCount()){
            auto listObj = new ListObject();
            this->lista_butoane.push_back(listObj);
            ++i;
        }
    }
    ///now there should be the same number
//    this->buttonGroup->deleteLater();
//    buttonGroup = new QButtonGroup;
    for(auto i = 0; i < this->myFirstModel->rowCount(); ++i){
        auto ansamble = lista_butoane[i];
        auto id_list = this->myFirstModel->data(myFirstModel->index(i), Qt::UserRole).toInt();
        auto name_list = this->myFirstModel->data(myFirstModel->index(i)).toString().toStdString();
        this->ui->listView->setIndexWidget(myFirstModel->index(i), ansamble);
        this->ui->listView->indexWidget(myFirstModel->index(i))->setVisible(false);
        ansamble->set_id(id_list, name_list);
        ansamble->get_del_btn()->disconnect();
        ansamble->get_rename_btn()->disconnect();
        QObject::connect(ansamble->get_del_btn(), &QPushButton::clicked, [&,ansamble](){
            //window ceva idk
//            QMesag
            auto fer = new QDialog();
            ///tre sa fac fereastra pentru asta neaprat
            fer->setModal(true);
            fer->show();
//            this->serviceApp.delete_list(ansamble->get_id());
            //cred ca crapa idk
        });
        QObject::connect(ansamble->get_rename_btn(), &QPushButton::clicked, [&, ansamble](){
            //window ceva idk
            ansamble->get_line_edit()->setVisible(true);
            ansamble->setAutoFillBackground(true);
            //cred ca crapa idk
        });
        QObject::connect(ansamble->get_line_edit(), &QLineEdit::editingFinished, [&, ansamble](){
            //window ceva idk
            ansamble->get_line_edit()->setVisible(false);
            ansamble->setAutoFillBackground(false);
            auto new_name = ansamble->get_line_edit()->text().toStdString();
            qDebug()<<new_name;
            this->serviceApp.modifiy_list(ansamble->get_id(), new_name);
            //cred ca crapa idk
        });
    }
}

void MainWidget::close_edit_buttons() {
    for(const auto& it : lista_butoane){
        it->setVisible(false);
    }
}



