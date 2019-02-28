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

#include "exchangeui.h"
#include <QApplication>
#include "exchangedispatcher.h"
#include "exchangeserver.h"
#include "exchangeclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Main ThreadId:" << QThread::currentThreadId();

    ExchangeServer eserver;
    QSharedPointer<ExchangeClient> client;
    QScopedPointer<ExchangeUI> ui;

    // wait for server initialization by ExchangeServer::ready signal
    QObject::connect(&eserver, &ExchangeServer::ready, [&ui, client]() mutable {
        // start exchange client
        client.reset(new ExchangeClient());
        client->connectToExchangeServer();

        // start UI
        ui.reset(new ExchangeUI(client));
        ui->show();
    });

    // start server thread
    eserver.start();

    // start main loop
    return a.exec();
}
