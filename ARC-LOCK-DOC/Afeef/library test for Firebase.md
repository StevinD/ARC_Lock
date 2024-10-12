# Test Report
**date**: 15/09/2024
**Objective**: To find the library that has best write and read speed in esp8266.

## Test file:
ARC_Lock/test/FirebaseESP8266_test

## Options
1. ESP8266Firebase.h
2. FirebaseArduino.h
3. FirebaseESP8266.h

## Report
### ESP8266Firebase.h
It is the slowest library but it requires less maintenance.
Since we cannot afford slow transactions we would reject using it.

### FirebaseArduino.h
It requires lot of maintenance, also sometimes get stuck processing the transactions. It is used in the club until we found the best one.

### FirebaseESP8266.h
It is the best library that I have found, It can send data and receive data with in 200 ms. this is the best for all our database library.
![[Pasted image 20240916151538.png]]

The entire test code is present in test/FirebaseESP8266_test.
