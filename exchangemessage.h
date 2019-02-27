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

#ifndef EXCHANGEMESSAGE_H
#define EXCHANGEMESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <memory>

extern const int default_refresh_interval;
extern const QString default_provider_url;
extern const QString default_provider_field;

// interface for ExchangeMessage Type
struct ExchangeMessage
{
    enum class MessageType
    {
        Parameters,
        SetActiveStatus,
        Data
    };

    virtual ~ExchangeMessage();
    virtual QJsonDocument toJsonDoc() = 0;
    virtual MessageType type() = 0;

    // factory method
    static std::unique_ptr<ExchangeMessage> createFromJson(QByteArray &&json_data);
};

struct ExchangeMessageParameters: public ExchangeMessage
{
    ExchangeMessageParameters(int _refresh_interval, QString _provider_url, QString _provider_field);
    MessageType type(){return MessageType::Parameters;}
    QJsonDocument toJsonDoc();

    int refresh_interval;
    QString provider_url;
    QString provider_field;
};

struct ExchangeMessageStatus: public ExchangeMessage
{
    ExchangeMessageStatus(bool _is_active);
    MessageType type(){return MessageType::SetActiveStatus;}
    QJsonDocument toJsonDoc();

    bool is_active;
};

struct ExchangeMessageData: public ExchangeMessage
{
    ExchangeMessageData(double _data);
    MessageType type(){return MessageType::SetActiveStatus;}
    QJsonDocument toJsonDoc();

    double data;
};

#endif // EXCHANGEMESSAGE_H
