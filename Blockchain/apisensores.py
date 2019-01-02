# -*- coding: utf-8 -*-
from flask import Flask, request			
from web3 import Web3
import GuardarDato

app = Flask(__name__)

@app.route('/sensores/guardar', methods=['GET'])
def Guardar():
    red = request.args.get('red')
    nodo = request.args.get('nodo')
    sensor = request.args.get('sensornombre')
    unidades = request.args.get('unidades')
    medida = request.args.get('medida')    
    address = request.args.get('address')
    clave = request.args.get('clave')
    res = GuardarDato.GuardarDato(str(address), str(clave), str(red), str(nodo), str(sensor), str(unidades), medida)
    return (res)

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0',port=15000)