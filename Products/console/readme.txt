After building there will be a ‘mockifer’ console application here, along with the package Mockifer javascript engine (in mockifer-js).

Starting the executable with no command line arguments will by default attempt to load the javascript engine from the ‘mockifer-js’ directory and will boot the server on port 8504.

To load the javascript engine from another location, specify the -d argument with a directory path, for example:

./mockifer -d ~/some-other-mockifer-js-directory

To listen on a different port than the default, specify the -p argument with a port number, for example:

./mockifer -p 8100

