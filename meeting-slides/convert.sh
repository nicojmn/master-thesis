#!/usr/bin/env bash

SRC="."
OUT="./out"

if [ ! -d "$OUT" ]; then
	mkdir -p "$OUT"
fi

for md in "$SRC"/*.md; do
	base=$(basename "$md" .md)
    pdf="$OUT/$base.pdf"
	marp --pdf --allow-local-files --html -o $"$pdf" "$md"
done
