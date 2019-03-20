export OPENWINHOME=/usr/openwin
export LM_LICENSE_FILE=/etc/opt/licenses/licenses_combined

source ${UML_HOME}/login.sh
source ${UML_HOME}/etc/environment.sh

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$UML_HOME/api/lib"
export TBK_SCHEMATA="UML_schema tbk os"
export DBI_MAX_CONCURRENT_FSDS=16
export TBK_HASH=$UML_HOME/tools/UML.tool
export TBK_PATH=$UML_HOME/api/tools/API.tool
export TCL_VERSION=82
export TCL_ROOT=$UML_HOME/tcl
export TCL_LIBRARY=$UML_HOME/tcl/lib/tcl8.2
export TCLLIBPATH=$UML_HOME/tcl/lib
export OOA_PERL_EXEC=$UML_HOME/perl/bin
