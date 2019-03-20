// 
// Filename : APIGenObject.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenObject.hh"
#include "APIGenDomain.hh"
#include "APIGenUtils.hh"

#include <sstream>
#include <set>
#include <fstream>
#include <iostream>
#include <iterator>


APIGen::Object::Object ( const std::string& parentDomain_, const std::string& line )
  : parentDomain(parentDomain_)
{
  std::istringstream sline(line);
  
  std::getline(sline,number,',');
  std::getline(sline,name,',');
  std::getline(sline,keyLetters);

  className = camelCase(name,true);
  apiNamespace = "iUMLAPI";
  domainNamespace = camelCase(parentDomain,true);
  fileName = domainNamespace + "_" + className;
  fullClassName = apiNamespace + "::" + domainNamespace + "::" + className;

}

const APIGen::Domain& APIGen::Object::getParentDomain() const
{
 return Domains::getDomain(parentDomain);
}

bool APIGen::Object::isAssociativeOnly() const
{
  // If we get two relationships with the same name, neither 
  // of which are tagged as associative, then we must be an 
  // associative ourself. 
  if ( relationships.size() == 2 )
  {
    std::multimap<std::string,Relationship>::const_iterator it = relationships.begin();
    const Relationship& rel1 = it->second;
    const Relationship& rel2 = (++it)->second;

    if ( ( rel1.getName() == rel2.getName() ) &&
         ( !rel1.isAssoc() && !rel2.isAssoc() ) &&
         ( !rel1.isSuperSub() ) )
    {
      // We are an associative, but may still have useful information, 
      // so check to see whether we have attributes that do not reference the relationship
      for ( std::map<std::string,Attribute>::const_iterator it = attributes.begin();
            it != attributes.end(); ++it )
      {
        // References consist of a number of refs of the form 
        // "(Rxx.zzz)" or "(Rxx)" 

        std::string reference = it->second.getReference();
        while ( !reference.empty() )
        {
          std::string firstRef = reference.substr(1,reference.find(')') - 1 );
          reference.erase(0,firstRef.size()+2);
          // Strip the relationship number out
          std::string::size_type dotPos = firstRef.find('.');
          if ( dotPos != std::string::npos )
          {
            firstRef = firstRef.substr(0,dotPos-1);
          }
          if ( firstRef != rel1.getName() )
          {
            return false;
          }
        }
      }
      // All attributes checked, and nothing inconsistent with associative only
      return true;
    }
  }

  return false;
}

void APIGen::Object::stripAssocOnlyRels( const std::set<std::string>& assocOnlyObjs )
{
  for ( std::multimap<std::string,Relationship>::iterator it = relationships.begin();
        it != relationships.end(); )
  {
    if ( assocOnlyObjs.count(it->second.getToObject()) )
    {
      relationships.erase(it++);
    }
    else
    {
      ++it;
    }
  }

}


void APIGen::Object::generateAttributeAPI()
{
  for ( std::map<std::string,Attribute>::const_iterator it = attributes.begin();
        it != attributes.end();
        ++it )
  {
    const Attribute& att = it->second;
    if ( att.getReference() == "()" )
    {

      members          += "        mutable std::pair<bool," + att.getCppType() + "> " + att.getMemberName() + ";\n"; 
      getterDecs       += "        " + att.getCppRefType() + " " + att.getGetMethodName() + "() const;\n";
      getterDefs       += att.getCppRefType()+ " " + getFullClassName() + "::" + att.getGetMethodName() + "() const\n"
                        + "{\n"
                        + "  if ( !" + att.getMemberName() + ".first )\n"
                        + "  {\n"
                        + "    " + att.getMemberName() + ".second = entity.getAttribute(\"" + att.getName() + 
                            "\", Entity::" + getParentDomain().getName() + ")." + att.getCppTypeGetMethod() + "();\n"
                        + "    " + att.getMemberName() + ".first  = true;\n"
                        + "  }\n"
                        + "  return " + att.getMemberName() + ".second;\n"
                        + "}\n"
                        + "\n";
      if ( !att.isId() )
      {
        setterDecs     += "        void " + att.getSetMethodName() + " ( " + att.getCppRefType() + " value );\n";
        setterDefs     += "void " + getFullClassName() + "::" + att.getSetMethodName() + " ( " + att.getCppRefType() + " value )\n"
                        + "{\n"
                        + "  entity.setAttribute(\"" + att.getName() + 
                            "\", value, Entity::" + getParentDomain().getName() + ");\n"
                        + "  " + att.getMemberName() + ".second = value;\n"
                        + "  " + att.getMemberName() + ".first  = true;\n"
                        + "}\n"
                        + "\n";
      }
      resetBody         += "  " + att.getMemberName() + ".first = false;\n";
    }
  }
}

void APIGen::Object::generateRelationshipAPI()
{
  for ( std::multimap<std::string,Relationship>::const_iterator it = relationships.begin();
        it != relationships.end();
        ++it )
  {
    const Relationship& rel = it->second;
    const Object& toClass = getParentDomain().getObject(rel.getToObject());
    std::string toClassName = toClass.getClassName();
    std::string roleName = camelCase(rel.getRole(),false);
    std::string RoleName = camelCase(rel.getRole(),true);
    referencedObjects.insert(toClass);

    std::string attType;
    std::string attRefType;
    std::string attRefFullType;
    std::string attSetType = toClassName + "_Set";
    std::string attSetRefType = toClassName + "_Set&";
    std::string attSetRefFullType = toClass.getFullClassName() + "_Set&";

    if ( rel.isMany() )
    {
      attType = attSetType;
      attRefType = attSetRefType;
      attRefFullType = attSetRefFullType;
    }
    else
    { 
      attType = toClassName;
      attRefType = toClassName + "&";
      attRefFullType = toClass.getFullClassName() + "&";
    }

    std::string navString = rel.getName();
    if ( relationships.count(rel.getName()) > 1 )
    {
      // Check for reflexive
      if ( rel.getToObject() == keyLetters )
      {
        // Need to specify role too
        navString += "." + rel.getRole();
      }

      // Check for associative on reflexive
      std::multimap<std::string,Relationship>::const_iterator rel2 = relationships.lower_bound(rel.getName());
      std::multimap<std::string,Relationship>::const_iterator rel1 = rel2++;
      if ( rel1->second.getToObject() == rel2->second.getToObject() )
      {
        // Need to specify role too
        navString += "." + rel.getRole();
      }

      // Associative, so specify destination object
      navString+= "." + toClass.getName();
    }
    else
    {
      if ( rel.getToObject() == keyLetters )
      {
        // Reflexive, so use role too
        navString += "." + rel.getRole();
      }
    }

    std::string attName = roleName + toClassName + rel.getName();
    std::string AttName = RoleName + toClassName + rel.getName();

    resetBody        += "  " + attName + ".reset();\n";
    getterDecs       += "        const " + attRefType + " get" + AttName + "() const;\n"; 
    if ( !rel.isMany() ) 
    {
      getterDecs     += "        " + attRefType + " get" + AttName + "();\n";
    }
    setGetterDecs    += "        const " + attSetRefType + " get" + AttName + "() const;\n"; 
    members          += "        mutable boost::shared_ptr<" + attType + "> " + attName + ";\n";
    setMembers       += "        mutable boost::shared_ptr<" + attSetType + "> " + attName + ";\n";

    getterDefs       += "const " + attRefFullType + " " + getFullClassName() + "::get" + AttName + "() const\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attType + ">( new " + attType + "( entity.navigate" + (rel.isMany()?"":"One") + "(\"" + navString + "\", Entity::" + getParentDomain().getName() + ") ) );\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";
    if ( !rel.isMany() )
    {
      getterDefs     += attRefFullType + " " + getFullClassName() + "::get" + AttName + "()\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attType + ">( new " + attType + "( entity.navigate" + (rel.isMany()?"":"One") + "(\"" + navString + "\", Entity::" + getParentDomain().getName() + ") ) );\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";
    }

    setGetterDefs    += "const " + attSetRefFullType + " " + getFullClassName() + "_Set::get" + AttName + "() const\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attSetType + ">( new " + attSetType + "());\n"
                      + "    for ( " + getClassName() + "_Set::const_iterator it = entities.begin();\n"
                      + "          it != entities.end(); ++it )\n"
                      + "    {\n";
    if ( rel.isMany() )
    {
      setGetterDefs  += "      std::copy(it->get" + AttName + "().begin(),it->get" + AttName + "().end(),std::back_inserter(*" + attName + "));\n";
    }
    else
    {
      setGetterDefs  += "      if ( it->get" + AttName + "().isValid() ) " + attName + "->push_back(it->get" + AttName + "());\n";
    }
    setGetterDefs    += std::string() + "    }\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";
  }

}

void APIGen::Object::generateCounterpartAPI()
{
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = counterparts.begin();
        it != counterparts.end();
        ++it )
  {
    const Object& toClass = Domains::getDomain(it->first).getObject(it->second);
    std::string toClassName = toClass.getClassName();
    std::string toDomainName = toClass.getDomainNamespace();

    std::string attType = toDomainName + "::" + toClassName;
    std::string attRefType = toDomainName + "::" + toClassName + "&";
    std::string attRefFullType = toClass.getFullClassName() + "&";
    std::string attSetType = toDomainName + "::" + toClassName + "_Set";
    std::string attSetRefType = "const " + toDomainName + "::" + toClassName + "_Set&";
    std::string attSetRefFullType = "const " + toClass.getFullClassName() + "_Set&";

    std::string attName = toDomainName + "_" + toClassName;

    getterDecs       += "        const " + attRefType + " get" + attName + "() const;\n"; 
    getterDecs       += "        " + attRefType + " get" + attName + "();\n"; 
    setGetterDecs    += "        " + attSetRefType + " get" + attName + "() const;\n"; 
    members          += "        mutable boost::shared_ptr<" + attType + "> " + attName + ";\n";
    setMembers       += "        mutable boost::shared_ptr<" + attSetType + "> " + attName + ";\n";

    getterDefs       += "const " + attRefFullType + " " + getFullClassName() + "::get" + attName + "() const\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attType + ">( new " + attType + "(entity) );\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";

    getterDefs       += attRefFullType + " " + getFullClassName() + "::get" + attName + "()\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attType + ">( new " + attType + "(entity) );\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";

    setGetterDefs    += attSetRefFullType + " " + getFullClassName() + "_Set::get" + attName + "() const\n"
                      + "{\n"
                      + "  if ( !" + attName + " )\n"
                      + "  {\n"
                      + "    " + attName + " = boost::shared_ptr<" + attSetType + ">( new " + attSetType + "());\n"
                      + "    for ( " + getClassName() + "_Set::const_iterator it = entities.begin();\n"
                      + "          it != entities.end(); ++it )\n"
                      + "    {\n"
                      +"      if ( it->get" + attName + "().isValid() ) " + attName + "->push_back(it->get" + attName + "());\n"
                      + "    }\n"
                      + "  }\n"
                      + "  return *" + attName + ";\n"
                      + "}\n"
                      + "\n";

    referencedObjects.insert(toClass);

  }

}

void APIGen::Object::generateLookupAPI()
{
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = lookups.begin(); it != lookups.end(); ++it )
  {
    std::string relName = it->first;
    std::string attName = it->second;

    std::multimap<std::string,Relationship>::const_iterator relIt = relationships.find(relName);
    if ( relIt == relationships.end() )
    {
      std::cerr << "Could not find relationship " << relName << " on " << name << std::endl;
      break;
    }
    const Object& otherObj = getParentDomain().getObject(relIt->second.getToObject());
    std::map<std::string,Attribute>::const_iterator attIt = otherObj.getAttributes().find(attName);
    if ( attIt == otherObj.getAttributes().end() )
    {
      std::cerr << "Could not find attribute " << attName << " on " << name << std::endl;
      break;
    }
    const Attribute& att = attIt->second;

    getterDecs += "        bool " + camelCase(otherObj.getName(),false,true) + "Exists ( " + att.getCppRefType() + " " + att.getMemberName() + " ) const;\n"
                + "        const " + otherObj.getClassName() + "& get" + otherObj.getClassName() + " ( " + att.getCppRefType()  + " " + att.getMemberName() + " ) const;\n";

    members    += "        class " + otherObj.getClassName() + "Lookup\n"
                + "        {\n"
                + "          public:\n"
                + "            " + otherObj.getClassName() + "Lookup();\n"
                + "            bool initialised;\n"
                + "            std::map<" + att.getCppType() + ", const " + otherObj.getClassName() + "*> lookupTable;\n"
                + "            int resetCount;\n"
                + "        };\n"
                + "        void init" + otherObj.getClassName() + "Lookup() const;\n"
                + "        mutable " + otherObj.getClassName() + "Lookup " + camelCase(otherObj.getName(),false,true) + "Lookup;\n";

    std::string relGetter = "get" + camelCase(relIt->second.getRole(),true) + getParentDomain().getObject(relIt->second.getToObject()).getClassName() + relIt->second.getName();

    getterDefs += "" + getFullClassName() + "::" + otherObj.getClassName() + "Lookup::" + otherObj.getClassName() + "Lookup()\n"
                + " : initialised(false),\n"
                + "   resetCount(0)\n"
                + "{\n"
                + "}\n"
                + "\n"
                + "void " + getFullClassName() + "::init" + otherObj.getClassName() + "Lookup() const\n"
                + "{\n"
                + "  if ( resetCount != " + camelCase(otherObj.getName(),false,true) + "Lookup.resetCount )\n"
                + "  {\n"
                + "    // There's been a reset, so clear the cache\n"
                + "    " + camelCase(otherObj.getName(),false,true) + "Lookup = " + otherObj.getClassName() + "Lookup();\n"
                + "  }\n"
                + "\n"
                + "  if ( !" + camelCase(otherObj.getName(),false,true) + "Lookup.initialised )\n"
                + "  {\n"
                + "    " + camelCase(otherObj.getName(),false,true) + "Lookup.initialised = true;\n"
                + "    " + camelCase(otherObj.getName(),false,true) + "Lookup.resetCount = resetCount;\n"
                + "    for ( " + otherObj.getClassName() + "_Set::const_iterator it = " + relGetter + "().begin();\n"
                + "          it != " + relGetter + "().end();\n"
                + "          ++it )\n"
                + "    {\n"
                + "      " + camelCase(otherObj.getName(),false,true) + "Lookup.lookupTable.insert(std::pair<" +att.getCppType() + ",const " + otherObj.getClassName() + "*>(it->" + att.getGetMethodName() + "(),&*it));\n"
                + "    }\n"
                + "  }\n"
                + "}\n"
                + "\n"
                + "const " + otherObj.getFullClassName() + "& " + getFullClassName() + "::get" + otherObj.getClassName() + " ( " +att.getCppRefType() + " " +att.getMemberName() + "_ ) const\n"
                + "{\n"
                + "  init" + otherObj.getClassName() + "Lookup();\n"
                + "\n"
                + "  std::map<" +att.getCppType() + ",const " + otherObj.getClassName() + "*>::const_iterator it = " + camelCase(otherObj.getName(),false,true) + "Lookup.lookupTable.find(" +att.getMemberName() + "_);\n"
                + "\n"
                + "  if ( it == " + camelCase(otherObj.getName(),false,true) + "Lookup.lookupTable.end() )\n"
                + "  {\n"
                + "    throw std::out_of_range(\"" + otherObj.getClassName() + " \" + boost::lexical_cast<std::string>(" + att.getMemberName() + "_) + \" not found.\");\n"
                + "  }\n"
                + "  return *(it->second);\n"
                + "}\n"
                + "\n"
                + "bool " + getFullClassName() + "::" + camelCase(otherObj.getName(),false,true) + "Exists ( " +att.getCppRefType() + " " +att.getMemberName() + "_ ) const\n"
                + "{\n"
                + "  init" + otherObj.getClassName() + "Lookup();\n"
                + "  return " + camelCase(otherObj.getName(),false,true) + "Lookup.lookupTable.count(" +att.getMemberName() + "_);\n"
                + "}\n\n";
  }

}

void APIGen::Object::generateWriteAPI()
{
  generateCreateAPI();
  generateEditAPI();
  generateLinkAPI();
  generateDeleteAPI();
}

void APIGen::Object::generateCreateAPI()
{
  for ( std::vector<ApiCreateFunction>::const_iterator fnIt = apiCreateFns.begin(); fnIt != apiCreateFns.end(); ++fnIt )
  {
    const ApiCreateFunction& apiCreateFn = *fnIt;

    Object obj = getParentDomain().getObject(apiCreateFn.obj); 
    referencedObjects.insert(obj);

    std::string paramList;    
    std::string argList;    
    for ( std::vector<std::string>::const_iterator paramIt = apiCreateFn.params.begin(); paramIt != apiCreateFn.params.end(); ++paramIt )
    {
      std::string paramStr = *paramIt;
      size_t dotPos = paramStr.find(':');
      std::string paramType;
      std::string paramName;
      bool useActual = false;
      if ( dotPos != std::string::npos )
      {
        paramName = camelCase(paramStr.substr(0,dotPos),false) + "_";
        paramType = paramStr.substr(dotPos+1,paramStr.length()-dotPos-1);
        if ( paramType != "" && paramType[0] == '#' )
        {
          paramType = paramType.substr(1);
          useActual = true;
        }
        if ( paramType == "" ) paramType = paramStr.substr(0,dotPos);
      }
      else
      {
        paramName = camelCase(paramStr,false) + "_";
        paramType = paramStr;
      }

      std::map<std::string,Attribute>::const_iterator attIt = obj.getAttributes().find(paramType);
      if ( attIt != obj.getAttributes().end() )
      {
        paramType = attIt->second.getCppRefType();
      }
      else if ( paramType == "string" )
      {
        paramType = "const std::string&";
      }

      if ( paramIt != apiCreateFn.params.begin() )
      {
        paramList+= ", ";
      }
      paramList += paramType + " " + paramName;
      if ( paramType == "const std::string&" || useActual )
      {
        argList += paramName + ", ";
      }
      else
      {
        argList += "boost::lexical_cast<std::string>(" + paramName + "), ";
      }
    }

    writeDecs += "        " + obj.getClassName() + " " + camelCase(apiCreateFn.function,false) + " ( " + paramList + ");\n";
    writeDefs += obj.getFullClassName() + " " + getFullClassName() + "::" + camelCase(apiCreateFn.function,false) + " ( "+ paramList + ")\n"
               + "{\n"
               + "  reset();\n"
               + "  API_ENTITY newEntity;\n"
               + "  iUMLAPI::api_create(&API_" + apiCreateFn.function + ", entity.getEntity(), " + argList + "newEntity);\n"
               + "  " + obj.getFullClassName() + " newItem(newEntity);\n"
               + "  iUMLAPI::release_entity(newEntity);\n"
               + "  return newItem;\n"
               + "}\n\n";
  }
}

void APIGen::Object::generateEditAPI()
{
}

void APIGen::Object::generateLinkAPI()
{
}

void APIGen::Object::generateDeleteAPI()
{
  if ( apiDeleteFn != "" )
  {
    writeDecs += "        void remove();\n";
    writeDefs += "void " + getFullClassName() + "::remove()\n"
               + "{\n"
               + "  reset();\n" 
               + "  iUMLAPI::api_delete(&API_" + apiDeleteFn + ", entity.getEntity() );\n"
               + "  entity = iUMLAPI::Entity();\n" 
               + "}\n\n" ;

  }
}

void APIGen::Object::generateExtrasAPI()
{
  std::ifstream headerExtras ( (getFileName() + ".extras.hh").c_str());
  if ( headerExtras )
  {
    std::ostringstream headerExtrasStream;
    headerExtrasStream << headerExtras.rdbuf();
    extrasHeader = headerExtrasStream.str();
  }

  std::ifstream classExtras ( (getFileName() + ".class_extras.hh").c_str());
  if ( classExtras )
  {
    std::ostringstream classExtrasStream;
    classExtrasStream << classExtras.rdbuf();
    extrasDecs = classExtrasStream.str();
  }

  std::ifstream codeExtras ( (getFileName() + ".extras.cc").c_str());
  if ( codeExtras )
  {
    std::ostringstream codeExtrasStream;
    codeExtrasStream << codeExtras.rdbuf();
    extrasDefs = codeExtrasStream.str();
  }
}

void APIGen::Object::generateReferences()
{
  for ( std::set<Object>::const_iterator it = referencedObjects.begin(); it != referencedObjects.end(); ++it )
  { 
    forwardDecs += "  namespace " + it->getDomainNamespace() + " { class " + it->getClassName() + "; class " + it->getClassName() + "_Set; }\n";
    includes += "#include \"iumlapi/" + it->getFileName() + ".hh\"\n"; 
  }
}

void APIGen::Object::generateAPI( const std::string& outputdir )
{
  std::cout << "Generating API for " << getFullClassName() << std::endl;

  generateAttributeAPI();
  generateRelationshipAPI();
  generateCounterpartAPI();
  generateLookupAPI();
  generateWriteAPI();
  generateExtrasAPI();
  
  generateReferences();  

  std::string header    = std::string("//\n") 
                        + "// Filename : " + getFileName() + ".hh\n"
                        + "//\n" 
                        + "// Description : Auto generated declaration for " + getFullClassName() + "\n" 
                        + "//\n" 
                        + "// UK Crown Copyright (c) 2016. All Rights Reserved.\n" 
                        + "//\n" 
                        + "#ifndef " + getFileName() + "_HH\n"
                        + "#define " + getFileName() + "_HH\n"
                        + "\n"
                        + "#include \"iumlapi/Entity.hh\"\n"
                        + "#include \"boost/shared_ptr.hpp\"\n"
                        + "#include <string>\n"
                        + "#include <vector>\n"
                        + "#include <map>\n"
                        + "\n"
                        + extrasHeader
                        + "\n"
                        + "namespace " + getAPINamespace() + "\n"
                        + "{\n"
                        + "\n"
                        + forwardDecs
                        + "\n"
                        + "  namespace " + getDomainNamespace() + "\n"
                        + "  {\n"
                        + "    class " + getClassName() + "\n"
                        + "    {\n"
                        + "      public:\n"
                        + "        " + getClassName() + " ( const Entity& entity );\n" 
                        + "        " + getClassName() + " ();\n" 
                        + "        bool isValid() const { return entity.isValid(); }\n"
                        + "        const Entity& getEntity() const { return entity; }\n"
                        + "        bool operator==( const " + getClassName() + "& rhs ) const { return entity == rhs.entity; }\n"
                        + "        void reset();\n"
                        + "\n"
                        + getterDecs
                        + "\n"
                        + setterDecs
                        + "\n"
                        + writeDecs
                        + "\n"
                        + extrasDecs
                        + "\n"
                        + "      private:\n"
                        + "        Entity entity;\n"
                        + "        int resetCount;\n"
                        + "\n"
                        + members
                        + "\n"
                        + "    };\n"
                        + "\n"
                        + "    class " + getClassName() + "_Set : public EntityCollection<" + getClassName() + ">\n"
                        + "    {\n"
                        + "      public:\n"
                        + "        " + getClassName() + "_Set();\n" 
                        + "        " + getClassName() + "_Set ( const std::vector<Entity>& entities );\n" 
                        + "\n"
                        + setGetterDecs
                        + "\n"
                        + "      private:\n"
                        + setMembers
                        + "\n"
                        + "    };\n"
                        + "\n"
                        + "  }\n"
                        + "}\n"
                        + "\n"
                        + "#endif\n";
  
  std::ofstream headerFile( (outputdir + "/include/iumlapi/" + getFileName() + ".hh").c_str() );
  headerFile << header << std::flush;

  std::string code      = std::string("//\n") 
                        + "// Filename : " + getFileName() + ".cc\n"
                        + "//\n" 
                        + "// Description : Auto generated definition for " + getFullClassName() + "\n" 
                        + "//\n" 
                        + "// UK Crown Copyright (c) 2016. All Rights Reserved.\n" 
                        + "//\n" 
                        + "#include \"iumlapi/" + getFileName() + ".hh\"\n"
                        + "#include \"boost/lexical_cast.hpp\"\n"
                        + includes
                        + "\n"
                        + "" + getFullClassName() + "::" + getClassName() + " ( const Entity& entity )\n" 
                        + "  : entity(entity),\n"
                        + "    resetCount(0)\n"
                        + "{\n"
                        + "}\n"
                        + "\n"
                        + "" + getFullClassName() + "::" + getClassName() + " ()\n" 
                        + "  : entity(),\n"
                        + "    resetCount(0)\n"
                        + "{\n"
                        + "}\n"
                        + "\n"
                        + getterDefs
                        + setterDefs
                        + writeDefs
                        + extrasDefs
                        + "void " + getFullClassName() + "::reset()\n" 
                        + "{\n"
                        + "  ++resetCount;\n"
                        + resetBody
                        + "}\n"
                        + "\n"
                        + "" + getFullClassName() + "_Set::" + getClassName() + "_Set()" 
                        + "{\n"
                        + "}\n"
                        + "\n"
                        + "" + getFullClassName() + "_Set::" + getClassName() + "_Set ( const std::vector<Entity>& entities )" 
                        + "  : EntityCollection(entities.begin(),entities.end())\n"
                        + "{\n"
                        + "}\n"
                        + "\n"
                        + setGetterDefs;

  std::ofstream codeFile( (outputdir + "/src/" + getFileName() + ".cc").c_str() );
  codeFile << code << std::flush;
}

