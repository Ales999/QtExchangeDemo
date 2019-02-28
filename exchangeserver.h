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

#ifndef EXCHANGESERVER_H
#define EXCHANGESERVER_H

#include "exchangedispatcher.h"

// RAII object that handle correct destruction of exchange dispatcher
// when ExchangeServer exit from scope.
class ExchangeServer : public QObject
{
    Q_OBJECT
public:
    explicit ExchangeServer(QObject *parent = nullptr);
    ~ExchangeServer();
    void start();

signals:
    void ready();

private:
    ExchangeDispatcher exchangeDispatcher;
};

#endif // EXCHANGESERVER_H
