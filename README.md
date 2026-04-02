# Evalexpr

Programme en C permettant d’évaluer des expressions arithmétiques en notation infixée ou en notation polonaise inverse (RPN).

## Description

Le programme lit une expression depuis l’entrée standard et affiche le résultat sur la sortie standard.

Exemple :

```bash
echo "1 + 1" | ./evalexpr
2
```

Le projet supporte :
- notation classique (infixe)
- notation RPN avec l’option `-rpn`

## Fonctionnalités

### Opérations supportées

- `+` : addition
- `-` : soustraction
- `*` : multiplication
- `/` : division
- `%` : modulo
- `^` : puissance

### Notation infixe

- gestion des priorités d’opérateurs
- parenthèses `(` `)`
- opérateurs unaires (`+` et `-`)

### Notation RPN

Exemple :

```bash
echo "5 2 2 ^ 3 + *" | ./evalexpr -rpn
35
```

## Compilation

```bash
make
```
Génère le binaire `evalexpr` à la racine :contentReference[oaicite:0]{index=0}  

## Exécution

### Infixe

```bash
echo "5*(2^2+3)" | ./evalexpr
```

### RPN

```bash
echo "1 1 +" | ./evalexpr -rpn
```

## Tests

```bash
make check
```
Lance la testsuite (tests fonctionnels en shell) :contentReference[oaicite:1]{index=1}  

## Nettoyage

```bash
make clean
```

## Codes de retour

- `0` : succès
- `1` : erreur lexicale
- `2` : erreur syntaxique
- `3` : erreur arithmétique (ex : division par 0)
- `4` : autre erreur (ex : mauvais argument)

## Structure

```
evalexpr/
├── Makefile
├── src/
│   └── ...
└── tests/
    └── ...
```

## Contraintes

- Compilation avec :
  `-std=c99 -pedantic -Werror -Wall -Wextra -Wvla`
- Programme avec `main` obligatoire
- Lecture depuis stdin# Evalexpr
