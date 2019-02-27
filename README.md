<H2>QtExchangeDemo</H2>

This is a sample application that tests the following Qt topics:
</br>

<b>qt threads</b>

<b>tcp socket, client/server communication</b>

<b>json messages</b>

<b>QtChart</b>

QtExchangeDemo is about the exchange between Euro and Ethereum.

The application starts a thread ( the exchange server ) that fetch json data from an host and get currency change values.

Furthermore the thread expose an interface that allows to communicate with a tcp client.

<H2>Installation</H2>

<pre>cd QtExchangeDemo</pre>

<pre>qmake</pre>

<pre>make</pre>
