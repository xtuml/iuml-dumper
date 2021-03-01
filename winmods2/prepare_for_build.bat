REM Script to copy from GITHUB Download to prepare to build

robocopy /S ..\..\iuml-dumper-master\unix\ D:\iUML_dumper\

REM Add Windows updates from Github to the Unix build

copy ..\..\iuml-dumper-master\winmods\APIGenMain.cc 			D:\iUML_dumper\apigen\src\ 

copy "..\..\iuml-dumper-master\winmods\CMakeLists (within iumlapi).txt"	D:\iUML_dumper\iumlapi\CMakeLists.txt 
copy ..\..\iuml-dumper-master\winmods\Entity.cc				D:\iUML_dumper\iumlapi\src\Entity.cc
copy ..\..\iuml-dumper-master\winmods\iUMLAPI.cc			D:\iUML_dumper\iumlapi\src\iUMLAPI.cc

REM 23 files
copy ..\..\iuml-dumper-master\winmods\Bridge.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\BridgeEvent.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\BridgeService.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\BridgeService.hh 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Callable.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\CodeFile.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Domain.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\DomainService.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\DomainService.hh 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\DumpModel.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\DumpOptions.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Event.cc 				D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\EventAction.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Object.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\ObjectInstanceService.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\ObjectInstanceService.hh 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Project.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\ProjectDomain.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\Service.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\StateAction.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\StateAction.hh 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\TextUtils.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods\UpdateModel.cc 			D:\iUML_dumper\maslgen\src\ 

REM 6 files
copy ..\..\iuml-dumper-master\winmods\CreateTags.cc 			D:\iUML_dumper\utils\src\ 
copy ..\..\iuml-dumper-master\winmods\DuplicateVersion.cc 		D:\iUML_dumper\utils\src\ 
copy ..\..\iuml-dumper-master\winmods\FreezeVersion.cc 			D:\iUML_dumper\utils\src\ 
copy ..\..\iuml-dumper-master\winmods\MaslTags.cc 			D:\iUML_dumper\utils\src\ 
copy ..\..\iuml-dumper-master\winmods\ShowVersions.cc 			D:\iUML_dumper\utils\src\ 
copy ..\..\iuml-dumper-master\winmods\TranslatorTags.cc 		D:\iUML_dumper\utils\src\ 

REM - New Files to enhance Windows fuctionality further.
copy ..\..\iuml-dumper-master\winmods2\Source\Utils.cc 			D:\iUML_dumper\iumlapi\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Utils.hh 			D:\iUML_dumper\iumlapi\include\iumlapi\

copy ..\..\iuml-dumper-master\winmods2\Source\Attribute.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Bridge.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Callable.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\CodeFile.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Datatype.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Domain.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Domain.hh 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Event.cc 			D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Object.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Project.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\ProjectDomain.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Relationship.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\Scenario.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\SuperSubtype.cc 		D:\iUML_dumper\maslgen\src\ 
copy ..\..\iuml-dumper-master\winmods2\Source\TagSet.cc 		D:\iUML_dumper\maslgen\src\ 

REM Delete old UNIX files
del D:\iUML_dumper\iuml.patch
del D:\iUML_dumper\README
del D:\iUML_dumper\setupenv.sh

REM Create a build directory and populate it with the cmake scripts
mkdir D:\iUML_dumper\build
copy ..\..\iuml-dumper-master\winmods2\BuildPrep\*.* 			D:\iUML_dumper\build\



