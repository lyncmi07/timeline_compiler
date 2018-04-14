#ifndef MAJOR_ENTITY_MANAGER_H
#define MAJOR_ENTITY_MANAGER_H

#include <vector>
#include <string>

class Entity;

class EntityManager {
private:
  //owned memory
  std::vector<Entity*> allEntities_;
  
  std::vector<int> entityOrder_;
  int startPointer_;
  int timelineSimulationPointer_;
  
  void putLatestInOrder();
public:
  EntityManager();
  ~EntityManager();
  
  void resetPointer();
  Entity* nextEntity();
  std::vector<Entity*>* allEntities();
  void addEntity(Entity* newEntity);
  
  std::string printToXML(int spaceIndent);
};

#endif
