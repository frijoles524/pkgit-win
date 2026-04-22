#include <iostream>

#include "vars.hh"
#include "help.hh"

void help() {
  std::cout << ""+bold_magenta+"                              ,          \n";
  std::cout << "                             "+bold_magenta+"/ \\         \n";
  std::cout << "                            "+bold_magenta+"/   \\        \n";
  std::cout << "                        "+bold_magenta+"__-'     '-__    \n";
  std::cout << "                      "+bold_magenta+"''--__     __--''  "+bold_yellow+"\n";
  std::cout << "                         "+bold_yellow+"_--"+bold_magenta+"\\   /"+bold_yellow+"--_     \n";
  std::cout << "                     "+bold_yellow+"_--'    "+bold_magenta+"\\ /"+bold_yellow+"    '--_ \n";
  std::cout << "                    "+bold_yellow+"'-__      "+bold_magenta+"'"+bold_yellow+"      __-'\n";
  std::cout << "                        "+bold_yellow+"'-__     __-'    \n";
  std::cout << "                            "+bold_yellow+"'-_-'        "+color_reset+"\n";
  std::cout << "\n";
  std::cout << "                            pkgit\n";
  std::cout << "                       "+italic+""+gray+"- package it! -"+color_reset+"\n";
  std::cout << "                           "+magenta+"v"+version+""+color_reset+"\n";
  std::cout << "\n";
  std::cout << ""+red+"subcommands"+color_reset+":\n";
  std::cout << ""+color_reset+"├─ "+green+"a"+color_reset+",   "+yellow+"add "+blue+"[url, file]        "+gray+"# add a repo/repopkg\n";
  std::cout << ""+color_reset+"├─ "+green+"b"+color_reset+",   "+yellow+"build "+blue+"[path]           "+gray+"# build a package\n";
  std::cout << ""+color_reset+"├┬ "+green+"i"+color_reset+",   "+yellow+"install "+blue+"[pkgs, urls]   "+gray+"# install a package/repo\n";
  //std::cout << ""+color_reset+"│├── "+green+"-t:"+color_reset+", "+yellow+"--tag:"+blue+"[tag]          "+gray+"# specify a version\n";
  //std::cout << ""+color_reset+"│└── "+green+"-l:"+color_reset+", "+yellow+"--list:"+blue+"[filename]    "+gray+"# install from a package list\n";
  std::cout << ""+color_reset+"├─ "+green+"r"+color_reset+",   "+yellow+"remove "+blue+"[pkgs]          "+gray+"# remove an installed package\n";
  //std::cout << ""+color_reset+"├─ "+green+"f"+color_reset+",   "+yellow+"files "+blue+"[pkgs]           "+gray+"# list all files of a package\n";
  //std::cout << ""+color_reset+"├─ "+green+"s"+color_reset+",   "+yellow+"search "+blue+"[pkgs]          "+gray+"# search for packages\n";
  std::cout << ""+color_reset+"├─ "+green+"l"+color_reset+",   "+yellow+"list                   "+gray+"# list installed packages\n";
  std::cout << ""+color_reset+"└─ "+green+"u"+color_reset+",   "+yellow+"update                 "+gray+"# update all installed packages\n";
  std::cout << "\n";
  std::cout << ""+red+"flags"+color_reset+":\n";
  std::cout << ""+color_reset+"├─ "+green+"-h"+color_reset+",  "+yellow+"--help                 "+gray+"# display this help message\n";
  std::cout << ""+color_reset+"└─ "+green+"-v"+color_reset+",  "+yellow+"--version              "+gray+"# display version number\n";
}
