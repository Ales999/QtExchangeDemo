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
#include "exchangemessages.h"
#include <QtCharts>
#include "config.h"

const int minWidth = 800;
const int minHeight = 600;
const int graph_time_window_secs = 60;

ExchangeUI::ExchangeUI(QSharedPointer<ExchangeClient> _client)
    : QMainWindow(nullptr), client(_client)
{
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Exchange Ethereum - Euro");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setUpdatesEnabled(true);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    axisX->setTickCount(5);
    axisX->setRange(0, 0);
    axisY->setRange(0, 0);

    connect(client.get(), &ExchangeClient::data_fetched, this, [series, axisX, axisY](double val){
        // handle chart updates
        time_t xval = time(nullptr);
        series->append(xval, val);

        //update vertical size of graph
        static double min = val;
        static double max = val;
        min = std::min(val,min);
        max = std::max(val,max);
        if(max > axisY->max())
            axisY->setMax(max+10);
        if(min > axisY->min())
            axisY->setMin(min-10);

        // initialize time range
        static QDateTime start = QDateTime::currentDateTime();
        static QDateTime end = QDateTime::currentDateTime().addSecs(graph_time_window_secs);
        if(axisX->max() < end.toTime_t() )
        {
            axisX->setRange(start.toTime_t(), end.toTime_t());
        }

        // update time range
        if(xval > end.toTime_t())
        {
            start = start.addSecs(graph_time_window_secs/2);
            end = end.addSecs(graph_time_window_secs/2);
            axisX->setRange(start.toTime_t(), end.toTime_t());
        }

    });


    // Setup of UI objects
    QVBoxLayout *layout = new QVBoxLayout;

    // refresh setting widget
    QHBoxLayout *layout_refresh = new QHBoxLayout;
    QLabel *label_refresh = new QLabel("refresh time ms.");
    QLineEdit *edit_refresh = new QLineEdit(QString::number(default_refresh_interval));
    layout_refresh->addWidget(label_refresh);
    layout_refresh->addWidget(edit_refresh);

    // url setting widget
    QHBoxLayout *layout_url = new QHBoxLayout;
    QLabel *label_url = new QLabel("exchange provider");
    QLineEdit *edit_url = new QLineEdit(default_provider_url);
    layout_url->addWidget(label_url);
    layout_url->addWidget(edit_url);

    // field setting widget
    QHBoxLayout *layout_field = new QHBoxLayout;
    QLabel *label_field = new QLabel("exchange field");
    QLineEdit *edit_field = new QLineEdit(default_provider_field);
    layout_field->addWidget(label_field);
    layout_field->addWidget(edit_field);

    // setup layout
    layout->addWidget(chartView);
    layout->addItem(layout_refresh);
    layout->addItem(layout_url);
    layout->addItem(layout_field);

    // button update parameters
    QPushButton *update_params = new QPushButton("Update Parameters");
    connect(update_params, &QPushButton::clicked, this, [this, edit_url, edit_field, edit_refresh](){
        client->setProviderUrl(edit_url->text());
        client->setProviderField(edit_field->text());
        client->setRefreshInterval(edit_refresh->text().toInt());
        client->setParameter();
    });
    layout->addWidget(update_params);

    // button activate fetch of exchange data
    QPushButton *activate_fetch = new QPushButton("Activate Fetch");
    static bool fetch_is_active = false;
    connect(activate_fetch, &QPushButton::clicked, this, [this, update_params]()
    {
        fetch_is_active = !fetch_is_active;
        update_params->setEnabled(!fetch_is_active);
        client->setActivateStatus(fetch_is_active);
    });

    layout->addWidget(activate_fetch);
    layout->addWidget(update_params);

    //setCentralWidget(layout);
    QWidget *window = new QWidget;
    window->setLayout(layout);
    setCentralWidget(window);
    setMinimumWidth(minWidth);
    setMinimumHeight(minHeight);
}

ExchangeUI::~ExchangeUI()
{

}
