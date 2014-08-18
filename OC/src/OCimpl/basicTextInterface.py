import os
import sys
from Acspy.Clients.SimpleClient import PySimpleClient
from MARSCommon_idl import _0_MARS as TYPES

#Set up the stuff
myComponent=PySimpleClient.getInstance()
ref=myComponent.getComponent("OC")

def getOption(numberOfOptions, displayStr, showMenu):
  while True:
    if showMenu:
      printMenu()
    userInput=raw_input(displayStr+" (0-%d): " % (numberOfOptions-1))
    if userInput.isdigit():
      selection=int(userInput)
      if selection<numberOfOptions and selection>-1:
        return selection
      else:
        print ""
        print ""
        print "ERROR: value out of range. Please try again."
        print ""
    else:
      print ""
      print ""
      print "ERROR: Please enter a number!"
      print ""
    if showMenu:
      printMenu


#This function gets a robot report from the TRD (Task and Report DB)
#TODO: implement
def runGetReport():
  print "Running getReport...."
  print "The possible reports are:"
  reportList=ref.getReportsList()
  counter=0
  for l in reportList:
    print "%d) %d" % (counter, l)
    counter+=1
  myOption=getOption(len(reportList), "Please enter an option number", False)
  toad=ref.getReport(reportList[myOption])
  if toad.isSuccessful:
    print "The last operation was successful."
  else:
    print "The last operation was not successful. We are going to blame this on ITS."
  print "The ID is %d." % toad.id
  print "The robot type is: %s" % toad.rabType
  print "The error message is: %s" % toad.errorMsg

#This function gets the robot status from the TS (Task Scheduler)
#TODO: implement
def runGetRobotStatus():
  print "Running getRobotStatus..."
  print "The possible robots are"
  robotList=ref.getRobotsList()
  counter=0
  for l in robotList:
    print "%d) %d" % (counter, l)
    counter+=1
  myOption=getOption(len(robotList), "Please enter an option number", False)
  cheeseCake=ref.getRobotStatus(robotList[myOption])
  print "The ID is %d." % cheeseCake.id
  print "The type is %d." % cheeseCake.type
  print "The position is (%d, %d)." % (cheeseCake.position.x, cheeseCake.position.y)
  print "Position map:"
  print "  ",
  for eggs in range (0,6):
    print (" %d" % (eggs)),
  print ""
  for iceCream in range(0,6):
    print ("%d: " % iceCream),
    for cake in range(0,6):
      if cake==cheeseCake.position.x and iceCream==cheeseCake.position.y:
        print "X ",
      else:
        print "_ ",
    print ""
  print ""
  if cheeseCake.isOperational:
    print "The robot is operational."
  else:
    print "The robot is not operational. Please ask ITS to fix it..."
  if cheeseCake.isParked:
    print "The robot is parked!"
  else:
    print "The robot is not parked." 

#This function gets the sensor status from FD (Failure Detection)
#TODO: implement
def runGetSensorStatus():
  print "Running getSensor status..."
  print "The possible sensors are"
  sensorList=ref.getSensorsList()
  counter=0
  for l in sensorList:
    print "%d) %d" % (counter, l)
    counter+=1
  myOption=getOption(len(sensorList), "Please enter an option number", False)
  toast=ref.getSensorStatus(sensorList[myOption])
  print "The ID is %d." % toast.id
  print "The type is %d." % toast.type
  print "The position is (%d, %d)." % (toast.position.x, toast.position.y)

  print "  ",
  for eggs in range (0,6):
    print (" %d" % (eggs)),
  print ""
  for iceCream in range(0,6):
    print ("%d: " % iceCream),
    for cake in range(0,6):
      if cake==toast.position.x and iceCream==toast.position.y:
        print "X ",
      else:
        print "_ ",
    print ""
  print ""


  if toast.isEnabled:
    print "This sensor is enabled."
  else:
    print "This sensor is disabled."
  if toast.isAlarmActive:
    print "No alarms active."
  else:
    print "ALARM ACTIVE! Please call the operator at the OSF to resolve this situation."

#This function asks the TS to run a manual RAB.
#TODO: implement
def runRunManualRAB():
  print "Running Manual RAB..."
  xVal=getOption(6, "Please enter X ", False)
  yVal=getOption(6, "Please enter Y ", False)
  myType=getOption(3, "Please enter the type: ", False)
  print "Running RAB with X=%d, Y=%d, and type=%d" % (xVal, yVal, myType)
  ref.executeRAB(myType, TYPES.pos_struct(xVal, yVal))


def printMenu():
  print ""
  print ""
  print "-----------------Robot Option Menu----------------"
  print "Your options are: "
  print "0: Quit"
  print "1: Get report."
  print "2: Get robot status."
  print "3: Get sensor status."
  print "4: Run manual RAB."

while True:
  nextCommand=getOption(5, "Please enter a number", True)
  sys.stdout.write(os.popen('clear').read())
  print ""
  print ""
  if nextCommand==0:
    break
  elif nextCommand==1:
    runGetReport()
  elif nextCommand==2:
    runGetRobotStatus()
  elif nextCommand==3:
    runGetSensorStatus()
  else:
    runRunManualRAB()


del myComponent
del ref
