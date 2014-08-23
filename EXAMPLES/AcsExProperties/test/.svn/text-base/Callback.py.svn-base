import ACS__POA

class Callback(ACS__POA.CBdouble):

    def __init__(self):
        self.f = open('monitoring_data.txt', 'w')

    def __del__(self):
        self.f.close() 

    def negotiate(self, time_to_transmit, desc):
        pass

    def working(self, value, completion, desc):
        self.f.write(str(completion.timeStamp) + ' ' + str(value) + '\n')
        self.f.flush()

    def done(self, value, completion, desc):
        self.f.write(str(completion.timeStamp) + ' ' + str(value) + '\n')
        self.f.flush()

