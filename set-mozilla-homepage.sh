#! /bin/bash

##
##  This script will search for the mozilla prefs.js file under the
##+ $HOME/.mozilla/ dir and set the (startup) homepage to the current
##+ date record on Wikipedia.
##
##  This script needs to be called before igniting the firefox (bin).
##

## Find the prefs.js file under $HOME/.mozilla/ directory
PREFS_JS=$(find ${HOME}/.mozilla/ -name prefs.js)
if test -z $PREFS_JS ; then
    echo 1>2 "Failed to find mozilla's prefs.js file. Giving up"
    exit 1
fi

## Format the starting page, something like:
##+ https://en.wikipedia.org/wiki/August_3
## Beware how we treat backslashes!
WPAGE="https:\/\/en.wikipedia.org\/wiki\/$(date +"%B_%d")"

## Change the startup page inside the prefs.js file. This looks like:
##+ user_pref("browser.startup.homepage", "https://en.wikipedia.org/wiki/August_23");
sed -i "s/\(user_pref(\"browser\.startup\.homepage\"\, *\"\)\(.*\)\(\" *);$\)/\1"$WPAGE"\3/" $PREFS_JS

exit 0
