import os

from PyQt5.QtWidgets import QWidget, QLineEdit, QPushButton, QLabel, QFileDialog, QMessageBox, QPushButton, QVBoxLayout, QTabWidget, QHBoxLayout
from PyQt5.QtGui import QIcon, QDoubleValidator

from functools import partial


class ProcessWindow(QWidget):
    def __init__(self) -> None:
        '''class constructor'''
        super().__init__()
        self.__lable = QLabel(self)
        self.initUI()

    def initUI(self) -> None:
        '''the method that sets the main parameters of the window'''
        self.setGeometry(100, 400, 1500, 300)
        self.setWindowTitle('Encryption process')
        self.show()

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
        layout_1.addWidget(QLabel('File:'))
        layout_1.addWidget(self.filename_edit_sim_key)
        layout_1.addWidget(file_browse1)
        layout_1.addSpacing(100)
        layout_gen.addLayout(layout_1)
        layout_2.addWidget(QLabel("select the path where the public key will be serialized\n"))
        file_browse2 = QPushButton('Browse')
        file_browse2.clicked.connect(partial(self.insert_dir, 1))
        self.filename_edit_public_key = QLineEdit()
        self.filename_edit_public_key.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_2.addWidget(QLabel('File:'))
        layout_2.addWidget(self.filename_edit_public_key)
        layout_2.addWidget(file_browse2)
        layout_2.addSpacing(100)
        layout_gen.addLayout(layout_2)
        layout_3.addWidget(QLabel("select the path where the private key will be serialized\n"))
        file_browse3 = QPushButton('Browse')
        file_browse3.clicked.connect(partial(self.insert_dir, 2))
        self.filename_edit_private_key = QLineEdit()
        self.filename_edit_private_key.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_3.addWidget(QLabel('File:'))
        layout_3.addWidget(self.filename_edit_private_key)
        layout_3.addWidget(file_browse3)
        layout_3.addSpacing(100)
        layout_gen.addLayout(layout_3) 
        layout.addLayout(layout_gen)
        path_tab1.setLayout(layout)      
        return path_tab1

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
        layout_text.addWidget(QLabel('File:'))
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
        layout_enc_text.addWidget(QLabel('File:'))
        layout_button_enc = QHBoxLayout()
        layout_button_enc.addWidget(self.filename_edit_enc_text)
        layout_button_enc.addWidget(file_browse_enc_text)
        layout_button_enc.addSpacing(100)
        layout_enc_text.addLayout(layout_button_enc)
        layout_enc.addLayout(layout_enc_text)
        path_tab2.setLayout(layout_enc)      
        return path_tab2
    

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
        layout.addWidget(QLabel('File:'))
        layout_button_dec = QHBoxLayout()
        layout_button_dec.addWidget(self.filename_edit_dec_text)
        layout_button_dec.addWidget(file_browse)
        layout.addLayout(layout_button_dec)
        layout_dec.addLayout(layout)
        path_tab_dec.setLayout(layout_dec)      
        return path_tab_dec
    
    def enc_process(self) -> QWidget:
        '''method for working with dataset by tasks'''
        path_tab_dec = QWidget()
        layout_dec = QHBoxLayout()
        layout_dec_text = QVBoxLayout()
        layout_dec_text.addWidget(QLabel("Select the path where the decrypted test will be saved\n"))
        file_browse = QPushButton('Browse')
        file_browse.clicked.connect(partial(self.insert_dir, 5))
        self.filename_edit_dec_text = QLineEdit()
        self.filename_edit_dec_text.setValidator(QDoubleValidator(0.99,99.99,2))
        layout_dec_text.addWidget(QLabel('File:'))
        layout_dec_text.addWidget(self.filename_edit_dec_text)
        layout_dec_text.addWidget(file_browse)
        layout_dec_text.addSpacing(100)
        layout_dec.addLayout(layout_dec_text)
        path_tab_dec.setLayout(layout_dec)      
        return path_tab_dec

    def check_path(self) -> bool:
        for elem in self.massiv:
            if(elem == '\0'):
                return False
        return True

    def insert_dir(self, i) -> None:
        '''method for selecting the main file'''
        flag = True
        while flag:
            self.massiv[i] = QFileDialog.getOpenFileName(self)
            if  (os.path.splitext((self.massiv[i])[0]))[1] != ".txt":
                error = QMessageBox()
                error.setWindowTitle("Error")
                error.setText("error when selecting a file")
                error.exec_()
            else:
                flag = False
                if i == 0:
                    self.filename_edit_sim_key.setText(str(self.massiv[i][0]))
                if i == 1:
                    self.filename_edit_public_key.setText(str(self.massiv[i][0]))
                if i == 2:
                    self.filename_edit_private_key.setText(str(self.massiv[i][0]))
                if i == 3:
                    self.filename_edit_text.setText(str(self.massiv[i][0]))
                if i == 4:
                    self.filename_edit_enc_text.setText(str(self.massiv[i][0]))
                if i == 5:
                    self.filename_edit_dec_text.setText(str(self.massiv[i][0]))
                if self.check_path() == True:
                    self.w1 = ProcessWindow()
                    self.w1.show()
                