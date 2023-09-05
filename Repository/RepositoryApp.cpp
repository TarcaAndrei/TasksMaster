//
// Created by Andrei on 02.09.2023.
//

#include <QJsonArray>
#include "RepositoryApp.h"

void RepositoryApp::set_token(const string &token) {
    this->access_token = token;
    this->reload_data();
}


void RepositoryApp::add_Task(const string &basicString, Task task) {
}

void RepositoryApp::reload_data() {
    this->list_names.clear();
    this->all_data_list.clear();
    this->reload_lists();

}

RepositoryApp::RepositoryApp() {
    this->accessManager = new QNetworkAccessManager;
}

void RepositoryApp::reload_lists() {
    std::string url = getenv("host_name");
    url += ":";
    url += getenv("port");
    url += "/api/";
    url += "lists/";
    QUrl qUrl = QUrl(QString::fromStdString(url));
    QNetworkRequest request(qUrl);
    request.setRawHeader("Content-Type", "application/json");
    QByteArray auth_token;
    auth_token.append("Token ");
    auth_token.append(QString::fromStdString(this->access_token).toUtf8());
    request.setRawHeader("Authorization", auth_token);
    this->reply_lists = accessManager->get(request);
//    qDebug()<<this->replylists;

//    QByteArray postData;
//    string string_to_post = R"({"listName": ")";
//    string_to_post += "ThirdList";
//    string_to_post += R"("})";        //=>> create-ul la o lista
//    postData.append(string_to_post);
//    this->replylists = accessManager->post(request, postData);

    QObject::connect(reply_lists, &QNetworkReply::finished, [&](){
//        qDebug()<<this->replylists;
        if(reply_lists->error() == QNetworkReply::NoError){
            auto responseData = reply_lists->readAll();
            auto jsonDoc = QJsonDocument::fromJson(responseData);
            if(not jsonDoc.isNull()){
                auto jsonArray = jsonDoc.array();
                for(auto it : jsonArray){
                    auto jsonObj = it.toObject();
                    auto id_l = jsonObj.value("id").toInt();
                    auto name_l = jsonObj.value("listName").toString().toStdString();
                    this->list_names.push_back(std::make_pair(id_l, name_l));
                }
            }
            else{
                qDebug()<<"Document null";
            }
        }
        else{
            qDebug()<<"Eroare la GET REQUEST la Lists";
        }
        this->reload_tasks();
        this->reply_lists->deleteLater();
    });
}

void RepositoryApp::reload_tasks() {
    std::string url = getenv("host_name");
    url += ":";
    url += getenv("port");
    url += "/api/";
    url += "tasks/";
    QUrl qUrl = QUrl(QString::fromStdString(url));
    QNetworkRequest request(qUrl);
    request.setRawHeader("Content-Type", "application/json");
    QByteArray auth_token;
    auth_token.append("Token ");
    auth_token.append(QString::fromStdString(this->access_token).toUtf8());
    request.setRawHeader("Authorization", auth_token);
    this->reply_tasks = accessManager->get(request);
//    qDebug()<<this->reply;
    QObject::connect(reply_tasks, &QNetworkReply::finished, [&](){
//        qDebug()<<this->reply;
        if(reply_tasks->error() == QNetworkReply::NoError){
            auto responseData = reply_tasks->readAll();
            auto jsonDoc = QJsonDocument::fromJson(responseData);
            if(not jsonDoc.isNull()){
                auto jsonArray = jsonDoc.array();
                for(auto it : jsonArray){
                    auto jsonObj = it.toObject();
                    qDebug()<<jsonObj;
                    auto id_t = jsonObj.value("id").toInt();
                    auto name_t = jsonObj.value("taskName").toString().toStdString();
                    auto details_t = jsonObj.value("taskDetails").toString().toStdString();
                    auto time_due_t = jsonObj.value("taskDue").toString();
                    auto done_t = jsonObj.value("taskDone").toBool();
                    auto priority_t = jsonObj.value("taskPriority").toString().toStdString();
                    auto task_list_id = jsonObj.value("taskList").toInt();
                    auto time_date_due = QDateTime::fromString(time_due_t, Qt::ISODate);
                    auto task_nou = Task(id_t, name_t, details_t, time_date_due, priority_t, done_t);
                    this->all_data_list[task_list_id].push_back(task_nou);
                }
            }
        }
        else{
            qDebug()<<"Eroare la GET REQUEST la Lists";
        }
        this->notify_all(LOAD_F);
        this->reply_tasks->deleteLater();
    });
}

vector<Task> RepositoryApp::get_tasks_from_list(int id_lista) {
    return this->all_data_list[id_lista];
}

std::vector<std::pair<int, string>> RepositoryApp::get_all_lists() {
    return this->list_names;
}










