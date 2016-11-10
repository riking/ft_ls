#!/bin/sh

mkdir -p $1
cd $1
echo File contents > contents
touch file
ln file linkhard
ln -s file linksoft
ln -s linksoft linktwo
ln -s linktwo linkthree
ln -s nonexist brokenlink
touch noperm ; chmod 000 noperm
touch allperm ; chmod 777 allperm
touch allBits ; chmod 7777 allBits
touch allCaps ; chmod 7666 allCaps
mkdir stickyFolder ; chmod +t stickyFolder
mkdir allWFolder ; chmod o+w allWFolder
mkdir stickyAllWFolder ; chmod o+w stickyAllWFolder ; chmod +t stickyAllWFolder

mkdir subfolder
touch subfolder/file

mkdir dir_noperm
chmod 000 dir_noperm

