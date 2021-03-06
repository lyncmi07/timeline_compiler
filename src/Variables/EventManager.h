#ifndef MAJOR_EVENT_MANAGER_H
#define MAJOR_EVENT_MANAGER_H

#include <vector>
#include <iostream>

class Event;

class EventManager {
private:
  //owned memory
  std::vector<Event*> allEvents_;
  
  std::vector<int> eventOrder_;
  int startPointer_;
  int timelineSimulationPointer_;
  
  void putLatestInOrder();
public:

  EventManager();
  ~EventManager();

  void resetPointer();
  Event* nextEvent();
  std::vector<Event*>* allEvents();
  void addEvent(Event* newEvent);
  
  
  std::string printToXML(int spaceIndent);
};

#endif
