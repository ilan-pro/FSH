.PHONY: init clean

init: 
	@sh -c 'if [ -e "work" ]; then echo "Erreur : le dossier \"work\" existe déjà"; exit 1; fi'
	cp -r template work
	./template/ex-1/init_rep_test.sh
