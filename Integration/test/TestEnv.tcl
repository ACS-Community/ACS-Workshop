global PID

set ACSDATA $env(PWD)
set ACS_TMP2 $env(PWD)/tmp
set env(ACS_TMP) $ACS_TMP2
set env(ACS_LOG_STDOUT) 2
set env(ACS_CDB) $env(PWD)
set env(MODPATH) 1
set env(ACS_STARTUP_TIMEOUT_MULTIPLIER) 10


