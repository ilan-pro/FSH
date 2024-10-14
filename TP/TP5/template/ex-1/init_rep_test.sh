#! /bin/bash

cd /tmp
mkdir -p $USER
cd  $USER
mkdir test
cd test	
mkdir test_rep
mkdir .truc_bidule
echo "ô le joli fichier" > test_fic
ln -s test_fic test_lien
mkfifo test_fifo
touch test_exe1
touch test_exe2
chmod 010 test_exe1
chmod 001 test_exe2
mkdir rep
echo "ô un autre joli fichier" > rep/fic
ln -s rep/test_fic rep/lien
mkfifo fifo
touch rep/exe1
touch rep/exe2
chmod 010 rep/exe1
chmod 001 rep/exe2
	

