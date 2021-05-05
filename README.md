# LoRa Hop Protocol

This project was developed on the end of summer 2021.
I use pieces of code developed for K. Herman on Chilean Scientific Antartic Expedition (summer 2020-2021)

My contribution was develop a Hop procol for test the maximun range of 100mW LoRa modules.
The idea was use one node like repetier, this node can recive a message, send this message to another node. The end node recive the meesege and return 
an acknowledgement message to ROOT NODE using Faro node.

Communiation flow was:
ROOT NODE -> FARO NODE -> END NODE -> FARO NODE -> ROOT NODE

Additionaly i include some libraries for sx1278 LoRa module for future works.
