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

#include "exchangeclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "exchangemessages.h"

ExchangeClient::ExchangeClient():
    refresh_interval(default_refresh_interval),
    provider_url(default_provider_url),
    provider_field(default_provider_field)
{
}

void ExchangeClient::connectToExchangeServer()
{
    conn.connectToHost("localhost", 9999);
    if( !conn.waitForConnected())
    {
        throw std::runtime_error("connection with the Exchange Server fail");
    }

    connect(&conn, &QTcpSocket::readyRead, this, [this](){
        std::unique_ptr<ExchangeMessage> msg_ptr = ExchangeMessage::createFromJson(conn.readAll());
        //qDebug() << "received data from exchange server:" << msg_ptr->toJsonDoc().toJson();
        double exchange_data = msg_ptr->toJsonDoc()["data"].toDouble();
        emit data_fetched(exchange_data);
    });
}

int ExchangeClient::refreshInterval()
{
    return refresh_interval;
}

QString ExchangeClient::providerUrl()
{
    return provider_url;
}

QString ExchangeClient::providerField()
{
    return provider_field;
}

void ExchangeClient::setProviderUrl(const QString& url)
{
    provider_url = url;
}

void ExchangeClient::setProviderField(const QString& field)
{
    provider_field = field;
}

void ExchangeClient::setRefreshInterval(const int refresh)
{
    refresh_interval = refresh;
}

void ExchangeClient::setParameter()
{
    ExchangeMessageParameters msg(refresh_interval, provider_url, provider_field);
    QJsonDocument doc = msg.toJsonDoc();
    conn.write(doc.toJson());
}

void ExchangeClient::setActivateStatus(bool active)
{
    ExchangeMessageStatus msg(active);
    conn.write(msg.toJsonDoc().toJson());
}
