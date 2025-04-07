#!/usr/bin/python3

"""
Gui for sending custom zmq messages
using PyQt6
"""

import sys
import zmq
from PyQt6.QtWidgets import (
    QApplication,
    QWidget,
    QHBoxLayout,
    QPushButton,
    QMainWindow,
    QVBoxLayout,
    QHBoxLayout,
    QLineEdit,
)


class ZmqGui(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ctx = zmq.Context()
        self.pub = self.ctx.socket(zmq.PUB)
        self.pub.bind("tcp://*:5555")

        self.initUI()

    def initUI(self):
        # buttons to send predermined messages
        topic1Btn = QPushButton()
        topic1Btn.setText("Send Topic1")
        topic1Btn.clicked.connect(self.sendTopic1Msg)

        topic2Btn = QPushButton()
        topic2Btn.setText("Send Topic2")
        topic2Btn.clicked.connect(self.sendTopic2Msg)

        topic3Btn = QPushButton()
        topic3Btn.setText("Send Topic3")
        topic3Btn.clicked.connect(self.sendTopic3Msg)

        topic4Btn = QPushButton()
        topic4Btn.setText("Send Topic4")
        topic4Btn.clicked.connect(self.sendTopic4Msg)

        topic5Btn = QPushButton()
        topic5Btn.setText("Send Topic5")
        topic5Btn.clicked.connect(self.sendTopic5Msg)

        # Add buttons to a vertical layout
        btnVBox = QVBoxLayout()
        # btnVBox.addStretch(1)
        btnVBox.addWidget(topic1Btn)
        btnVBox.addWidget(topic2Btn)
        btnVBox.addWidget(topic3Btn)
        btnVBox.addWidget(topic4Btn)
        btnVBox.addWidget(topic5Btn)

        # Construct Custom Topic VLayout
        self.topicEdit = QLineEdit()
        self.topicEdit.setWindowTitle("Topic")
        self.keyEdit = QLineEdit()
        self.keyEdit.setWindowTitle("Key")
        self.valueEdit = QLineEdit()
        self.valueEdit.setWindowTitle("Value")
        custBtn = QPushButton()
        custBtn.setText("Send Custom Topic Msg")
        custBtn.clicked.connect(self.sendCustTopicMsg)

        custVBox = QVBoxLayout()
        custVBox.addStretch()
        custVBox.addWidget(self.topicEdit)
        custVBox.addWidget(self.keyEdit)
        custVBox.addWidget(self.valueEdit)
        custVBox.addWidget(custBtn)

        # Add Vertical layout to topHbox
        topHbox = QHBoxLayout()
        # topHbox.addStretch(1)
        topHbox.addLayout(btnVBox)
        topHbox.addLayout(custVBox)

        centWidget = QWidget()
        centWidget.setLayout(topHbox)

        self.setWindowTitle("ZmqMsgGui")
        self.setCentralWidget(centWidget)
        self.show()

    # Make messages functions
    def sendZmqMsg(self, topic, key, value):
        self.pub.send_string(topic, zmq.SNDMORE)
        self.pub.send_json({key: value})

    def sendTopic1Msg(self):
        self.sendZmqMsg("Topic1", "key1", 100)

    def sendTopic2Msg(self):
        self.sendZmqMsg("Topic2", "key2", 200)

    def sendTopic3Msg(self):
        self.sendZmqMsg("Topic3", "key3", 300)

    def sendTopic4Msg(self):
        self.sendZmqMsg("Topic4", "key4", 400)

    def sendTopic5Msg(self):
        self.sendZmqMsg("Topic5", "key5", 500)

    def sendCustTopicMsg(self):
        topic = self.topicEdit.text()
        key = self.keyEdit.text()
        try:
            value = int(self.valueEdit.text())
        except Exception as e:
            value = 0
            print(e)
        self.sendZmqMsg(topic, key, value)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    gui = ZmqGui()
    sys.exit(app.exec())
