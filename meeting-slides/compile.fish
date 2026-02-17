#!/usr/bin/env fish

set SRC "./Q2"
set OUT "./out/Q2"
set TARGET ""
set FILE_PREFIX meeting-

argparse p/recompile "r/target=" -- $argv
or begin
    printf "Usage %s \n\t[-p] (recompile previously build PDF)\n\t[-r s<week-number] (compile only the specified meeting slides)"
end

if set -q _flag_p
    set COMPILE_ALL 1
end

if set -q _flag_r
    set TARGET $_flag_r
end

if not test -d "$OUT"
    mkdir -p "$OUT"
end

function compile_marp
    set md_file $argv[1]
    set base (basename "$md_file" .md)
    set pdf "$OUT/$base.pdf"

    if not test -f "$pdf"; or test "$COMPILE_ALL" -eq 1
        if string match -q "$FILE_PREFIX*" -- "$base"
            marp --pdf --allow-local-files --html -o "$pdf" "$md_file"
        end
    end
end

if test -n "$TARGET"
    set md "$SRC/meeting-$TARGET.md"

    if test -f "$md"
        compile_marp "$md"
        exit 0
    else
        echo "Target file $md does not exist"
        exit 1
    end
else
    for md in "$SRC"/*.md
        compile_marp "$md"
    end
end
