#!/bin/bash

GLOBAL_SUCCESS=true

echo_red() {
  if [ -t 1 ] && command -v tput >/dev/null 2>&1; then
    echo -n "$(tput setaf 1)$(tput bold)" 2>/dev/null
    echo $@
    echo -n "$(tput sgr0)" 2>/dev/null
  else
    echo $@
  fi
}

compare_with_sed() {
    local SUCCESS=true
    local TMP_MY=$(mktemp)
    local TMP_SYS=$(mktemp)
    local RES_MY

    if [ "$INPUT" == "fichierinexistant" ]; then
        echo -n "Running \`./my-sed $@ NONEXISTENT_FILE\`... "
	rm -rf NONEXISTENT_FILE
	./my-sed $@ "NONEXISTENT_FILE" 2>/dev/null
    	RES_MY=$?
	sed -i s/$1/$2/g "NONEXISTENT_FILE" 2>/dev/null
    else
    	yes blablaaabla | head -c 5000 > "$TMP_MY"
    	cp "$TMP_MY" "$TMP_SYS"
 
    	echo -n "Running \`./my-sed $@ fic\`... "
    	./my-sed $@ "$TMP_MY" 2>/dev/null
   	RES_MY=$?
    	if [ -z "$INPUT" ]; then
	    #sed -i '' s/$1/$2/g "$TMP_SYS" 2>/dev/null #version bsd
	    sed -i s/$1/$2/g "$TMP_SYS" 2>/dev/null
    	else
	    #sed -i '' s/$2//g "$TMP_SYS"  2>/dev/null #version bsd
	    sed -i s/$2//g "$TMP_SYS"  2>/dev/null
    	fi 
    fi	
    local RES_SYS=$?
	
    if [ $RES_SYS -ne 0 ]; then
        RES_SYS=1
    fi
    cmp --silent "$TMP_MY" "$TMP_SYS"
    OUT=$?
    if [ $RES_MY -eq $RES_SYS -a $OUT -eq 0 ]; then
        echo "OK"
    else
        SUCCESS=false
        GLOBAL_SUCCESS=false
        echo_red "Failed"
    fi
    if [ ! $RES_MY -eq $RES_SYS ]; then
        echo " - Expected exit code $RES_SYS, got $RES_MY"
    fi
    if [ ! $OUT -eq 0 ]; then
        if [ $(dd "if=$TMP_SYS" bs=101 count=1 2>/dev/null | wc -c 2>/dev/null) -le 100 ]; then
            echo " - Expected output:"
            cat "$TMP_SYS"
            echo " - Actual output:"
            dd "if=$TMP_MY" bs=100 count=1 2>/dev/null
            if [ $(dd "if=$TMP_MY" bs=101 count=1 2>/dev/null | wc -c) -gt 100 ]; then
                echo "[TRUNCATED]"
            fi
        else
            echo " - Wrong output"
        fi
    fi

    if ! $SUCCESS ; then
        echo ""
    fi
}

check_valgrind() {
    echo -n "Running \`valgrind --leak-check=full \`echo "blablbbla" | ./my-sed $@\`... "
    valgrind --leak-check=full --error-exitcode=33 echo "blablabbla" | ./my-sed $@ >/dev/null 2>&1
    RES=$?
    if [ "$RES" -eq 33 ]; then
        echo_red "Failed"
    else
        echo "OK"
    fi
}
    
INPUT=
compare_with_sed "a" "i"
INPUT="delete"
compare_with_sed "-d" "a"
INPUT="fichierinexistant"
compare_with_sed "a" "i"

TMP_FIC=$(mktemp)
yes blablaaabla | head -c 5000 > "$TMP_FIC"

if command -v valgrind >/dev/null 2>&1; then
    check_valgrind l j "$TMP_FIC"
else
    echo "Could not run memory leaks check: please install valgrind on this computer if you can, otherwise ignore this line."
fi

