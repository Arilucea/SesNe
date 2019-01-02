# -*- coding: utf-8 -*-
from cryptography.fernet import Fernet
from flask import Flask, request	

app = Flask(__name__)

ferne = Fernet(<key>.encode())

@app.route('/decode', methods=['GET'])
def dec():
    valor = request.args.get('value')
    json = str(ferne.decrypt(valor.encode()))
    #print(valor)
    return str(json)

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0',port=20500)
