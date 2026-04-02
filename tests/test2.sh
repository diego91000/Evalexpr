#!/bin/sh

t() {
    mode="$1"
    input="$2"
    expected="$3"
    desc="$4"

    [ "$mode" = "rpn" ] && opt="-rpn" || opt=""

    output=$(printf "%s" "$input" | ./evalexpr $opt 2>/dev/null)
    rc=$?
    if [ "$output" = "$expected" ] && [ "$rc" -eq 0 ]; then
        echo "[OK]  $desc"
    else
        echo "[FAIL] $desc"
        echo "  input:    '$input'"
        echo "  expected: '$expected' (rc=0)"
        echo "  got:      '$output' (rc=$rc)"
    fi
}

echo "-----------[TEST]-----------"
echo

t inf "4+3" "7" "add simple"
t inf "(2+4)*5" "30" "parentheses + mult"
t inf "14%5" "4" "modulo"
t inf "6*(3^2-1)+18/3-11%6" "49" "mix ops"
t inf "  7*  2 + 5" "19" "espaces"
t inf "0^1+0*9" "0" "priorite zero"
t inf "(3^(2^3))" "6561" "puissance assoc droite explicite"
t inf "3^2^3" "6561" "puissance assoc droite implicite"
t inf "-7+3*9" "20" "unaire negatif"
t inf "12%7*4+2^4^2" "65556" "mix fort/faible"
t inf "12345*8+67" "98827" "entiers grands"
t inf "(((4+3)))*(2+(1*(3+1)))" "42" "beaucoup de parens"
t inf "2---+-+5" "-3" "chaine d'unaires"
t inf "" "0" "vide = 0"

t rpn "4 3 +" "7" "rpn add"
t rpn "3 4 * 5 +" "17" "rpn mult+add"
t rpn "9 1 - 2 3 + *" "40" "rpn mix"
t rpn "12 7 % 4 * 2 4 2 ^ ^ +" "65556" "rpn gros mix"
t rpn "0 7 - 3 9 * +" "20" "rpn avec negatif"

