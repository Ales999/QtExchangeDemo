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

#ifndef EXCHANGECLIENT_H
#define EXCHANGECLIENT_H

#include <QObject>
#include <QTcpSocket>

// ExchangeClient handle communication with the exchange server,
// by messages sent and received through a tcp socket.
// This allows future implementation as for example move server outside in a standalone process.
class ExchangeClient: public QObject
{
    Q_OBJECT
public:
    ExchangeClient();
    void connectToExchangeServer();
    int refreshInterval();
    QString providerUrl();
    QString providerField();
    void setProviderUrl(const QString&);
    void setProviderField(const QString&);
    void setRefreshInterval(const int);

    void setParameter();
    void setActivateStatus(bool active);
signals:
    void data_fetched(double);
private:
    int refresh_interval;
    QString provider_url;
    QString provider_field;
    QTcpSocket conn;
};

#endif // EXCHANGECLIENT_H
