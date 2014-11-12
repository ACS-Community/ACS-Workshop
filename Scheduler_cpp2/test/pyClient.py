from Acspy.Clients.SimpleClient import PySimpleClient
client = PySimpleClient()
m=client.getComponent("SchedulerCpp2")
m.start()



