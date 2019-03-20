#
# UK Crown Copyright (c) 2016. All Rights Reserved
#
add_library ( MaslGen SHARED 
        src/AssignerStateAction.cc
        src/AssignerStateTransitionTable.cc
        src/Attribute.cc
        src/Bridge.cc
        src/BridgeEvent.cc
        src/BridgeService.cc
        src/Callable.cc
        src/CodeFile.cc
        src/DataType.cc
        src/DeferredInstanceService.cc
        src/Domain.cc
        src/DomainService.cc
        src/Event.cc
        src/EventAction.cc
        src/External.cc
        src/Identifier.cc
        src/Object.cc
        src/ObjectInstanceService.cc
        src/Parameter.cc
        src/Project.cc 
        src/ProjectDomain.cc 
        src/ProjectTerminator.cc
        src/ProjectBridgeEvent.cc
        src/ProjectBridgeService.cc
        src/Relationship.cc
        src/Scenario.cc
        src/Service.cc
        src/StateAction.cc
        src/StateTransitionTable.cc
        src/SuperSubtype.cc
        src/SupplementaryCodeBlock.cc
        src/TagSet.cc
        src/TextUtils.cc
        src/DumpOptions.cc
)

target_link_libraries ( MaslGen iUMLAPI )

add_executable(
  UMLDumpModel 
    src/DumpModel.cc
  )
target_link_libraries (
  UMLDumpModel
    MaslGen
    ${UML_LIB_HACK}
  )

add_executable(
  UMLUpdateModel
    src/UpdateModel.cc
  )
target_link_libraries (
  UMLUpdateModel
    MaslGen
    ${UML_LIB_HACK}
  )

install ( 
  TARGETS
          UMLDumpModel 
          UMLUpdateModel
          MaslGen
  EXPORT  iUML
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
)
