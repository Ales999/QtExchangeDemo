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

#ifndef EXCHANGEUI_H
#define EXCHANGEUI_H

#include <QMainWindow>
#include "exchangeclient.h"

class ExchangeUI : public QMainWindow
{
    Q_OBJECT

public:
    ExchangeUI(QSharedPointer<ExchangeClient> _client);
    ~ExchangeUI();

    QSharedPointer<ExchangeClient> client;
};

#endif // EXCHANGEUI_H
