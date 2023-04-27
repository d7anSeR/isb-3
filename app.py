import os

from PyQt5.QtWidgets import QWidget, QLineEdit, QPushButton, QLabel, QFileDialog, QMessageBox, QPushButton, QVBoxLayout, QTabWidget, QHBoxLayout
from PyQt5.QtGui import QIcon, QDoubleValidator

from functools import partial

from keys_asym import gen_serial_assym_keys
from key_sim import gen_serial_sim_key


class MainWindow(QWidget):

    def __init__(self) -> None:
        '''class constructor'''
        super().__init__()
        self.__lable = QLabel(self)
        self.initUI()

    def initUI(self) -> None:
        '''the method that sets the main parameters of the window'''
        self.setGeometry(100, 400, 1500, 200)
        self.setWindowTitle('Laboratory Work №3')
        self.massiv = []
        self.flag_gen = self.flag_enc = self.flag_dec = False
        for i in range(0, 6):
            self.massiv.append('\0')
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
        '''method for working with dataset by tasks'''
        path_tab1 = QWidget()
        layout = QVBoxLayout()
        layout_gen = QHBoxLayout()
        layout_1 = QVBoxLayout()
        layout_2 = QVBoxLayout()
        layout_3 = QVBoxLayout()
        layout.addWidget(QLabel("Attention: when you select all the paths, you will be transferred to the encryption process window\n\n\n"))
        layout_1.addWidget(QLabel("select the path by which the encrypted symmetric key will be serialized\n"))
        file_browse1 = QPushButton('Browse')
        file_browse1.clicked.connect(partial(self.insert_dir, 0))
        self.filename_edit_sim_key = QLineEdit()
        self.filename_edit_sim_key.setValidator(QDoubleValidator(0.99,99.99,2))    
        layout_1.addWidget(QLabel('Folder:'))
        layout_1.addWidget(self.filename_edit_sim_key)
        layout_1.addWidget(file_browse1)
        layout_1.addSpacing(100)
        layout_gen.addLayout(layout_1)
        layout_2.addWidget(QLabel("select the path where the public key will be serialized\n"))
        file_browse2 = QPushButton('Browse')
        file_browse2.clicked.connect(partial(self.insert_dir, 1))
        self.filename_edit_public_key = QLineEdit()
        self.filename_edit_public_key.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_2.addWidget(QLabel('Folder:'))
        layout_2.addWidget(self.filename_edit_public_key)
        layout_2.addWidget(file_browse2)
        layout_2.addSpacing(100)
        layout_gen.addLayout(layout_2)
        layout_3.addWidget(QLabel("select the path where the private key will be serialized\n"))
        file_browse3 = QPushButton('Browse')
        file_browse3.clicked.connect(partial(self.insert_dir, 2))
        self.filename_edit_private_key = QLineEdit()
        self.filename_edit_private_key.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_3.addWidget(QLabel('Folder:'))
        layout_3.addWidget(self.filename_edit_private_key)
        layout_3.addWidget(file_browse3)
        layout_3.addSpacing(100)
        layout_gen.addLayout(layout_3) 
        layout.addLayout(layout_gen)
        button_gen = QPushButton("generation keys")
        button_gen.clicked.connect(self.tmp_fun_gen)
        layout.addWidget(button_gen)
        path_tab1.setLayout(layout)   
        return path_tab1

    def tmp_fun_gen(self):
        if self.massiv[0] != '\0' and self.massiv[1] != '\0' and self.massiv[2] != '\0':
            self.gen_key
        if self.flag_gen == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed generation...")
            error.exec_()
        else:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen all the paths...")
            error.exec_()

    def gen_key(self):
        public_key = gen_serial_assym_keys(self.massiv[1], self.massiv[2])
        gen_serial_sim_key(self.massiv[0], public_key)
        self.flag_gen = True

    def encryption(self) -> QWidget:
        '''method for working with dataset by tasks'''
        path_tab2 = QWidget()
        layout_enc = QVBoxLayout()
        layout_text = QVBoxLayout()
        layout_enc_text = QVBoxLayout()
        layout_text.addWidget(QLabel("Select the path to get text file for encryption"))
        file_browse_text = QPushButton('Browse')
        file_browse_text.clicked.connect(partial(self.insert_dir, 3))
        self.filename_edit_text = QLineEdit()
        self.filename_edit_text.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_text.addWidget(QLabel('Folder:'))
        layout_button_text = QHBoxLayout()
        layout_button_text.addWidget(self.filename_edit_text)
        layout_button_text.addWidget(file_browse_text)
        layout_button_text.addSpacing(100)
        layout_text.addLayout(layout_button_text)
        layout_enc.addLayout(layout_text)
        layout_enc_text.addWidget(QLabel("\nSelect the path to save the encrypted text file"))
        file_browse_enc_text = QPushButton('Browse')
        file_browse_enc_text.clicked.connect(partial(self.insert_dir, 4))
        self.filename_edit_enc_text = QLineEdit()
        self.filename_edit_enc_text.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_enc_text.addWidget(QLabel('Folder:'))
        layout_button_enc = QHBoxLayout()
        layout_button_enc.addWidget(self.filename_edit_enc_text)
        layout_button_enc.addWidget(file_browse_enc_text)
        layout_button_enc.addSpacing(100)
        layout_enc_text.addLayout(layout_button_enc)
        button_enc = QPushButton("encryption text")
        button_enc.clicked.connect(self.tmp_fun_enc)
        layout_enc.addWidget(button_enc)
        layout_enc.addWidget(layout_enc_text)
        path_tab2.setLayout(layout_enc)      
        return path_tab2
    
    def tmp_fun_enc(self):
        if self.massiv[3] == '\0' or self.massiv[4] != '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen all the paths...")
            error.exec_()
        if self.flag_gen == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't generate keys ...")
            error.exec_()
        if self.flag_enc == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed encryption...")
            error.exec_()
        else:
            self.enc_key

    def enc_key(self):
        # public_key = gen_serial_assym_keys(self.massiv[1], self.massiv[2])
        # gen_serial_sim_key(self.massiv[0], public_key)
        self.flag_enc = True
    

    def decryption(self) -> QWidget:
        '''method for working with dataset by tasks'''
        path_tab_dec = QWidget()
        layout_dec = QVBoxLayout()
        layout = QVBoxLayout()
        layout_dec.addWidget(QLabel("Select the path where the decrypted test will be saved\n"))
        file_browse = QPushButton('Browse')
        file_browse.clicked.connect(partial(self.insert_dir, 5))
        self.filename_edit_dec_text = QLineEdit()
        self.filename_edit_dec_text.setValidator(QDoubleValidator(0.99,99.99,2))
        layout.addWidget(QLabel('Folder:'))
        layout_button_dec = QHBoxLayout()
        layout_button_dec.addWidget(self.filename_edit_dec_text)
        layout_button_dec.addWidget(file_browse)
        layout.addLayout(layout_button_dec)
        layout_dec.addLayout(layout)
        button_enc = QPushButton("decryption text")
        button_enc.clicked.connect(self.tmp_fun_dec)
        layout_dec.addWidget(button_enc)
        path_tab_dec.setLayout(layout_dec)           
        return path_tab_dec
    
    def tmp_fun_dec(self):
        if self.massiv[5] == '\0':
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have not chosen the path...")
            error.exec_()
        if self.flag_gen == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't generate keys ...")
            error.exec_()
        if self.flag_enc == False:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you didn't encrypt the text ...")
            error.exec_()
        if self.flag_dec == True:
            error = QMessageBox()
            error.setWindowTitle("Error")
            error.setText("error: you have already performed decryption...")
            error.exec_()
        else:
            self.dec_key

    def dec_key(self):
        # public_key = gen_serial_assym_keys(self.massiv[1], self.massiv[2])
        # gen_serial_sim_key(self.massiv[0], public_key)
        self.flag_enc = True

    def insert_dir(self, i) -> None:
        '''method for selecting the main file'''
        flag = True
        self.massiv[i] = QFileDialog.getExistingDirectory(self)
        if i == 0:
            self.flag_gen = False
            self.filename_edit_sim_key.setText(str(self.massiv[i][0]))
        if i == 1:
            self.flag_gen = False
            self.filename_edit_public_key.setText(str(self.massiv[i][0]))
        if i == 2:
            self.flag_gen = False
            self.filename_edit_private_key.setText(str(self.massiv[i][0]))
        if i == 3:
            self.flag_enc = False
            self.filename_edit_text.setText(str(self.massiv[i][0]))
        if i == 4:
            self.flag_enc = False
            self.filename_edit_enc_text.setText(str(self.massiv[i][0]))
        if i == 5:
            self.flag_dec = False
            self.filename_edit_dec_text.setText(str(self.massiv[i][0]))
                