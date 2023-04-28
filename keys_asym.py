import logging

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding as padding1
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.serialization import load_pem_private_key


def gen_serial_assym_keys(path_public_key: str, path_private_key: str) -> rsa.RSAPublicKey:
    '''generation and implementation of asymmetric keys'''
    try:
        keys = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048)
        private_key = keys
        public_key = keys.public_key()
        with open(path_public_key, 'wb') as public_out:
            public_out.write(public_key.public_bytes(encoding=serialization.Encoding.PEM,
                                                     format=serialization.PublicFormat.SubjectPublicKeyInfo))
        with open(path_private_key, 'wb') as private_out:
            private_out.write(private_key.private_bytes(encoding=serialization.Encoding.PEM,
                              format=serialization.PrivateFormat.TraditionalOpenSSL, encryption_algorithm=serialization.NoEncryption()))
        return public_key
    except Exception as e:
        logging.error(
            f"there was an error opening the file or generating keys: {str(e)}")
        raise Exception("error")


def deserial_key_and_dec(path_sym: str, path_private: str) -> bytes:
    '''deserialization of asymmetric keys'''
    dc_text = ''
    d_private_key = 0
    try:
        with open(path_sym, 'rb') as sym_in, open(path_private, 'rb') as private_in:
            dc_text = sym_in.read()
            private_bytes = private_in.read()
            d_private_key = load_pem_private_key(private_bytes, password=None)
        return d_private_key.decrypt(dc_text, padding1.OAEP(mgf=padding1.MGF1(algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None))
    except Exception as e:
        logging.error(f"there was an error opening the file: {str(e)}")
        raise Exception("error")
