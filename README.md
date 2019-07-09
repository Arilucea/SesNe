# SesNe
SesNe is a system that allows the creation of wireless sensor networks at a very low cost, together with their supervision using an application for smartphones, and the storage of generated data in blockchain, which guarantees their immutability and allows trading with them.
<kbd>
![Schematic](Documentation%20(Spanish)/Schematic.jpg)
</kbd>
<p align="center">Operating diagram</p>

## Introduction
Firstly, the general operation of the system will be explained, from the connection to the network to the purchase/sale of the data generated, and later on, in each of the corresponding sections, the individual operation will be detailed.

Once the size of the network had been defined and the necessary sensors had been selected, the microcontrollers would be programmed to read the data and send the information between the nodes and the application.

When the network is started, from the application a connection is established with the initial node of the network (node 0). When it is established, the names of the nodes that form it are received and the application menu is updated. When selecting a node, a call to node 0 is executed requesting the reading of the data of the indicated node, this call spreads through the whole network until finding the selected one that will respond with the information of its sensors.

Sensor values will be displayed in the application and constantly updated. At the same time when values are received they are sent to an API (Application Programming Interface) where they are encrypted and stored in Blockchain ready for sale or verification.

On the other hand, using an interface developed in Python allows the purchase of stored data. In the front you select the network whose data you want to buy and indicate the address that will be used for payment. Once the purchase process has started, the data will be extracted from the network,decrypt and store in a CSV file.



## Network Design
In order to be able to connect the different elements that make up the system in a simple way, an own communication protocol has been developed, thanks to which all the parameters exchanged between the different elements can be controlled, as well as the format and the time of updating the information in the data sampling.

The first condition that was taken into account when designing the network was the selection of the device used for the transmission and reception of information. After considering various options involving the use of commercial systems such as XBee, Sigfox or LoRa, its high cost made me choose to create my own protocol using a low-cost module, such as nrf24l01.

The main reason for the selection of this module was precisely its cost which is much lower than commercial systems, its price is between 0.5 and 4 euros. The second reason for their choice was the availability of the module in two versions with different transmission power. There is a short range version, about 20 meters, and another long range version, with a theoretical effective distance of 1100 meters (in real conditions, and with a low error rate, about 250 meters). The existence of this second version was one of the factors that determined the selected network topology, as explained below.

As a network topology, it was first considered the possibility of creating a centralized network, in which all secondary nodes were connected to a single central node. The problem presented by this topology, for which it was discarded, is that the need to connect all the nodes to the central limits the maximum distance at which distributed nodes can be located, thus preventing the deployment of a wide network capable of covering great distances.


<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/sensornetwork.png">
</p>

Taking into account this last requirement, it was decided to design a multi-hop network, in which each node is connected to the previous node and to the next one (if any). The main disadvantage of this configuration is that it forces information to travel through several intermediate nodes until it reaches the main node. On the contrary, it has the advantage of allowing a greater distance between the main node and other nodes of the network, being able to deploy larger networks.


## App
The application has been developed to be dynamic and versatile, with no predefined number of nodes or number of sensors per node, so that a single activity is used for all nodes. In this way, both the content of the activity and the number of nodes to be displayed are only conditioned by the information received from the main node.

When you start the application, the first thing that appears is the main activity. Inside, a list of known and found Bluetooth devices is displayed. Once the desired device has been selected, the connection is established.

In the side menu, destined to centralize the access to the different nodes of the network, the nodes of the network are shown from the data relative to the structure of the network received from the main node, it is proceeded to update its name in the side menu, as well as to add the rest of the nodes. From then on, you can select the node you want.

When the desired node is pressed, a table is shown at the top with the information received from the node's sensors

The information received is represented in a table showing in each row the parameters specific to each sensor: its name, the measured value and the measurement units.

<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/app.jpg">
</p>

As the data is displayed, it is checked whether blockchain data storage is enabled; if so, a Python API call is made in which the data is encrypted and stored on the network.


## Blockchain
Blockchain is a technology that has been talked about a lot lately, although most of the time only by means of ideas, possible functionalities, or about its application in cryptocurrencies; in rare occasions real and viable facts and applications of the technology are shown. What this project aims to show is one of the possible applications that this technology has in the world of electronics.

With the use of this technology we have tried to show two possible applications that can be useful. On the one hand, a permanent and immutable storage of the information has been created, in such a way that a historical record of the values generated by the sensors is obtained, which can serve, if necessary, as a quality certificate. On the other hand it has been tried to give an added value to these stored data; instead of being simply information available to the owner of the system, a mechanism has been created by which anyone interested in the data will be able to acquire them by paying a small amount of a token already existing on the network. In this way, anyone who wants to conduct a study, develop an optimization algorithm, or other applications for which data is needed, will have a way to get the information they need, while the owner of the data can get an economic return from them.

Quorum, a network developed by JP Morgan Bank, based on Ethereum but with a consensus protocol distinct from standard Ethereum and without the existence of a cryptomoneda, has been used as the blockchain network in this project, so the transactions are of zero cost.

In order to be able to establish the connection with the network, both for the publication of the data and for its purchase, a series of scripts were developed in Python that offer all the functionality that the system needs.

In order to be able to publish the data within the network from the mobile application, it must be sent to an APIREST that encrypts and publishes it.

The reason for using a key to encrypt the securities received stems from one of the fundamental characteristics of blockchain: the need for transactions to be public, so that all the information is visible and that what is happening on the network can be known at all times. However, when it comes to sending data, they cannot be published directly over the network because anyone consulting the transaction history of the block could know the information sent and thus obtain the data without having to pay. To avoid this, once the data has been received in ApiRest, and before being published in Quorum, it is encrypted using the Fernet system. This is a system that uses a private key to encrypt the data, so that, if it is not available, it is not possible to decipher it and only a set of meaningless characters is displayed. In this way, the necessary privacy of information is made compatible with the public and auditable character inherent in blockchain.

For example, if the API receives a value of 20.5 (let's assume it's a temperature sensor measurement), the result after encryption could be something like this:

<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/encripteddata.png">
</p>

As you can see, it is not possible to know the information sent, once encrypted, if the private key is not available.
In order to be able to extract data from the network in a comfortable and simple way, the front shown in figure 6 has been developed, using Python's "tkinter" library.

<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/front.png">
</p>

First, the specified information is obtained through the different fields of the application. The number of data to be extracted is obtained from the name of the selected network. Next, the event filter is created that will allow capturing the events generated by the data purchase function and the data capture starts. During the development of this system a problem arose: it was necessary to be able to capture the events, but at the same time execute the function that generates them. In order to solve this problem, we used the "threading" library, which allows us to execute a process in a thread in the background and maintain the execution of the program in the main one. In this way, it is possible to execute a function in the background while the program continues in the main thread. When the data extraction process is finished, a CSV file is generated that presents all the information organized and ready to be processed.

The extraction process involves decrypting the data when captured in the event. By means of different processes, the information contained in the event (figure 7) is transferred to the information to be stored in the CSV

<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/event.png">
</p>

Once this process is finished, a list is obtained containing the extracted data in legible form.

<p align="center">
  <img src="https://github.com/arilucea/SesNe/blob/master/Documentation%20(Spanish)/ImagesReadme/result.png">
</p>
