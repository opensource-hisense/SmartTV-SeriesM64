#!/bin/sh
[ -L drivers/mstar2/utopia2k ] && rm -f drivers/mstar2/utopia2k
[ -e ./drvers/mstar2/utopia2k ] && rm -f drivers/mstar2/utopia2k
[ ! -L ./drvers/mstar2/utopia2k ] && ln -s ../../utopia_develop/utopia2K ./drivers/mstar2/utopia2k 
