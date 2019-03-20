// 
// Filename : Entity.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef iUMLAPI_Entity_HH
#define iUMLAPI_Entity_HH

#include <string>
#include <vector>
#include <stdexcept>
#include <set>

#include "boost/shared_ptr.hpp"

#include "iumlapi/iUMLAPI.hh"

namespace iUMLAPI
{

class Entity
{
  private:
    class Attribute
    {
      public:
        Attribute ( const API_ATTRIBUTE& attribute );
        Attribute ( const std::string& value );
        Attribute ( bool value );
        Attribute ( long value );

        bool getBool() const;
        long getLong() const;
        const std::string& getString() const;

        operator API_ATTRIBUTE() const;

      private:
        API_ATTR_TYPE type;
        std::string stringVal;
        long longVal;
        bool boolVal;
    };

  public:
    enum UMLDomain { None, ATT, BS, ORG, TAGS, XUML, DM, DS, REQ, SQ, UC };

    Entity();
    Entity(const API_ENTITY& other);


    Entity(const Entity& other);

    Entity& operator= ( const API_ENTITY& other );

    Entity& operator= ( const Entity& other );

    ~Entity();

    bool operator== ( const Entity& rhs ) const { return valid && rhs.valid && iUMLAPI::same_entity(rhs.entity,entity); }

    API_ATTR_TYPE getAttributeType ( const std::string& name, UMLDomain domain = XUML ) const;
    Attribute getAttribute ( const std::string& name, UMLDomain domain = XUML ) const;

    void setAttribute ( const std::string& name, const Attribute& value, UMLDomain domain = XUML );

    template<class T>
    std::vector<T> navigate ( const std::string& relationship, UMLDomain domain = XUML ) const;

    std::vector<Entity> navigate ( const std::string& relationship, UMLDomain domain = XUML ) const;

    Entity navigateOne ( const std::string& relationship, UMLDomain domain = XUML ) const;

    bool isValid() const { return valid; }
    API_ENTITY& getEntity() { return entity; }
    const API_ENTITY& getEntity() const { return entity; }

    void acquireTopLevelLock();
    void acquireAnalysisAreaLock();
    void acquireProjectVersionLock();

  private:
    API_ENTITY entity;
    bool valid;

    static UMLDomain currentDomain;
    static void setUMLDomain ( Entity::UMLDomain domain );

};

template<class ContainedType>
class EntityCollection
{
  public:
    typedef std::vector<ContainedType> Container;

    // Container
    typedef ContainedType     value_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;


    typedef typename Container::iterator               iterator;
    typedef typename Container::const_iterator         const_iterator;
    typedef typename Container::difference_type        difference_type;
    typedef typename Container::size_type              size_type;

    EntityCollection ( const EntityCollection& x ) : entities(x.entities) {}
    virtual ~EntityCollection() {}
    EntityCollection& operator= ( const EntityCollection& x ) { entities = x.entities; return *this; }

    size_type size() const { return entities.size(); }
    size_type max_size() const { return entities.max_size(); }
    bool empty() const { return entities.empty(); }
    void swap ( EntityCollection& x ) { entities.swap(x.entities); } 
    
    iterator begin() { return entities.begin(); }
    const_iterator begin() const { return entities.begin(); }

    iterator end() { return entities.end(); }
    const_iterator end() const { return entities.end(); }

    // ForwardContainer
    bool operator== ( const EntityCollection& x ) { return entities == x.entities; }
    bool operator!= ( const EntityCollection& x ) { return entities != x.entities; }
    bool operator<  ( const EntityCollection& x ) { return entities <  x.entities; }
    bool operator>  ( const EntityCollection& x ) { return entities >  x.entities; }
    bool operator<= ( const EntityCollection& x ) { return entities <= x.entities; }
    bool operator>= ( const EntityCollection& x ) { return entities >= x.entities; }


    // Reversible Container
    typedef typename Container::reverse_iterator       reverse_iterator;
    typedef typename Container::const_reverse_iterator const_reverse_iterator;

    reverse_iterator rbegin() { return entities.rbegin(); }
    const_reverse_iterator rbegin() const { return entities.rbegin(); }

    reverse_iterator rend() { return entities.rend(); }
    const_reverse_iterator rend() const { return entities.rend(); }

    // RandomAccessContainer
    reference operator[] ( size_type n ) { return entities[n]; }
    const_reference operator[] ( size_type n ) const { return entities[n]; }

    // Sequence

    EntityCollection(size_type n, const value_type& x) : entities(n,x) { }
    EntityCollection(size_type n) : entities(n) { }
    EntityCollection() : entities() { }

    template<class InputIterator>
    EntityCollection(InputIterator first, InputIterator last) : entities(first,last) {}
 
    iterator insert ( iterator p, const value_type& x) { return entities.insert(p,x); }
    iterator insert ( iterator p, size_type n, const value_type& x) { return entities.insert(p,n,x); }

    template<class InputIterator>
    void insert ( iterator p, InputIterator i, InputIterator j) { return entities.insert(p,i,j); }

    void erase (iterator pos) { entities.erase(pos); } 
    void erase (iterator first,iterator last) { entities.erase(first,last); } 

    reference front() { return entities.front(); }
    const_reference front() const { return entities.front(); }

    // BackInsertionSequence
    reference back() { return entities.back(); }
    const_reference back() const { return entities.back(); }
    
    void push_back(const value_type& x) { entities.push_back(x); }
    void pop_back() { entities.pop_back(); }

  protected:
    Container entities;
};

template<class T>
std::vector<T> Entity::navigate ( const std::string& relationship, UMLDomain domain ) const
{
  std::vector<Entity> tmp = navigate(relationship,domain);
  std::vector<T> result;
  for ( std::vector<Entity>::const_iterator it = tmp.begin();
        it != tmp.end(); ++it )
  {
    result.push_back(T(*it));
  }
  return result;
}


};

#endif
