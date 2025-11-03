#!/usr/bin/env bash

SRC="."
OUT="./out"

while getopts "p" opt; do
  case $opt in
    p)
      COMPILE_ALL=true
      ;;
    *)
      echo "Usage: $0 [-p] (recompile previously build PDF)"
      exit 1
      ;;
  esac
done

if [ ! -d "$OUT" ]; then
	mkdir -p "$OUT"
fi

if [ "$COMPILE_ALL" == true ]; then
	for md in "$SRC"/*.md; do
		base=$(basename "$md" .md)
		pdf="$OUT/$base.pdf"
		marp --pdf --allow-local-files --html -o $"$pdf" "$md"
	done
else
for md in "$SRC"/*.md; do
    base=$(basename "$md" .md)
    pdf="$OUT/$base.pdf"

    
    if [ ! -f "$pdf" ]; then
      marp --pdf --allow-local-files --html -o $"$pdf" "$md"
    fi
  done
fi

