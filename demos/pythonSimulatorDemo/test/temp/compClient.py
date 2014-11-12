from Acspy.Clients.SimpleClient import PySimpleClient
client = PySimpleClient()
m=client.getComponent("MOUNT")
m.objfix(3,6)
