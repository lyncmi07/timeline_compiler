#include "EventManager.h"

#include "Event.h"
#include <iostream>

void EventManager::putLatestInOrder() {
  if(eventOrder_.size() == 0) {
    eventOrder_.push_back(-1);
    startPointer_ = 0;
  } else {
    eventOrder_.push_back(-1);
    int pointer = startPointer_;
    int previousPointer = -1;
    bool placeFound = false;
    while(pointer != -1) {
      if(pointer < eventOrder_.size() - 1) {
        if(allEvents_[pointer]->timelineOrderDate() > allEvents_[allEvents_.size()-1]->timelineOrderDate()) {
          //place for this event has been found
          eventOrder_[allEvents_.size()-1] = pointer;
          if(previousPointer == -1) {
            //the event is the earliest in the timeline therefore startPointer points to it
            startPointer_ = allEvents_.size()-1;
          } else {
            //set the previous object to point to the new object instead
            eventOrder_[previousPointer] = allEvents_.size()-1;
          }
          placeFound = true;
          break;
        }
      }
      
      previousPointer = pointer;
      pointer = eventOrder_[pointer];
    }
    if(!placeFound) {
      //if place isn't found yet then this event is placed at the end
      //previousPointer points to the last Event in the timeline
      eventOrder_[previousPointer] = allEvents_.size()-1;
    }
    
  }
}

EventManager::EventManager() {
  startPointer_ = -1;
  timelineSimulationPointer_ = -1;
}

EventManager::~EventManager() {
  for(int i = 0; i < allEvents_.size(); i++) {
    delete allEvents_[i];
  }
}

void EventManager::resetPointer() {
  timelineSimulationPointer_ = startPointer_;
}

Event* EventManager::nextEvent() {
  if(timelineSimulationPointer_ == -1) {
    return nullptr;
  }
  Event* nextEvent = allEvents_[timelineSimulationPointer_];
  timelineSimulationPointer_ = eventOrder_[timelineSimulationPointer_];
  return nextEvent;
}

std::vector<Event*>* EventManager::allEvents() {
  return &allEvents_;
}

void EventManager::addEvent(Event* newEvent) {
  allEvents_.push_back(newEvent);
  putLatestInOrder();
}

std::string EventManager::printToXML(int spaceIndent) {
  //return "event manager stuff";
  std::string returnString = "";
  std::string indent(spaceIndent,' ');
  returnString += indent + "<events>\n";
  
  //now print out all events in their timeline order
  resetPointer();
  Event* currentEvent = nextEvent();
  while(currentEvent != nullptr) {
    returnString += currentEvent->printToXML(spaceIndent + 2);
    currentEvent = nextEvent();
  }
  
  returnString += indent + "</events>\n";
  
  return returnString;
}
