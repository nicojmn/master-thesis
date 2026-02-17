#!/usr/bin/env bash

SRC="./Q2"
OUT="./out/Q2"
TARGET=""
FILE_PREFIX="meeting-"

while getopts "pr:" opt; do
  case $opt in
    p)
      COMPILE_ALL=true
      ;;
    r)
      TARGET=$OPTARG
      ;;
    *)
      printf "Usage: ./$0
      \t[-p] (recompile previously build PDF)
      \t[-r s<week-number>] (compile only the specified meeting slides)\n"
      exit 1
      ;;
  esac
done

compile_marp() {
  local md_file=$1
  local base=$(basename "$md_file" .md)
  local pdf="$OUT/$base.pdf"
  if [[ ! -f "$pdf" || "$COMPILE_ALL" == true ]]; then
    if [[ "$base" == "$FILE_PREFIX"* ]]; then
      marp --pdf --allow-local-files --html -o $"$pdf" "$md_file"
    fi
  fi
}

if [ ! -d "$OUT" ]; then
	mkdir -p "$OUT"
fi

if [ -n "$TARGET" ]; then
  md="$SRC/meeting-$TARGET.md"

  if [ -f "$md" ]; then
    compile_marp "$md"
    exit 0
  else
    echo "Target file $md does not exist."
    exit 1
  fi
fi

for md in "$SRC"/*.md; do
  compile_marp "$md"
  done

