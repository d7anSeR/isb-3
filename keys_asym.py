import os

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization


def gen_serial_assym_keys(path_public_key, path_private_key):
    keys = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048)
    private_key = keys
    public_key = keys.public_key()
    with open(os.path.join(path_public_key, "public_key.pem"), 'wb') as public_out:
        public_out.write(public_key.public_bytes(encoding=serialization.Encoding.PEM,
                                                    format=serialization.PublicFormat.SubjectPublicKeyInfo))
    with open(os.path.join(path_private_key, "private_key.pem"), 'wb') as private_out:
        private_out.write(private_key.private_bytes(encoding=serialization.Encoding.PEM, format=serialization.PrivateFormat.TraditionalOpenSSL, encryption_algorithm=serialization.NoEncryption()))
    return public_key   
