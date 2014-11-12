from Acspy.Clients.SimpleClient import PySimpleClient
client = PySimpleClient()
m=client.getComponent("CONSOLE_CPP")
m.cameraOn()
m.cameraOff()
client.releaseComponent("CONSOLE_CPP")
