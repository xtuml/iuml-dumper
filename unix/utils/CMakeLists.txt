#
# UK Crown Copyright (c) 2016. All Rights Reserved
#

add_executable(
  UMLShowVersions
    src/ShowVersions.cc
  )
target_link_libraries ( 
  UMLShowVersions 
    iUMLAPI 
    ${UML_LIB_HACK}
  )

add_executable(
  UMLFreezeVersion
    src/FreezeVersion.cc )
target_link_libraries ( 
  UMLFreezeVersion 
    iUMLAPI
    ${UML_LIB_HACK}
  )

add_executable(
  UMLDuplicateVersion
    src/DuplicateVersion.cc )
target_link_libraries ( 
  UMLDuplicateVersion 
    iUMLAPI
    ${UML_LIB_HACK} )

add_executable(
  UMLCreateTags 
    src/CreateTags.cc
    src/MaslTags.cc
    src/TranslatorTags.cc
  )
target_link_libraries ( 
  UMLCreateTags 
      iUMLAPI 
      ${UML_LIB_HACK}
  )

install ( 
  TARGETS
          UMLShowVersions 
          UMLFreezeVersion
          UMLDuplicateVersion
          UMLCreateTags
  EXPORT  iUML
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)
