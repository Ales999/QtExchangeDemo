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

#include "exchangemessage.h"
#include <QDebug>

// factory method
std::unique_ptr<ExchangeMessage> ExchangeMessage::createFromJson(QByteArray &&json_data)
{
    std::unique_ptr<ExchangeMessage> msg_ptr;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_data);
    MessageType type = static_cast<MessageType>(json_doc["type"].toInt());
    switch(type)
    {
    case MessageType::Parameters:
        {
            int refresh_interval = json_doc["refresh"].toInt();
            QString provider_url = json_doc["url"].toString();
            QString provider_field = json_doc["field"].toString();
            msg_ptr.reset(new ExchangeMessageParameters(refresh_interval, provider_url, provider_field));
            break;
        }
    case MessageType::SetActiveStatus:
        {
            bool is_active = json_doc["is_active"].toBool();
            msg_ptr.reset(new ExchangeMessageStatus(is_active));
            break;
        }
    case MessageType::Data:
        {
            double data = json_doc["data"].toDouble();
            msg_ptr.reset(new ExchangeMessageData(data));
            break;
        }
    default:
        qWarning() << __FUNCTION__ << " bad message type";
    }

    return msg_ptr;
}

ExchangeMessage::~ExchangeMessage(){}

ExchangeMessageParameters::ExchangeMessageParameters(int _refresh_interval, QString _provider_url, QString _provider_field):
    refresh_interval(_refresh_interval),
    provider_url(_provider_url),
    provider_field(_provider_field)
{}

QJsonDocument ExchangeMessageParameters::toJsonDoc()
{
    QJsonDocument json_doc;
    json_doc.setObject(QJsonObject({{"type", QString::number(static_cast<int>(MessageType::Parameters))},
                                    {"refresh", refresh_interval},
                                    {"url", provider_url},
                                    {"field", provider_field}}));
    return json_doc;
}

ExchangeMessageStatus::ExchangeMessageStatus(bool _is_active):
    is_active(_is_active)
{}


QJsonDocument ExchangeMessageStatus::toJsonDoc()
{
    QJsonDocument json_doc;
    json_doc.setObject(QJsonObject({{"type", QJsonValue(static_cast<int>(MessageType::SetActiveStatus))},
                                    {"is_active", QJsonValue(is_active)}}));
    return json_doc;
}

ExchangeMessageData::ExchangeMessageData(double _data):data(_data)
{}

QJsonDocument ExchangeMessageData::toJsonDoc()
{
    QJsonDocument json_doc;
    json_doc.setObject(QJsonObject({{"type", QJsonValue(static_cast<int>(MessageType::Data))},
                                    {"data", QJsonValue(data)}}));
    return json_doc;
}

