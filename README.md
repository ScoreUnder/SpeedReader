SpeedReader
===========

This is a cross-platform SpeedReader developed with Qt, forked from
@Vanniktech's repo. It is licensed under the GPL v2 terms.
[Speed reading](http://en.wikipedia.org/wiki/Speed_reading) is a technique to
read any kinds of texts super fast. Further forks/pulls are welcome. If you
have any problems or you have found any issues or bugs, feel free to report
them.

## Features
- read multiple words
- speedread from 300 up to 3000 words per minute
- customize text style and appearance
- number grouping (10 000 won't be displayed seperately, because of the whitespace; condition: one word at a time)
- stop- delay- and break-words
- longer words get displayed longer
- option to stall at indentions
- command line interface
- overview to see, what you've already read of your text and what is going to come
- calculation of estimated reading time

## Building

This project is built with Qt v5.15.2.

    qmake && make

## License
GPL v2

For more information see the [LICENSE file](LICENSE).
