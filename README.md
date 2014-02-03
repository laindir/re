#Regular Expressions

Regular expressions are a syntax for describing regular languages. These are
languages that can be parsed without any look ahead. The following elements are
available:

##literal

Any character that does not have a special meaning matches itself.

##.

The dot matches any character.

##*
The star operatior indicates that the preceding subexpression may be repeated
zero or more times to be matched by the star.

##\( and \)
Parentheses escaped with a backslash group tokens into a subexpression.

##\{x,y\}
Braces escaped with a backslash indicate that the preceding subexpression may
be repeated at least x times but not more than y times. If y is omitted, the
subexpression may be matched x or more times. If y and the comma are omitted,
the subexpression must be matched exactly x times.

##|
The pipe stands for alternation between two subexpressions. If either
subexpression matches, their alternation also matches.
