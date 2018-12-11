echo off
lib /NOLOGO /OUT:nsfsdkvc.lib /MACHINE:IX86 /DEF:npnez.def
del nsfsdkvc.exp
coff2omf -q nsfsdkvc.lib nsfsdkbc.lib