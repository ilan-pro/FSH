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

compare_with_tr() {
    local SUCCESS=true
    local TMP_MY=$(mktemp)
    local TMP_SYS=$(mktemp)
    local TMP_FIC=$(mktemp)

    if [ -z "$INPUT" ]; then
	yes blablaaabla | head -c 5000 > "$TMP_FIC"
	echo -n "Running \`cat fic | ./my-tr $@\`... "
	cat "$TMP_FIC" | ./my-tr $@ > "$TMP_MY" 2>/dev/null
	local RES_MY=$?
	cat "$TMP_FIC" | tr $@ 2>/dev/null > "$TMP_SYS" 
	local RES_SYS=$?
    else
	echo -n "Running \`echo '$INPUT' | ./my-tr $@\`... "
	echo "$INPUT" | ./my-tr $@ > "$TMP_MY" 2>/dev/null
	local RES_MY=$?
	echo "$INPUT" | tr $@ >/dev/null 2>&1
	local RES_SYS=$?
	echo "$INPUT" | tr $@ 2>/dev/null > "$TMP_SYS" 2>/dev/null
    fi
	
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
    echo -n "Running \`valgrind --leak-check=full \`echo "blablabbla" | ./my-tr $@\`... "
    valgrind --leak-check=full --error-exitcode=33 echo "blablabbla" | ./my-tr $@ >/dev/null 2>&1
    RES=$?
    if [ "$RES" -eq 33 ]; then
        echo_red "Failed"
    else
        echo "OK"
    fi
}

INPUT='Hello, world!'
compare_with_tr o U
compare_with_tr "-d" o

INPUT=
compare_with_tr a i
compare_with_tr "-d" a

if command -v valgrind >/dev/null 2>&1; then
    check_valgrind l j
else
    echo "Could not run memory leaks check: please install valgrind on this computer if you can, otherwise ignore this line."
fi
