import os
import logging

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.asymmetric import padding as padding1
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


def gen_serial_sim_key(path: str, public_key: rsa.RSAPublicKey, len_key: str) -> None:
    '''symmetric key generation and serialization'''
    sym_key = os.urandom(int(len_key) // 8)
    text = public_key.encrypt(sym_key, padding1.OAEP(mgf=padding1.MGF1(
        algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None))
    try:
        with open(path, 'wb') as file:
            file.write(text)
    except Exception as e:
        logging.error(f"an error occurred while opening the file: {str(e)}")
        raise Exception("error")


def enc_text(path_save: str, path_text: str, key: bytes) -> None:
    '''encrypting the text and saving it to the specified path'''
    text = ''
    try:
        with open(path_text, 'r', encoding='utf-8') as text_first:
            text = text_first.read()
            padder = padding.ANSIX923(128).padder()
            text = bytes(text, 'UTF-8')
            padded_text = padder.update(text)+padder.finalize()
            iv = os.urandom(8)
            cipher = Cipher(algorithms.Blowfish(key), modes.CBC(iv))
            encryptor = cipher.encryptor()
            enc_text = encryptor.update(padded_text) + encryptor.finalize()
            with open(path_save, 'wb') as save_file:
                save_file.write(iv)
                save_file.write(enc_text)
    except Exception as e:
        logging.error(
            f"there was an error opening the file or encrypting the text: {str(e)}")
        raise Exception("error")


def decrypt_text(path_save: str, path_text: str, key: bytes) -> None:
    '''decrypting the text and saving it to the specified path'''
    text = ''
    iv = 0
    try:
        with open(path_text, 'rb') as data_in:
            iv = data_in.read(8)
            text = data_in.read()
        cipher = Cipher(algorithms.Blowfish(key), modes.CBC(iv))
        decryptor = cipher.decryptor()
        dc_text = decryptor.update(text) + decryptor.finalize()
        unpadder = padding.ANSIX923(128).unpadder()
        unpadded_dc_text = unpadder.update(dc_text) + unpadder.finalize()
        with open(path_save, 'w', encoding='utf-8') as data_out:
            data_out.write(unpadded_dc_text.decode('UTF-8', errors='ignore'))
    except Exception as e:
        logging.error(
            f"there was an error opening the file or decrypting the text: {str(e)}")
        raise Exception("error")
