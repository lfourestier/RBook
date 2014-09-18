/**
 * @mainpage
 *
 * @section intro_sec Introduction
 * The RBook component is a road book library that provides utilities to decode road book files and pass information to an upper application that
 * will display them. 
 * 
 * @section conventionsection Conventions
 * - @ref namingconventions
 * - @ref logconventions
 *
 */

/** @page namingconventions Naming conventions
 *
 * Conceptually, we want names to be meaningful, only.
 * What is more meaningful than an actual sentence then?... So let's use sentences!
 * If you want to be meaningful is your names (Function, class, variable...), just write them as a sentence, whatever length it is.
 * Eg: "this is my variable for counting the sheep"
 *
 * BUT, spaces and punctuation are not allowed!...
 *
 * So here are methods to transform a sentence into a name (By order of preferences):
 *
 * -# "adapted camel case": each word in a sentence are concatenated (no spaces, no punctuation) and each word's first letter is "upper cased".
 *    Eg: "this is my variable for counting the sheep" => ThisIsMyVariableForCountingTheSheep.
 *
 * -# "Full upper case with underscore": each word in a sentence are concatenated (no spaces, no punctuation) separated by an underscore '_'
 *    and all letters are "upper cased"
 *    Eg: "this is my variable for counting the sheep" => THIS_IS_MY_VARIABLE_FOR_COUNTING_THE_SHEEP
 *
 * -# "full lower case concatenation with underscore": each word in a sentence are concatenated (no spaces, no punctuation) separated by an underscore '_'
 *    and all letters are lower cased.
 *    Eg: "my variable" => my_variable
 *
 * -# "full lower case concatenation": each word in a sentence are concatenated (no spaces, no punctuation) and all letters are lower cased.
 *    Eg: "my variable" => myvariable
 *
 * The mandatory rules:
 * - Any names should respect one of these methods (With a recommendation for the first one)
 * - DO NOT shorten the words in your sentences. eg: for "my conventional variable", NO "MyConvVar" BUT "MyConventionalVariable"
 * - Please, no stupid prefix or suffix (eg: "mytype_t", mMyMember, _myprivate...) to say it is a type, a member or else...
 *   - Optionally, tell it in your sentence. eg: ThisIsMyObjectType...
 *   - But surely, use a proper IDE that gives you all required details of a name in a pop up!!!
 * Optional rules:
 * - The second method is used for macros and enumeration members.
 * - The third and fourth method can be used interchangeably for naming of local variables (within a limited scope).
 *   In that case the sentence MUST stay short (two words max). eg: my_variable.
 *
 * I know it is not common... But I don't care! This is more readable like this.
 *
 */

/** @page logconventions Log conventions
 *
 * RBook provides a log component @see Log.h. You can use it to punctuate the code with logs (eg: LOG_V, LOG_D...). This can be useful!
 *
 * Main rule:
 * - Verbose mode is used to expose data or values. (eg: LOG_V(tag, "Loaded the file with content: %s", filecontent); )
 * - Debug mode is used to mark the passage at a certain point in the code. (eg: LOG_D(tag, "Function/event %s has been triggered!" , eventname); )
 * - Info mode is used to mark global functionality or component events. (eg: LOG_I(tag, "Start the book manager!"); )
 * - Warning mode is used to report an unimportant, recoverable or recovered issue. (eg: LOG_W(tag, "List is empty!"); )
 * - Error mode is used to report important errors, for which the user or the upper layer are requested to interact. (eg: LOG_E(tag, "pointer is null!); )
 * - Fatal mode is used to report just before dying. (eg: LOG_F(tag, "Library is not found! Closing application!); )
 *
 */






