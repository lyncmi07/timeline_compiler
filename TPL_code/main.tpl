class Person extends Entity {

  int armNumber;
  
  _construct(string nameP, int startYearP, int endYearP) {
    name = nameP;
    startYear = startYearP;
    endYear = endYearP;
    armNumber = 2;
  }
  string theName(void) {
    return(name);
  }
  empty printObject(void) {
    println("Person:");
    string test = "hello";
    test = theName();
    println(theName());
    println(armNumber);
  }
  
  
  empty setEndYear(int nEndYear) {
    endYear = nEndYear;
  }
}


string newName = "Michael"+"lynch";
Person michael = new Person(newName, 1996, 2070)[];
int theName = 10;

