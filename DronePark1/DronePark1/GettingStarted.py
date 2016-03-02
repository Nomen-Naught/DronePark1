from PythonQt import *

# Functions and stuff

def _print(message):
	print("DEBUG: " + message)
	
def _sigTest():
	contInt.sigTest()
	_print("sigTest called")
	
def _atNextSpot():
	contInt.atNextSpot()
	_print("atNextSpot called")
	
def _connFail():
	contInt.connFail()
	_print("connFail called")

# Program Start
	
_print("Python Started")

test = 1

_connFail()

_atNextSpot()

_connFail()

while 1:
	test = test + 1
	_print(str(test))

_connFail()

_atNextSpot()
    

