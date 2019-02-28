<H2>QtExchangeDemo</H2>

This is just a test to some of the following Qt topics as:
</br>

<ul>
  <li>QtThread</li>
  <li>QTcpServer, QTcpSocket and how to hanlde client server communication by json messages</li>
  <li>QJsonDocument</li>
  <li>QtChart</li>
</ul> 

QtExchangeDemo fetch data of the currency exchange between Euro and Ethereum, future improvements should allow more customization and fetch more data, but at the moment is not the scope of the application.

The QtExchangeDemo run starting a thread ( the exchange server ) that fetch json data from an host and get currency change values.

Furthermore the thread server expose an interface (json messages) that allows to handle server configuration and data by a tcp client.

The client class is the backend for the UI that represent fetched data on a Qt Chart widget.

The exchange server run inside the same process as a separated thread but data exchanges take place by messages sent by a tcp socket therefore a separation of server and client would be the best option... maybe future implementations.   

<b>Installation</b>

<code>

cd QtExchangeDemo
  
qmake

make

</code>

<b>Run</b>

<code>just launch ./QtExchangeDemo</code>
