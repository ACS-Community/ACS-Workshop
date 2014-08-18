ALTAZ_ROOT=/home/maray/devel/scarlet
ACS_ROOT=/alma/ACS-5.0
INTROOT=/home/maray/introot

CLASSPATH=""

DIREC=$ACS_ROOT/ACSSW/lib
for x in $(ls -R $DIREC |grep ".jar");
do
	CLASSPATH=$CLASSPATH:$DIREC/$x;
done

DIREC=$ACS_ROOT/java/j2sdk1.4.2/lib
for x in $(ls -R $DIREC |grep ".jar");
do 
	CLASSPATH=$CLASSPATH:$DIREC/$x;
done

CLASSPATH=$CLASSPATH:$ALTAZ_ROOT/lib/AltAzAbeans.jar
CLASSPATH=$CLASSPATH:$ALTAZ_ROOT/lib/AltAz.jar

export CLASSPATH

export INTROOT
export IDL_PATH="$IDL_PATH -I$ALTAZ_ROOT/idl"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ALTAZ_ROOT/lib
export ACS_CDB=$ALTAZ_ROOT/test
echo $CLASSPATH
