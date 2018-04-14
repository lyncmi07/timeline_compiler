#include "animal.tpl"
#include "person.tpl"

println("running interview code");

string myName = "michael";
int birthDate = 1996;

string hello;
hello = "world";

print("My name is ");
printlnn(myName);
print("My birth date is ");
println(birthDate);

Person michael = new Person(myNames, birthDate, 2070)[];
michael.printout();

print("name: ");
println(michael.name);
