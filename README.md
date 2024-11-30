# chtml

I did a thing where you can use html inline in c using a preprocessor. Felt inspired after tsoding and quickly hacked it up as a proof of concept. Just a bit of fun lol.

The parsing is very rudementary so the current syntax is like this:

```c
#include <chtml.h> // contains definitions for the CHTML_START and CHTML_END macros

...

int main()
{
    ...
    #define CHTML_EMIT(str) dprintf(fd, str)  // all html will get wrapped in CHTML_EMIT
	CHTML_START(
		<!DOCTYPE html>
		<body>
			<c> // c tags contain raw c
				for (int i = 1; i < 5; i++) {
					dprintf(fd, "<h%d> %d </h%d>", i, i, i);
				} 
			</c>
		</body>
	)CHTML_END

}
```

But the CHTML_START, CHTML_END, <c> and </c> tags each need to be on their own lines. Also doesnt account for comments.

## Building

    git clone git@github.com:JanGolicnik/cerv.git
    
    mkdir build && cd build

    cmake .. && make

## Usage

It parses every file given as input parameter and transforms them into file1_CHTML.c variants.

```c
chtml file1.c file2.c file3.c
```

There is an example cmakelists.txt given that automatically transforms all sources into approapriate files, creates the executable and cleans up after itself.