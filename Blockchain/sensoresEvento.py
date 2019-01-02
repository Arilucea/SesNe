# -*- coding: utf-8 -*-
import time
import datetime
import web3						
from web3 import Web3, HTTPProvider, IPCProvider
import abisensores

#base64.urlsafe_b64encode(os.urandom(32))

web3 = Web3(HTTPProvider(<IP:Port blockchain network>))

abi = abisensores.abi()
'''
'''
scAddress = "0x1226600589283c8de1d684b09a0fb143bb9121a7"
scAddress = Web3.toChecksumAddress(scAddress)

contrato = web3.eth.contract(address=scAddress, abi=abi);

#Almacenar datos dentro de la cadena de bloques
def GuardarDato (From, Clave, Red, nodo, sensor, unidades, medida):
    From = Web3.toChecksumAddress(From)
    web3.personal.unlockAccount(From, Clave, 3600);
    
    res = contrato.transact({'from': From, 'gas': 4700000}).SaveData(Red, nodo, unidades, sensor, str(medida))
    return (res)

#Obtener las redes que hay dentro de la la cadena de bloques
def Redes ():
    NumeroRedes = contrato.call().networksNumber()
    redesLista=[]
    for i in range (0, NumeroRedes):
        red = contrato.call().networkName(i)
        redesLista.append(red)
    return (redesLista)

#Obtener los nodos pertenecen a la red
def Nodos (red):
    NumeroNodos = contrato.call().nodesNumber(red)
    nodosLista = []
    for i in range (0, NumeroNodos):
        nodosLista.append(contrato.call().nodeName(red, i))
    return (nodosLista)

#Sacar todos los datos almacenados en la cadena de bloques
def SacarDato (Red, From, Key):
    if (Red == "0"):
        listaRedes = Redes()
        for red in listaRedes:
            redb = red
            NodosLista = Nodos(redb)
            #resultado.append(red)
            for nodo in NodosLista:
                #resultado.append(nodo)
                nodob = nodo
                MedidasHistorico(red, nodob, From, Key)
                
    elif (Red != "0"):
        NodosLista = Nodos(Red)
        #resultado.append(red)
        for nodo in NodosLista:
            #resultado.append(nodo)
            nodob = nodo
            MedidasHistorico(Red, nodo, From, Key)     

#Obten el numero de datos dentro de un nodo
def NumDato (red, nodo):
    NumeroDatos = contrato.call().datosNumber(red, nodo)
    return NumeroDatos


#Valor n
def MedidaN(red, nodo, n, From, Key):
    From = Web3.toChecksumAddress(From)
    web3.personal.unlockAccount(From, Key, 5);
    res = contrato.transact({'from': From, 'gas': 47000000}).BuyData(red, nodo, n)
    
    
def MedidasHistorico (red, nodo, From, Key):
    n = NumDato(red, nodo)
    n = n+1
    for i in range (0, n):
        MedidaN(str(red), str(nodo),i, From, Key)

#Coste de obtencion de todos los datos
def ContDatos (Red):
    NumDatos = 0
    if (Red == "0"):
        listaRedes = Redes()
        for red in listaRedes:
            NodosLista = Nodos(red)
            for nodo in NodosLista:
                #resultado.append(nodo)
                NumDatos = NumDatos + NumDato(red, nodo)
                
    elif (Red != "0"):
        red = Red
        NodosLista = Nodos(red)
        #resultado.append(red)
        for nodo in NodosLista:
            #resultado.append(nodo)
            NumDatos = NumDatos + NumDato(red, nodo)
    
    return (NumDatos)