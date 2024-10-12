[[Project_Plan.excalidraw | Project diagram plan]]
1. we are using FirebaseESP8266.h library as per the [[library test for Firebase | test report]].
2. The plan of using EEPROM is eliminated because of the following reasons.
	1. It will make the code lengthy and will cause problem in uploading to the board.
	2. It is difficult to handle the large RFID card number in the ROM 
	3. There is no need of using it because there will be continuous internet and power, even the bypass system can also be used if there is any issues.
3. A temporary code is designed to keep the system working until the project is done completely. file location: ARC_Lock/temp/FirebaseESP8266_temp
4. 