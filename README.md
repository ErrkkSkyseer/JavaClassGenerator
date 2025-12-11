# Hello this is Errkk Skyseer.

This is a program that can convert UML Diagram into java class - given that your UML Diagram is from Dia and it is a text.
It probably help you in some way or another. You guys(refer to my friends) have OOP homework to do.

-=========<3=========-

# Syntax Guide

text file is read line-by-line.
The first line is a class header. If you only have a single words(no whitespace) it will be parsed as "public class **classname**", otherwise the program will copy the line.
file name will be named as **classname**.java

### ==field==
\[Access_modifier_symbol\]\[field_name\]:\[field_type\]|\[get?,set?\]
<ul>
  <li>Access_modifier_symbol : 
    + for public
    # for protected
    - for private
  </li>
  <li>field_name : field name</li>
  <li>field_type : field type</li>
  <li>get?,set? : generate getter or setter for field</li>
</ul>

### ==function==
\[Access_modifier_symbol\]\[func_name\](\[params1\],\[param2\],...):\[func_type\]
param = \[param_name\]:\[param_type\]
<ul>
  <li>Access_modifier_symbol : 
    + for public
    # for protected
    - for private
  </li>
  <li>func_name : function name</li>
  <li>param : function parameter consist of param_name(parameter name) and param_type(parameter type). You can add as many param as you want.</li>
  <li>func_type : function type</li>
</ul>

**note**
<ul>
  <li>All whitespace is remove before parsing.</li>
  <li>For private field, parser will try to add "m_" to field_name if not already.</li>
  <li>Vertical bar ("|") is optional. You don't need it the field doesn't need getter/setter.</li>
  <li>Function with type that's not void will have defult return value. (0 for numeric, false for boolean, null for otherwise)</li>
</ul>
