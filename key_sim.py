import logging
import os

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.asymmetric import padding as padding1
from cryptography.hazmat.primitives import hashes


def gen_serial_sim_key(path, public_key: rsa.RSAPublicKey):
    sym_key = os.urandom(32)
    name = os.path.join(path, "key_sym.bin")
    text = public_key.encrypt(sym_key, padding1.OAEP(mgf=padding1.MGF1(
        algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None))
    try:
        with open(name, 'wb') as file:
            file.write(text)
    except Exception as e:
        logging.error(f"something went wrong with {name} and {str(e)}")
        raise Exception("error")