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

#ifndef EXCHANGEDISPATCHER_H
#define EXCHANGEDISPATCHER_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qthread.h>
#include <QTcpServer>
#include <QTimer>

// DataFetcher fetch exchange data and run in a timer loop
// DataFetcher is a worker activated by ExchangeDispatcher.
class DataFetcher : public QObject
{
    Q_OBJECT
public:
    DataFetcher();
signals:
    void data_fetched(double);
private slots:
    void onTimeout();
    void response(QNetworkReply *reply);
private:
    QNetworkAccessManager qnam;
};

// ExchangeDispatcher handle client messages and communication
// and by DataFetcher fetch exchange data.
class ExchangeDispatcher : public QThread
{
    Q_OBJECT
public:
    ExchangeDispatcher();
signals:
    // use runTimer/stopTimer signal to start/stop timer by ExchangeDispatcher thread also
    // if caller run inside another thread.
    void runTimer(int msec);
    void stopTimer();
    void newConnection();
    void data_fetched(double);
    void server_is_listening();

private slots:
    void handleReadyRead();
protected:
    void run();
private:
    QTcpSocket *socket;
    std::unique_ptr<QTimer> timer;
};

#endif // EXCHANGEDISPATCHER_H


