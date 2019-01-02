import sys
import time
import web3						
from web3 import Web3, HTTPProvider, IPCProvider
import abitoken

web3 = Web3(HTTPProvider(<IP:Port blockchain network>))

abi = abitoken.abi()
'''
'''
scAddress = "0x8da33b52ad54b6eb0c7d9304f3d29044f3557091"
scAddress = Web3.toChecksumAddress(scAddress)

contrato = web3.eth.contract(address=scAddress, abi=abi);

def Balance (walletid):
    try:
        assert type (walletid) == str
    except:
        return("Error")
    walletidC = Web3.toChecksumAddress(walletid)
    QCoin = contrato.call().balanceOf(walletidC)
    return QCoin

def Transfer (Afrom, Ato ,QTrans, Unlock):
    print(type(Unlock))
    AfromC = Web3.toChecksumAddress(Afrom)
    AtoC = Web3.toChecksumAddress(Ato)
    web3.personal.unlockAccount(AfromC, Unlock, 1);
    contrato.transact({'from': AfromC, 'gas': 4700000}).transfer(AtoC,int(QTrans))
    time.sleep(1)
    return("ok")
    
def Approve (From, Quant, Who, Unlock):
    From = Web3.toChecksumAddress(From)
    Who = Web3.toChecksumAddress(Who)
    web3.personal.unlockAccount(From, Unlock, 100);
    resp = contrato.transact({'from': From, 'gas': 4700000}).approve(Who, Quant)
    return (resp)
    
    
