# -*- coding: utf-8 -*-
import time
import datetime
from cryptography.fernet import Fernet
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

ferne = Fernet(<key>.encode())

#Almacenar datos dentro de la cadena de bloques
def GuardarDato (From, Key, Red, nodo, sensor, unidades, medida):
    From = Web3.toChecksumAddress(From)
    web3.personal.unlockAccount(From, Key, 5);
    
    unidades = unidades.encode()
    unidades = str(ferne.encrypt(unidades))
    unidades = unidades[2:-1]
    
    sensor = sensor.encode()
    sensor = str(ferne.encrypt(sensor))
    sensor = sensor[2:-1]
    
    medida = medida.encode()
    medida = str(ferne.encrypt(medida))
    medida = medida[2:-1]
    
    res = contrato.transact({'from': From, 'gas': 4700000}).SaveData(Red, nodo, unidades, sensor, medida)
    return (res)


