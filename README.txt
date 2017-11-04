Name: 			Matthew Cummins
Start Date:		30th October 2017
Project Title: 		Commodity Market System
Description: 		A platform for trading commodities contracts.
Extensions Added: 	None

OVERVIEW
The main body of the program consists of two classes, Market and Post. The Post class is a simple object containing the dealer name, side
and commodity in std::strings, the amount as an int, and the price as a double. The class contains basic constructors, accessors and 
mutator functions.

The Market object contains a std::map<int, Post>, where the integer is the order ID. This is the list that stores all of the orders. It has
one public function, process_input(std::string) which takes in the user input, splits the command into a vector of individual words, checks
that the first word is a valid dealer, and then sends off the vector of words to one of the post(), revoke(), check(), list() or aggress()
functions. Each of these check that the rest of the command is valid and does the required job.

If any of the checks return false, the flag_error(int) function is called to display the appropriate error message before the program 
requests new input. To do these checks, the Market class also has five functions with boolean output to tell if the given string is valid.
For example, the is_commodity_accpeted(std::string) function will only return true if the given string matches the name of one of the
accepted commodities, which are all stored in the const std::vector accepted_commodities.

The libraries that the program uses are <iostream>, <string>, <map>, <vector> and <sstream>.

To exit the program, use the command "EXIT".

KNOWN BUGS
This is about the command line arguments. Specifically, when handling the command line arguments for the extensions. To detect these, since 
the arguments are "ext1<PortNo>" and "ext2<PortNo>", the program simply looks at the fourth character and uses that to determine which exit 
code to display. This should still do the job since my program will only be ran using those 3 arguments and nothing else, but it still feels 
like a bug as other commands could cause the specific exit codes to run (for example, "abc1" would exit with exit code 1 even though it 
should not).

HOW TO BUILD
The code is split over four files, "Source.cpp", "CMarket.cpp", "CMarket.h" and "CPost.h". I have included my full Visual Studio 2017 
project file, as well as a folder called "CODE" containing just the .cpp and header files in case you wish to compile the program yourself.