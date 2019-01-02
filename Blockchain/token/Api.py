from flask import Flask, request
import sys					
from web3 import Web3
import Token

app = Flask(__name__)

@app.route('/token/balance', methods=['GET'])
def Balancce():
    address = request.args.get('address')
    json = Token.Balance(address)
    return str(json)

@app.route('/token/transfer', methods=['GET'])
def Transfer():
    Afrom = request.args.get('from')
    Ato = request.args.get('to')
    QTrans = request.args.get('coins')
    Unlock = request.args.get('unlock')
    
    json = Token.Transfer(Afrom, Ato ,QTrans, Unlock)
    return str(json)

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0',port=16000)