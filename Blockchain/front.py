# -*- coding: utf-8 -*-
from tkinter import *
from tkinter import ttk
import time
import requests
from threading import Thread
import csv
import datetime
import codecs
import web3						
from web3 import Web3, HTTPProvider, IPCProvider

import abisensores
import sensoresEvento

web3 = Web3(HTTPProvider(<IP:Port blockchain network>))

abi = abisensores.abi()

scAddress = <contract address>
scAddress = Web3.toChecksumAddress(scAddress)

cont = 0
def SavaDataCsv(name, datos):
    name = name + ".csv"
    with open(name, "w") as output:
        writer = csv.writer(output, lineterminator='\n')
        for i in datos:
            writer.writerow([i])    

def stripData(data, cont):
    resultado =[]
    data = data[335:]
    data = [pair for pair in data.split('0000000000')  if pair!='']
    #print(data)
    for i in range (0, 6):
        datos = data[i]
        datos = datos.lstrip("0")
        #print(datos)
        if i == 0: #Si es el timestamp
            valor = int(datos, 16) #Se convierte a decimal y se elmininan los milisegundos
            valor = datetime.datetime.fromtimestamp(valor/1000000000+time.altzone).strftime('%Y-%m-%d %H:%M:%S')
            resultado.append(valor)
        elif i > 0:
            if (len(datos) >= 32):  #Elimina los carateres que indican la longitud
                datos = datos[2:]   #Elimina 2
            else:
                datos = datos[1:]   #Elimina 1
            if (len(datos) % 2 != 0):   #Si hay un numero impar de caracteres añade un 0 al final
                datos = datos + "0"
            valor = str(codecs.decode(datos, "hex"))
            valor = valor[2:-1]
            if (len(valor) > 25):
                url = "<Api IP:Port>/decode?value=" + valor
                valor = requests.get(url)
                valor = valor.text
                valor = valor[2:-1]
            resultado.append(valor)
    return(resultado)

def GetData(event, cont):
    data = event["data"]
    dato = stripData(data, cont)
    return (dato)

def log_loop(event_filter, poll_interval, cont, last, red):
    valores = []
    while cont < last:
        for event in event_filter.get_new_entries():
            nuevoDato = GetData(event, cont)
            cont = cont + 1
            valores.append(nuevoDato)
            if (cont == last):
                 if (red == "0"):
                     red = "All" 
                 SavaDataCsv(red, valores)

class Aplicacion():
    def __init__(self):
        
        self.raiz = Tk()
        #Se configura la ventana, dimensiones, icono y titulo
        self.raiz.geometry('500x220')
        self.raiz.resizable(width=False,height=False)
        self.raiz.iconbitmap('C:\\Users\\Javier\\Desktop\\TFG_Block\\icon.ico')        
        self.raiz.title('Compra de datos')
        
        #Elementos label y textbox de la direccion
        self.LAddress = Label(self.raiz, text="Dirección", font=("Helvetica", 15))
        self.LAddress.pack(side=TOP)
        self.TextAddress = Text(self.raiz, width=45, height=1,font=("Helvetica", 15))
        self.TextAddress.pack(side=TOP)
        
        #Elementos de la clave privada
        self.LClave = Label(self.raiz, text="Clave", font=("Helvetica", 15))
        self.LClave.pack(side=TOP)
        self.TextKey = Text(self.raiz, width=45, height=2,font=("Helvetica", 15))
        self.TextKey.pack(side=TOP)
        
        #Informacion
        self.LMessage = Label(self.raiz, text="Los datos se almacenarán en un csv con el nombre de la red", font=("Helvetica", 9))
        self.LMessage.pack(side=BOTTOM)
        
        #Balance cuenta       
        self.BotonBalance = ttk.Button(self.raiz, text='Balance', command=self.Balance)               
        self.BotonBalance.pack(side=TOP, anchor=W, expand=NO)
        
        #Boton comprar datos        
        self.BotonBuy = ttk.Button(self.raiz, text='Comprar Datos', command=self.buyData)               
        self.BotonBuy.pack(side=LEFT)
        
        #Combobox que contiene las redes
        self.combobox = ttk.Combobox(self.raiz, state="readonly")
        self.combobox.set("Selecciona la red")  
        self.combobox.bind("<<ComboboxSelected>>", self.selection_changed)
        #Llama a la funcion que devuelve la lista de redes y añadelas al combobox
        res = sensoresEvento.Redes()
        res.append("All")
        self.combobox["values"] = res                         
        self.combobox.pack(side=RIGHT)
        
        self.BotonBuy.focus_set()        
        self.raiz.mainloop()
    
    #Funcion del boton BotonBuy
    def buyData(self):
        sel = self.combobox.get()
        
        From = self.TextAddress.get("1.0",END)
        From = From[:-1]

        Key = self.TextKey.get("1.0",END)
        Key = Key[:-1]
        
        if (sel == "All"):
            sel = "0"
        totaldatos = sensoresEvento.ContDatos(sel)
        block_filter = web3.eth.filter({"Deposit": scAddress})
        worker = Thread(target=log_loop, args=(block_filter, 0, cont, totaldatos, sel))
        worker.start()
        sensoresEvento.SacarDato(sel, From, Key)
        self.LMessage['text'] = 'Process Complete'
    
    #Funcion del boton BotonBuy
    def Balance(self):
        From = self.TextAddress.get("1.0",END)
        From = From[:-1]
        url = "<Api IP:Port>/token/balance?address=" + From
        valor = requests.get(url)
        valor = valor.text
        if (len(valor) < 30):
            self.LMessage['text'] = "La cuenta introducida contiene: " + valor + " Arken"
        else:
            self.LMessage['text'] = "La cuenta introducida no es correcta"
        
    
    #Cambio en el combobox
    def selection_changed(self, event):
        sel = self.combobox.get()
        if (sel == "All"):
            sel = "0"
        cost = sensoresEvento.ContDatos(sel)*10
        self.LMessage['text'] = "El coste total sera de: " + str(cost) + " Arken"


def main():
    mi_app = Aplicacion()
    return 0

if __name__ == '__main__':
    main()