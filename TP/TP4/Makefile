.PHONY: init tarexamples

init: work tarexamples

work: templates
	cp -r templates work

tarexamples :
	echo "Z'ai cru voir passer un 'rominet" >| titi
	echo "Faprifti faufiffe !!!" >| grosminet
	yes "Vilain chat !" | head -3 >| meme
	chmod ug+x grosminet
	tar cf titi.tar titi grosminet meme
	rm titi grosminet meme
	head -c 3072 titi.tar >| sans_zeros.tar
	head -c 2602 titi.tar >| tronque.tar
	mkdir -p /tmp/$$USER
	mv titi.tar sans_zeros.tar tronque.tar /tmp/$$USER
	(cd templates; tar cf tarutils.tar tarutils.c tarutils.h ; mv tarutils.tar /tmp/$$USER)
