import os
from functools import partial

from PyQt5.QtWidgets import (QWidget, QLineEdit, QLabel, QFileDialog, QMessageBox,
                             QPushButton, QVBoxLayout, QTabWidget, QHBoxLayout)
from PyQt5.QtGui import QIcon, QDoubleValidator

from keys_asym import gen_serial_assym_keys, deserial_key_and_dec
from key_sym import gen_serial_sim_key, enc_text, decrypt_text


class MainWindow(QWidget):
    """class window with working functionality"""

    def __init__(self) -> None:
        """class constructor"""
        super().__init__()
        self.initUI()

    def initUI(self) -> None:
        """the method that sets the main parameters of the window"""
        self.setGeometry(100, 400, 1500, 200)
        self.setWindowTitle('Laboratory Work №3')
        self.length_key = '\0'
        self.dict_path = {
            "path_sym": '\0',
            "path_public": '\0',
            "path_private": '\0',
            "path_orig_text": '\0',
            "path_enc_text": '\0',
            "path_dec_text": '\0'
        }
        self.flag_gen = False
        self.flag_enc = False
        self.flag_dec = False
        self.setWindowIcon(QIcon('icon.jpg'))
        layout = QVBoxLayout()
        self.setLayout(layout)
        tabs = QTabWidget()
        tabs.addTab(self.generation(), "generation")
        tabs.addTab(self.encryption(), "encryption")
        tabs.addTab(self.decryption(), "decryption")
        layout.addWidget(tabs)
        self.show()

    def generation(self) -> QWidget:
        """the method that gets the paths to the keys and asks the user
        for the length of the symmetric from 32 to 448 bits"""
        path_tab1 = QWidget()
        layout = QVBoxLayout()
        layout_gen = QHBoxLayout()
        layout_1 = QVBoxLayout()
        layout_2 = QVBoxLayout()
        layout_3 = QVBoxLayout()
        layout.addWidget(QLabel(
            "Attention: when you select all the paths, you will be transferred to the encryption process window\n\n\n"))
        layout_1.addWidget(QLabel(
            "select the path by which the encrypted symmetric key will be serialized\n"))
        file_browse1 = QPushButton('Browse')
        file_browse1.clicked.connect(partial(self.insert_dir, "path_sym"))
        self.filename_edit_sim_key = QLineEdit()
        self.filename_edit_sim_key.setValidator(
            QDoubleValidator(0.99, 99.99, 2))
        layout_1.addWidget(QLabel('Folder:'))
        layout_1.addWidget(self.filename_edit_sim_key)
        layout_1.addWidget(file_browse1)
        layout_1.addSpacing(100)
        layout_gen.addLayout(layout_1)
        layout_2.addWidget(
            QLabel("select the path where the public key will be serialized\n"))
        file_browse2 = QPushButton('Browse')
        file_browse2.clicked.connect(partial(self.insert_dir, "path_public"))
        self.filename_edit_public_key = QLineEdit()
        self.filename_edit_public_key.setValidator(
            QDoubleValidator(0.99, 99.99, 2))
        layout_2.addWidget(QLabel('Folder:'))
        layout_2.addWidget(self.filename_edit_public_key)
        layout_2.addWidget(file_browse2)
        layout_2.addSpacing(100)
        layout_gen.addLayout(layout_2)
        layout_3.addWidget(
            QLabel("select the path where the private key will be serialized\n"))
        file_browse3 = QPushButton('Browse')
        file_browse3.clicked.connect(partial(self.insert_dir, "path_private"))
        self.filename_edit_private_key = QLineEdit()
        self.filename_edit_private_key.setValidator(
            QDoubleValidator(0.99, 99.99, 2))
        layout_3.addWidget(QLabel('Folder:'))
        layout_3.addWidget(self.filename_edit_private_key)
        layout_3.addWidget(file_browse3)
        layout_3.addSpacing(100)
        layout_gen.addLayout(layout_3)
        layout.addLayout(layout_gen)
        layout_enter = QHBoxLayout()
        self.len_key = QLineEdit()
        layout_enter.addWidget(self.len_key)
        self.button_enter = QPushButton('Enter')
        self.button_enter.clicked.connect(self.len_key_fun)
        layout_enter.addWidget(self.button_enter)
        layout.addWidget(
            QLabel("Enter key length from 32 to 448 bits in 8-bit increments"))
        layout.addWidget(QLabel("Note: you must specify the length in bits"))
        layout.addLayout(layout_enter)
        self.button_gen = QPushButton("generation keys")
        self.button_gen.clicked.connect(self.tmp_fun_gen)
        layout.addWidget(self.button_gen)
        path_tab1.setLayout(layout)
        return path_tab1

    def tmp_fun_gen(self) -> None:
        """the method calls the key generation functions"""
        if self.flag_gen == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed generation...")
            error.exec_()
        elif self.dict_path["path_sym"] == '\0' or self.dict_path["path_public"] == '\0' or self.dict_path["path_private"] == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen all the paths...")
            error.exec_()
        elif self.length_key == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't input length of key...")
            error.exec_()
        else:
            public_key = gen_serial_assym_keys(
                self.dict_path["path_public"], self.dict_path["path_private"])
            gen_serial_sim_key(
                self.dict_path["path_sym"], public_key, self.length_key)
            self.flag_gen = True
            self.button_gen.setStyleSheet("background-color : green")

    def encryption(self) -> QWidget:
        """the method that gets the paths to the source text and to the encrypted one"""
        path_tab2 = QWidget()
        layout_enc = QVBoxLayout()
        layout_text = QVBoxLayout()
        layout_enc_text = QVBoxLayout()
        layout_text.addWidget(
            QLabel("Select the path to get text file for encryption"))
        file_browse_text = QPushButton('Browse')
        file_browse_text.clicked.connect(self.insert_text)
        self.filename_edit_text = QLineEdit()
        self.filename_edit_text.setValidator(QDoubleValidator(0.99, 99.99, 2))
        layout_text.addWidget(QLabel('File:'))
        layout_button_text = QHBoxLayout()
        layout_button_text.addWidget(self.filename_edit_text)
        layout_button_text.addWidget(file_browse_text)
        layout_button_text.addSpacing(100)
        layout_text.addLayout(layout_button_text)
        layout_enc.addLayout(layout_text)
        layout_enc_text.addWidget(
            QLabel("\nSelect the path to save the encrypted text file"))
        file_browse_enc_text = QPushButton('Browse')
        file_browse_enc_text.clicked.connect(
            partial(self.insert_dir, "path_enc_text"))
        self.filename_edit_enc_text = QLineEdit()
        self.filename_edit_enc_text.setValidator(
            QDoubleValidator(0.99, 99.99, 2))
        layout_enc_text.addWidget(QLabel('Folder:'))
        layout_button_enc = QHBoxLayout()
        layout_button_enc.addWidget(self.filename_edit_enc_text)
        layout_button_enc.addWidget(file_browse_enc_text)
        layout_button_enc.addSpacing(100)
        layout_enc_text.addLayout(layout_button_enc)
        self.button_enc = QPushButton("encryption text")
        self.button_enc.clicked.connect(self.tmp_fun_enc)
        layout_enc.addLayout(layout_enc_text)
        layout_enc.addWidget(self.button_enc)
        path_tab2.setLayout(layout_enc)
        return path_tab2

    def tmp_fun_enc(self) -> None:
        """the method that calls text encryption functions"""
        if self.dict_path["path_orig_text"] == '' or self.dict_path["path_enc_text"] == '':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen all the paths...")
            error.exec_()
        elif self.flag_gen == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't generate keys ...")
            error.exec_()
        elif self.flag_enc == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed encryption...")
            error.exec_()
        elif self.length_key == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't input length of key...")
            error.exec_()
        else:
            self.flag_enc = True
            enc_text(str(self.dict_path["path_enc_text"]), str(self.dict_path["path_orig_text"]), deserial_key_and_dec(
                self.dict_path["path_sym"], self.dict_path["path_private"]))
            self.button_enc.setStyleSheet("background-color : green")

    def decryption(self) -> QWidget:
        """the method that gets the path to the decrypted text"""
        path_tab_dec = QWidget()
        layout_dec = QVBoxLayout()
        layout = QVBoxLayout()
        layout_dec.addWidget(
            QLabel("Select the path where the decrypted test will be saved\n"))
        file_browse = QPushButton('Browse')
        file_browse.clicked.connect(partial(self.insert_dir, "path_dec_text"))
        self.filename_edit_dec_text = QLineEdit()
        self.filename_edit_dec_text.setValidator(
            QDoubleValidator(0.99, 99.99, 2))
        layout.addWidget(QLabel('Folder:'))
        layout_button_dec = QHBoxLayout()
        layout_button_dec.addWidget(self.filename_edit_dec_text)
        layout_button_dec.addWidget(file_browse)
        layout.addLayout(layout_button_dec)
        self.button_dec = QPushButton("decryption text")
        self.button_dec.clicked.connect(self.tmp_fun_dec)
        layout.addWidget(self.button_dec)
        layout_dec.addLayout(layout)
        path_tab_dec.setLayout(layout_dec)
        return path_tab_dec

    def len_key_fun(self) -> None:
        """the method that checks the input length for the key"""
        flag = False
        for i in range(32, 449, 8):
            if self.len_key.text() == str(i):
                self.length_key = self.len_key.text()
                flag = True
                self.button_enter.setStyleSheet("background-color : green")
            elif i == 448 and flag == False:
                error = QMessageBox()
                error.setWindowTitle("Error")
                error.setText("error: you entered an incorrect value...")
                self.button_enter.setStyleSheet("background-color : white")
                error.exec_()

    def tmp_fun_dec(self) -> None:
        """the method that calls decryption functions for text"""
        if self.dict_path["path_dec_text"] == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen the path...")
            error.exec_()
        elif self.flag_gen == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't generate keys ...")
            error.exec_()
        elif self.flag_enc == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't encrypt the text ...")
            error.exec_()
        elif self.flag_dec == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed decryption...")
            error.exec_()
        elif self.length_key == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't input length of key...")
            error.exec_()
        else:
            self.button_dec.setStyleSheet("background-color : green")
            decrypt_text(self.dict_path["path_dec_text"], self.dict_path["path_enc_text"], deserial_key_and_dec(
                self.dict_path["path_sym"], self.dict_path["path_private"]))
            self.flag_dec = True

    def insert_text(self) -> None:
        """the method that gets the path to the original file"""
        flag = True
        name = QFileDialog.getOpenFileName(self)
        if (os.path.splitext(name[0]))[1] != ".txt":
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error when selecting a file")
            error.exec_()
        else:
            self.dict_path["path_orig_text"] = name[0]
            self.flag_enc = False
            self.filename_edit_text.setText(str(self.dict_path["path_orig_text"]))
            self.button_enc.setStyleSheet("background-color : white")

    def insert_dir(self, str_dict: str) -> None:
        """the method that gets the path to the encrypted file"""
        name = QFileDialog.getExistingDirectory(self)
        if str_dict == "path_sym":
            self.dict_path[str_dict] = os.path.join(name, "sym.txt")
            self.flag_gen = False
            self.filename_edit_sim_key.setText(self.dict_path[str_dict])
            self.button_gen.setStyleSheet("background-color : white")
        if str_dict == "path_public":
            self.dict_path[str_dict] = os.path.join(
                name, "public_key_asym.pem")
            self.flag_gen = False
            self.filename_edit_public_key.setText(self.dict_path[str_dict])
            self.button_gen.setStyleSheet("background-color : white")
        if str_dict == "path_private":
            self.dict_path[str_dict] = os.path.join(
                name, "private_key_asym.pem")
            self.flag_gen = False
            self.filename_edit_private_key.setText(self.dict_path[str_dict])
            self.button_gen.setStyleSheet("background-color : white")
        if str_dict == "path_enc_text":
            self.dict_path[str_dict] = os.path.join(name, "enc_text.txt")
            self.flag_enc = False
            self.filename_edit_enc_text.setText(self.dict_path[str_dict])
            self.button_enc.setStyleSheet("background-color : white")
        if str_dict == "path_dec_text":
            self.dict_path[str_dict] = os.path.join(name, "dec_text.txt")
            self.flag_dec = False
            self.filename_edit_dec_text.setText(self.dict_path[str_dict])
            self.button_dec.setStyleSheet("background-color : white")
