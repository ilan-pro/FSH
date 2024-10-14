.PHONY: init rep-test

init: work rep-test

work: template
	@sh -c 'if [ -e "work" ]; then echo "Erreur : le dossier \"work\" existe déjà"; exit 1; fi'
	cp -r template work

rep-test:
	@mkdir -p /tmp/$$USER/test/test_rep
	@mkdir -p /tmp/$$USER/test/.truc_bidule
	@echo "ô le joli fichier" > test_fic
	@ln -s test_fic test_lien
	@mkfifo test_fifo
	@touch test_exe1 test_exe2
	@chmod 510 test_exe1
	@chmod 001 test_exe2
	@cp -f test_{fic,lien,exe1} /tmp/$$USER/test/test_rep
	@mv -f test_{fic,lien,fifo,exe1,exe2} /tmp/$$USER/test/
	@mkfifo /tmp/$$USER/test/test_rep/test_fifo
	

