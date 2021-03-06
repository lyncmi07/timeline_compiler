#include "EntityManager.h"
#include "Entity.h"

#include <iostream>

void EntityManager::putLatestInOrder() {
  if(entityOrder_.size() == 0) {
    entityOrder_.push_back(-1);
    startPointer_ = 0;
  } else {
    entityOrder_.push_back(-1);
    int pointer = startPointer_;
    int previousPointer = -1;
    bool placeFound = false;
    while(pointer != -1) {
      if(pointer < entityOrder_.size() - 1) {
        if(allEntities_[pointer]->timelineOrderDate() > allEntities_[allEntities_.size()-1]->timelineOrderDate()) {
          //place for this event has been found
          entityOrder_[allEntities_.size()-1] = pointer;
          if(previousPointer == -1) {
            //the entity is the earliest in the timeline therefore startPointer points to it
            startPointer_ = allEntities_.size()-1;
          } else {
            //set the previous object to point to the new object instead
            entityOrder_[previousPointer] = allEntities_.size()-1;
          }
          placeFound = true;
          break;
        }
      } 
      
      previousPointer = pointer;
      pointer = entityOrder_[pointer];
    }
    if(!placeFound) {
      //if place isn't found yet then this event is placed at the end
      //previousPointer points to the last Entity in the timeline
      entityOrder_[previousPointer] = allEntities_.size()-1;
    }
  }
}

EntityManager::EntityManager() {
  startPointer_ = -1;
  timelineSimulationPointer_ = -1;
}

EntityManager::~EntityManager() {
  for(int i = 0; i < allEntities_.size(); i++) {
    delete allEntities_[i];
  }
}

void EntityManager::resetPointer() {
  timelineSimulationPointer_ = startPointer_;
}

Entity* EntityManager::nextEntity() {
  if(timelineSimulationPointer_ == -1) {
    return nullptr;
  }
  Entity* nextEntity = allEntities_[timelineSimulationPointer_];
  timelineSimulationPointer_ = entityOrder_[timelineSimulationPointer_];
  return nextEntity;
}

std::vector<Entity*>* EntityManager::allEntities() {
  return &allEntities_;
}

void EntityManager::addEntity(Entity* newEntity) {
  allEntities_.push_back(newEntity);
  putLatestInOrder();
}

std::string EntityManager::printToXML(int spaceIndent) {
  std::string returnString = "";
  std::string indent(spaceIndent,' ');
  returnString += indent + "<entities>\n";
  
  resetPointer();
  Entity* currentEntity = nextEntity();
  while(currentEntity != nullptr) {
    returnString += currentEntity->printToXML(spaceIndent + 2);
    currentEntity = nextEntity();
  }
  
  returnString += indent + "</entities>\n";
  
  return returnString;
}




