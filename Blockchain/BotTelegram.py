# -*- coding: utf-8 -*-
import base64
import os
import web3						#Se importan las librerias
from web3 import Web3, HTTPProvider

from telegram.ext import Updater
from telegram.ext import CommandHandler
import telegram
    
web3 = Web3(HTTPProvider(<IP:Port blockchain network>)) 	#Se establece la conexion

#Se crea la conexion con el bot
TokenPhotoBlock = <telegram token>
updater = Updater(token=TokenPhotoBlock)
dispatcher = updater.dispatcher

def main():
    def newAddress(bot, update, args):
        chatID=update.message.chat_id
        key = str(args[0])
        address = web3.personal.newAccount(key)
        bot.send_message(chat_id=update.message.chat_id, text='Tu nueva dirección es:')
        bot.send_message(chat_id=update.message.chat_id, text=address)
    
    def start(bot, update):
        updates = bot.get_updates()
        chatID=update.message.chat_id
        bot.send_message(chat_id=chatID, text="Comandos:  ")
        bot.send_message(chat_id=chatID, text="/Direccion {Key}")
    
    dispatcher.add_handler(CommandHandler("Direccion", newAddress, pass_args=True)) 
    dispatcher.add_handler(CommandHandler('Start', start))
    
    updater.start_polling()
    updater.idle()
    
if __name__ == '__main__':
    main()