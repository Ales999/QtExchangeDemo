/*
 * Copyright (C) 2019 Alessandro Arrabito
 */

/*
 * This file is part of QtExchangeDemo.
 *
 * QtExchangeDemo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtExchangeDemo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtExchangeDemo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "exchangedispatcher.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QErrorMessage>
#include <QTimer>
#include "exchangemessage.h"
#include "config.h"

static int current_refresh_interval;
static QString provider_url;
static QString provider_field;

DataFetcher::DataFetcher()
{
    connect(&qnam, &QNetworkAccessManager::finished, this, &DataFetcher::response);
}

void DataFetcher::onTimeout()
{
    QUrl url(provider_url);
    QNetworkRequest req(url);
    qnam.get(req);
}

void DataFetcher::response(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->error();
        return;
    }

    QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
    emit data_fetched(json_doc[provider_field].toString().toDouble());
}

ExchangeDispatcher::ExchangeDispatcher():socket(nullptr)
{
    current_refresh_interval = default_refresh_interval;
    provider_url = default_provider_url;
    provider_field = default_provider_field;
}

void ExchangeDispatcher::run()
{
    // setup of timer to fetch data
    timer.reset(new QTimer());
    DataFetcher data_fetcher;
    connect(timer.get(), SIGNAL(timeout()), &data_fetcher, SLOT(onTimeout()));

    // Connect by direct connection to be sure that slot will be executed in this thread
    connect(&data_fetcher ,
            &DataFetcher::data_fetched,
            this,
            [this](double val){
                ExchangeMessageData msg(val);
                socket->write(msg.toJsonDoc().toJson());
            },
            Qt::DirectConnection);

    connect(this,SIGNAL(runTimer(int)),timer.get(), SLOT(start(int)));
    connect(this,SIGNAL(stopTimer()),timer.get(), SLOT(stop()));

    qDebug() << "ExchangeDispatcher ThreadId:" << QThread::currentThreadId();

    QTcpServer server;
    server.setParent(nullptr);
    server.moveToThread(this);
    connect(&server,
            &QTcpServer::newConnection,
            this,
            [&server, this]()
            {
                socket = server.nextPendingConnection();
                connect(socket, &QTcpSocket::readyRead, this, &ExchangeDispatcher::handleReadyRead);
            },
            Qt::DirectConnection);

    server.setMaxPendingConnections(1);
    if(!server.listen(QHostAddress::Any, 9999))
    {
        throw std::runtime_error("Server could not start");
    }

    emit server_is_listening();
    qDebug() << "Server started";

    exec();
    timer.release();
    qDebug() << "end thread";
}

void ExchangeDispatcher::handleReadyRead()
{
    std::unique_ptr<ExchangeMessage> gmsg_ptr = ExchangeMessage::createFromJson(socket->readAll());
    qDebug() << "handleReadyRead type:" << static_cast<int>(gmsg_ptr->type());
    switch(gmsg_ptr->type())
    {
    case ExchangeMessage::MessageType::Parameters:
        {
            ExchangeMessageParameters* msg_ptr = static_cast<ExchangeMessageParameters*>(gmsg_ptr.get());
            int new_refresh_interval = msg_ptr->refresh_interval;
            if(new_refresh_interval != current_refresh_interval)
                current_refresh_interval = new_refresh_interval;
            provider_field = msg_ptr->provider_field;
            provider_url = msg_ptr->provider_url;
            break;
        }

    case ExchangeMessage::MessageType::SetActiveStatus:
        {
            ExchangeMessageStatus* msg_ptr = static_cast<ExchangeMessageStatus*>(gmsg_ptr.get());
            qDebug() << "SetActiveStatus:" << msg_ptr->is_active;
            if(msg_ptr->is_active && !timer->isActive())
                emit runTimer(current_refresh_interval);
            else if(!msg_ptr->is_active && timer->isActive())
                emit stopTimer();
            break;
        }
    default:
        break;
    }
}
