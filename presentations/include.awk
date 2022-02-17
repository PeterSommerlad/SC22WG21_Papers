/^\$include / { system("awk -f include.awk " $2); next;}
{ print; }
